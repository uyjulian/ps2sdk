/*
 * Copyright (c) 2007 Lukasz Bruun <mail@lukasz.dk>
 *
 * See the file LICENSE included with this distribution for licensing terms.
 */

/**
 * @file
 * IOP pad driver
 */

#ifndef __FREEPAD_H__
#define __FREEPAD_H__

#include "irx.h"
#include "types.h"

#define MODNAME "freepad"
#define M_PRINTF(format, args...)	printf(MODNAME ": " format, ## args)
#define M_KPRINTF(format, args...)	Kprintf(MODNAME ": " format, ## args)

#ifdef DEBUG
#define D_PRINTF(format, args...)	printf(MODNAME ": " format, ## args)
#else
#define D_PRINTF(a...)		(void)0
#endif

#define PADMAN_THPRI_LO	46
#define PADMAN_THPRI_HI	20

#define SB_STAT	*((volatile unsigned int*)0xBD000040)

/*
 * Pad states
 */
#define PAD_STATE_DISCONN     	0x00
#define PAD_STATE_FINDPAD       0x01
#define PAD_STATE_FINDCTP1      0x02
#define PAD_STATE_EXECCMD       0x05
#define PAD_STATE_STABLE        0x06
#define PAD_STATE_ERROR      	0x07

/*
 * Pad request states
 */
#define PAD_RSTAT_COMPLETE      0x00
#define PAD_RSTAT_FAILED        0x01
#define PAD_RSTAT_BUSY          0x02

/*
 * Connected pad type
 */
#define PAD_TYPE_NEJICON		0x2
#define PAD_TYPE_KONAMIGUN  	0x3
#define PAD_TYPE_DIGITAL    	0x4
#define PAD_TYPE_ANALOG	    	0x5
#define PAD_TYPE_NAMCOGUN   	0x6
#define PAD_TYPE_DUALSHOCK		0x7

#define PAD_MODEL_DUALSHOCK		0x1
#define PAD_MODEL_DUALSHOCK2	0x3

#define MODE_CONFIG_NO_PAD		0x0
#define MODE_CONFIG_QUERY_PAD	0x1
#define MODE_CONFIG_READY		0x2

// PAD Tasks
#define TASK_NONE				0
#define	TASK_UPDATE_PAD			1
#define TASK_QUERY_PAD			2
#define TASK_PORT_CLOSE			3
#define TASK_SET_MAIN_MODE		4
#define TASK_SET_ACT_ALIGN		5
#define TASK_SET_BUTTON_INFO	6
#define TASK_SET_VREF_PARAM		7

// Main event flags
#define EF_UPDATE_PAD			0x0001
#define EF_QUERY_PAD			0x0002

#define EF_SET_MAIN_MODE		0x0008
#define EF_SET_ACT_ALIGN		0x0010
#define EF_SET_SET_BUTTON_INFO	0x0020
#define EF_SET_VREF_PARAM		0x0040
#define EF_PAD_TRANSFER_START	0x0080
#define EF_PAD_TRANSFER_DONE	0x0100
#define EF_TASK_DONE			0x0200
#define EF_PORT_CLOSE			0x0400
#define EF_EXIT_THREAD			0x1000

// Vblank event flags
#define EF_VB_TRANSFER			0x0001
#define EF_VB_TRANSFER_DONE		0x0004
#define EF_VB_WAIT_THREAD_EXIT	0x0008

// Global Types
typedef struct
{
	int padEnd;
	int eventflag;
	int init;
	int	stopTransfer;
	int tid_1;
	int tid_2;
} vblankData_t;

typedef struct
{
    u8 data[32];
	u32 actDirData[2];
	u32 actAlignData[2];
    u32 actData[4];
    u32 combData[4];
    u32 modeTable[2];
    u32 frame;
    u32 findPadRetries;
    u32 length;
    u8 modeConfig;
    u8 modeCurId;
    u8 model;
    u8 buttonDataReady;
    u8 nrOfModes;
    u8 modeCurOffs;
    u8 nrOfActuators;
	u8 numActComb;
	u8 val_c6;
	u8 mode;
    u8 lock;
	u8 actDirSize;
    u8 state;
    u8 reqState;
    u8 currentTask;
	u8 runTask;
	u8 stat70bit;
    u8 padding[11];
} ee_paddata_t;

typedef struct
{
    u32 frame;
    u8 state;
    u8 reqState;
    u8 ok;
    u8 unkn7;
    u8 data[32];
    u32 length;
    u8 request;
    /** CTP=1/no config; CTP=2/config, acts... */
    u8 CTP;
    /** 1, 2 or 3 */
    u8 model;
    /** the data in the buffer is already corrected */
    u8 correction;
    u8 errorCount;
    u8 unk49[15];
} ee_old_paddata_t;

typedef struct
{
	ee_paddata_t ee_pdata __attribute__((aligned(4)));
	ee_old_paddata_t ee_old_pdata __attribute__((aligned(4)));
	u8 modeConfig;
	u8 modeCurId;
	u8 model;
	u8 numModes;
	u8 modeCurOffs;
	u8 numActuators;
	u8 numActComb;
	u8 disconnected;
	union {
		u8 data[4][4];
		u32 data32[4];
	} actData;
	union {
		u8 data[4][4];
		u32 data32[4];
	} combData;
	union {
		u16 data[4];
		u32 data32[2];
	} modeTable;
	u8 buttonInfo[4];
	u8 buttonMask[4];
	u8 modeParam[12];
	u16 val_c4; // unused
	u16 val_c6;
	u8 inbuffer[32];
	u8 outbuffer[32];
	u8 buttonStatus[32];
	u32 buttonDataReady;
	u8 mode;
	u8 lock;
	union {
		u8 data[8];
		u32 data32[8 / sizeof(u32)];
	} ee_actDirectData;
	s16 ee_actDirectSize;
	union {
		u8 data[8];
		u32 data32[8 / sizeof(u32)];
	} ee_actAlignData;
	u16 state;
	u16 reqState;
	u32 frame;
	u32 padarea_ee_addr;
	u32 findPadRetries;
	s32 updatepadTid;
	s32 querypadTid;
	s32 unused158; // probably unused thread id
	s32 setmainmodeTid;
	s32 setactalignTid;
	s32 setbuttoninfoTid;
	s32 setvrefparamTid;
	int eventflag;
	u16 port;
	u16 slot;
	u32 currentTask;
	u32 runTask;
	u32 taskTid;
	u32 stat70bit;
	u32 val_184; // Set, but unused
} padState_t;

// Internal functions
extern void WaitClearEvent(int eventflag, u32 bits, int mode, u32 *resbits_out);
extern int VblankStart(void *arg);
extern int VblankEnd(void *arg);

// Exported/RPC functions
extern s32 padInit(void * ee_addr);
extern s32 padEnd(void);
extern s32 padPortClose(s32 port, s32 slot, s32 wait);
extern s32 padPortOpen(s32 port, s32 slot, s32 pad_area_ee_addr, u32 *buf);
extern u32 padSetMainMode(u32 port, u32 slot, u32 mode, u32 lock);
extern u32 padSetActDirect(u32 port, u32 slot, u8 *actData);
extern u32 padSetActAlign(u32 port, u32 slot, const u8 *actData);
extern u32 padGetButtonMask(u32 port, u32 slot);
extern u32 padSetButtonInfo(u32 port, u32 slot, u32 info);
extern u32 padSetVrefParam(u32 port, u32 slot, const u8 *vparam);
extern s32 padInfoAct(u32 port, u32 slot, s32 act, u32 val);
extern s32 padInfoComb(u32 port, u32 slot, s32 listno, u32 offs);
extern s32 padInfoMode(u32 port, u32 slot, s32 term, u32 offs);
extern u32 padGetPortMax(void);
extern u32 padGetSlotMax(u32 port);
extern u32 padGetModVersion(void);

#endif
