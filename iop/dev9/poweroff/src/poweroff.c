/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

#include "types.h"
#include "defs.h"
#include "irx.h"
#include "loadcore.h"
#include "sysmem.h"
#include "stdio.h"
#include "sysclib.h"
#include "sifcmd.h"
#include "thbase.h"
#include "intrman.h"
#include "loadcore.h"
#include "thsemap.h"
#include "poweroff.h"
#include "pwroff_rpc.h"

//#define DEBUG

#define TYPE_C		1
#define CDVDreg_PWOFF	(*(volatile unsigned char*)0xBF402008)

#define MAX_CALLBACKS	8

#define MODNAME "Poweroff_Handler"
IRX_ID(MODNAME, 1, 1);

#define M_PRINTF(format, args...) \
    printf(MODNAME ": " format, ##args)

#ifdef DEBUG
#define M_DEBUG M_PRINTF
#else
#define M_DEBUG(format, args...)
#endif

extern struct irx_export_table _exp_poweroff;

//---------------------------------------------------------------------

static void Shutdown(void* data);
static void SendCmd(void* data);

//---------------------------------------------------------------------
typedef int (*intrhandler)(void*);

static intrhandler	oldCdHandler=0;

struct handlerTableEntry{
	intrhandler	handler;
	void		*param;
};

struct CallbackEntry
{
	pwoffcb cb;
	void *data;
} CallbackTable[MAX_CALLBACKS];

//---------------------------------------------------------------------

static char cmdData[16];
static pwoffcb poweroff_button_cb = 0;
static void *poweroff_button_data = 0;
static struct t_SifRpcDataQueue qd;
static struct t_SifRpcServerData sd0;
static int PowerOffThreadID = -1;
static SifRpcClientData_t client;

static int myCdHandler(void *param)
{
	if (((CDVDreg_PWOFF & 1)==0) && (CDVDreg_PWOFF & 4))
	{
		/* can't seem to register a sif cmd callback in ps2link so... */
		/* Clear interrupt bit */
		CDVDreg_PWOFF = 4;
		M_DEBUG("Poweroff!!!! %08x\n", CDVDreg_PWOFF);

		if (poweroff_button_cb)
			poweroff_button_cb(poweroff_button_data);
	}

	return oldCdHandler(param);
}

static void Shutdown(void* data)
{
	(void)data;

	M_DEBUG("Shutdown\n");
	int i;
	/* Do callbacks in reverse order */
	for(i = MAX_CALLBACKS-1; i >= 0; i--)
	{
		if(CallbackTable[i].cb)
		{
			CallbackTable[i].cb(CallbackTable[i].data);
		}
	}

	// Turn off PS2
	*((unsigned char *)0xBF402017) = 0;
	*((unsigned char *)0xBF402016) = 0xF;
}

static void SendCmd(void* data)
{
	(void)data;

	iWakeupThread(PowerOffThreadID);
}

static void PowerOffThread(void *arg)
{
	(void)arg;

	SleepThread();
	sceSifCallRpc(&client, POFF_RPC_BUTTON, 0, NULL, 0, NULL, 0, NULL, NULL);
}

static void InitPowerOffThread(void)
{
	iop_thread_t thread;

	if(PowerOffThreadID < 0)
	{
		thread.thread = &PowerOffThread;
		thread.attr = TH_C;
		thread.option = PWROFF_IRX;
		thread.stacksize = 0x400;
		thread.priority = 0x27;
		PowerOffThreadID = CreateThread(&thread);
		StartThread(PowerOffThreadID, NULL);

		client.server = NULL;
		while(sceSifBindRpc(&client, PWROFF_IRX, 0) < 0 || client.server == NULL) DelayThread(500);
	}
}

//---------------------------------------------------------------------
//-----------------------------------------------------------entrypoint
//---------------------------------------------------------------------

void SetPowerButtonHandler(pwoffcb func, void* param)
{
	poweroff_button_cb = func;
	poweroff_button_data = param;
}

void AddPowerOffHandler(pwoffcb func, void* param)
{
	int i;

	for(i = 0; i < MAX_CALLBACKS; i++)
	{
		if(CallbackTable[i].cb == 0)
		{
			CallbackTable[i].cb = func;
			CallbackTable[i].data = param;
			M_DEBUG("Added callback at position %d\n", i);
			break;
		}
	}

	if(i == MAX_CALLBACKS)
	{
		M_PRINTF("Could not add poweroff callback\n");
	}
}

void RemovePowerOffHandler(pwoffcb func)
{
	int i;

	for(i = 0; i < MAX_CALLBACKS; i++)
	{
		if(CallbackTable[i].cb == func)
		{
			break;
		}
	}

	if(i < MAX_CALLBACKS)
	{
		for(; i < (MAX_CALLBACKS-1); i++)
		{
			CallbackTable[i] = CallbackTable[i+1];
		}
		memset(&CallbackTable[i], 0, sizeof(struct CallbackEntry));
	}
}

void PoweroffShutdown()
{
	Shutdown(NULL);
}

void* poweroff_rpc_server(int fno, void *data, int size)
{
	(void)size;

	switch(fno) {
	case PWROFF_SHUTDOWN:
		Shutdown(NULL);
		break;

	case PWROFF_ENABLE_AUTO_SHUTOFF:
		InitPowerOffThread();

		int* sbuff = data;
		if (sbuff[0])
			SetPowerButtonHandler(Shutdown, 0);
		else
			SetPowerButtonHandler(SendCmd, 0);
		sbuff[0] = 1;
		return sbuff;
	}
	return NULL;
}

void poweroff_rpc_Thread(void* param)
{
	(void)param;

	sceSifInitRpc(0);

	sceSifSetRpcQueue(&qd, GetThreadId());
	sceSifRegisterRpc(&sd0, PWROFF_IRX, poweroff_rpc_server, cmdData, NULL, NULL, &qd);
	sceSifRpcLoop(&qd);
}

int _start(int argc, char *argv[])
{
	register struct handlerTableEntry *handlers=(struct handlerTableEntry*)0x480;//iopmem
	iop_thread_t mythread;

	(void)argc;
	(void)argv;

	if(RegisterLibraryEntries(&_exp_poweroff) != 0)
	{
		M_PRINTF("Poweroff already registered\n");
		return MODULE_NO_RESIDENT_END;
	}

	SetPowerButtonHandler(Shutdown, 0);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
	if (handlers[IOP_IRQ_CDVD].handler==0) {
		M_PRINTF("No CDROM handler. Run CDVDMAN first\n");
		return MODULE_NO_RESIDENT_END;
	}

	if (((int)handlers[IOP_IRQ_CDVD].handler & 3) != TYPE_C){
		M_PRINTF("Cannot chain to non-C handler\n");
		return MODULE_NO_RESIDENT_END;
	}

	oldCdHandler=(intrhandler)((int)handlers[IOP_IRQ_CDVD].handler & ~3);
	handlers[IOP_IRQ_CDVD].handler=(intrhandler)((int)myCdHandler | TYPE_C);
#pragma GCC diagnostic pop

	memset(CallbackTable, 0, sizeof(struct CallbackEntry) * MAX_CALLBACKS);

	mythread.attr = TH_C;
	mythread.option = PWROFF_IRX;
	mythread.thread = poweroff_rpc_Thread;
	mythread.stacksize = 0x1000;
	mythread.priority = 0x27;

	int pid = CreateThread(&mythread);

	if (pid > 0) {
		int i;

		if ((i=StartThread(pid, NULL)) < 0) {
			M_PRINTF("StartThread failed (%d)\n", i);
			return MODULE_NO_RESIDENT_END;
		}
	}
	else {
		M_PRINTF("CreateThread failed (%d)\n", pid);
		return MODULE_NO_RESIDENT_END;
	}

	M_PRINTF("Poweroff installed\n");
	return MODULE_RESIDENT_END;
}
