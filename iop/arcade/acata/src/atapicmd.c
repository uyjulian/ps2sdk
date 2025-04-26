/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

#include "acata_internal.h"

static int atapi_ops_command(struct ac_ata_h *atah, int cmdpri, int pri);
static void atapi_ops_done(struct ac_ata_h *atah, int result);

static struct ac_ata_ops ops_48 = {&atapi_ops_command, &atapi_ops_done, NULL};

typedef struct scsi_mode_data_
{
	u8 m_retry_pg01[20];
	u8 m_timer_pg0d[16];
	u8 m_speed_pg2a[28];
} scsi_mode_data_t;

// TODO: do we actually need to intialize this? for medium, retry, speed, and timer
static scsi_mode_data_t g_scsi_mode_data;

static int atapi_ops_command(struct ac_ata_h *atah, int cmdpri, int pri)
{
	acAtapiT atapi;

	atapi = (acAtapiT)atah;
	// DMA flag: atah->a_flag & 1
	// Wait for interrupt w/ SleepThread flag: atah->a_flag & 2
	// Write flag: atah->a_flag & 4
	// DMA ready stuff removed
	ChangeThreadPriority(0, cmdpri);
	if ( atah->a_state < 0x1FFu )
	{
		atah->a_state = 31;
	}
	// DMA handling omitted
	ChangeThreadPriority(0, pri);
	switch(atapi->ap_packet.u_b[0])
	{
	case 0x00: // TEST UNIT READY: ready
		// Just do nothing here
		break;
	case 0x1b: // START/STOP UNIT: start stop
		// TODO: do we actually need to handle this? for tray open/close
		break;
	case 0x25: // READ CAPACITY (10): read capacity
		{
			u8 *datau8;
			u32 maxlba;
			u32 blocklen;

			if ( !atah->a_buf || atah->a_size < 8 )
				return -1;
			// FIXME: set the max LBA value (in 2048 byte units) here correctly
			maxlba = 0xFFFFFFFF;
			blocklen = 2048;
			datau8 = (u8 *)(atah->a_buf);
			memset(datau8, 0, 8);
			datau8[0] = (maxlba >> 24) & 0xFF;
			datau8[1] = (maxlba >> 16) & 0xFF;
			datau8[2] = (maxlba >> 8) & 0xFF;
			datau8[3] = (maxlba) & 0xFF;
			datau8[4] = (blocklen >> 24) & 0xFF;
			datau8[5] = (blocklen >> 16) & 0xFF;
			datau8[6] = (blocklen >> 8) & 0xFF;
			datau8[7] = (blocklen) & 0xFF;
		}
		break;
	case 0x28: // READ (10): read / seek / seek+start
		{
			u32 readlba;
			u32 readlen;

			readlba = (atapi->ap_packet.u_b[2] << 24) | (atapi->ap_packet.u_b[3] << 16) | (atapi->ap_packet.u_b[4] << 8) | (atapi->ap_packet.u_b[5]);
			readlen = (atapi->ap_packet.u_b[7] << 8) | (atapi->ap_packet.u_b[8]);
			if ( !atah->a_size )
				return 0;
			if ( !atah->a_buf )
				return -1;
			if ( (atah->a_size >> 11) != readlen )
				return -1;
			// FIXME: READ to atah->a_buf
			(void)readlba;
			// lba (2048 byte sectors) = readlba
			// count (2048 byte sectors) = readlen
		}
		break;
	case 0x43: // READ TOC/PMA/ATIP: (mmc commands) read toc
		{
			u16 *datau16;

			if ( (atah->a_flag & 4) )
				return -1;
			if ( !atah->a_buf || atah->a_size < 1024 )
				return -1;
			// TODO: Do we need this? For now, just clear the data
			datau16 = (u16 *)(atah->a_buf);
			memset(datau16, 0, 1024);
		}
		break;
	case 0x55: // MODE SELECT (10): mode select
		{
			u16 datalen;
			u16 outdatalen;
			u8 *outdataptr;

			if ( !(atah->a_flag & 4) )
				return -1;
			// check PF bit is set
			if ( !(atapi->ap_packet.u_b[2] & 0x10) )
				return -1;
			datalen = (atapi->ap_packet.u_b[7] << 8) | (atapi->ap_packet.u_b[8]);
			if ( !atah->a_buf || atah->a_size != datalen || datalen < 10 )
				return -1;
			outdatalen = 0;
			outdataptr = NULL;
			switch(((u8 *)(atah->a_buf))[8])
			{
			case 0x01:
				outdatalen = sizeof(g_scsi_mode_data.m_retry_pg01);
				outdataptr = g_scsi_mode_data.m_retry_pg01;
				break;
			case 0x0d:
				outdatalen = sizeof(g_scsi_mode_data.m_timer_pg0d);
				outdataptr = g_scsi_mode_data.m_timer_pg0d;
				break;
			case 0x2a:
				outdatalen = sizeof(g_scsi_mode_data.m_speed_pg2a);
				outdataptr = g_scsi_mode_data.m_speed_pg2a;
				break;
			default:
				break;
			}
			if ( datalen < outdatalen )
				return -1;
			if (outdataptr && outdatalen)
			{
				memcpy(outdataptr, atah->a_buf, outdatalen);				
			}
		}
		break;
	case 0x5a: // MODE SENSE (10): mode sense
		{
			u16 datalen;
			u16 outdatalen;
			u8 *outdataptr;

			if ( (atah->a_flag & 4) )
				return -1;
			datalen = (atapi->ap_packet.u_b[7] << 8) | (atapi->ap_packet.u_b[8]);
			outdatalen = 0;
			outdataptr = NULL;
			switch(atapi->ap_packet.u_b[2])
			{
			case 0x01:
				outdatalen = sizeof(g_scsi_mode_data.m_retry_pg01);
				outdataptr = g_scsi_mode_data.m_retry_pg01;
				break;
			case 0x0d:
				outdatalen = sizeof(g_scsi_mode_data.m_timer_pg0d);
				outdataptr = g_scsi_mode_data.m_timer_pg0d;
				break;
			case 0x2a:
				outdatalen = sizeof(g_scsi_mode_data.m_speed_pg2a);
				outdataptr = g_scsi_mode_data.m_speed_pg2a;
				break;
			default:
				break;
			}
			if ( !atah->a_buf || atah->a_size != datalen || datalen < outdatalen )
				return -1;
			if (outdataptr && outdatalen)
			{
				outdataptr[0] = ((outdatalen - 2) >> 8) & 0xFF;
				outdataptr[1] = (outdatalen - 2) & 0xFF;
				outdataptr[8] = atapi->ap_packet.u_b[2];
				memcpy(atah->a_buf, outdataptr, outdatalen);
			}
		}
		break;
	case 0xbb: // SET CD SPEED: (mmc commands) set speed
		{
			if ( (atah->a_flag & 4) )
				return -1;
			g_scsi_mode_data.m_speed_pg2a[21] = atapi->ap_packet.u_b[2];
			g_scsi_mode_data.m_speed_pg2a[22] = atapi->ap_packet.u_b[3];
		}
		break;
	default:
		break;
	}
	// Timeout checking removed
	// Wait for IO finish removed
	// MMIO checking removed here
	if ( atah->a_state >= 0x1FFu )
	{
		return -116;
	}
	atah->a_state = 127;
	return 0;
}

static void atapi_ops_done(struct ac_ata_h *atah, int result)
{
	acAtapiT atapi;

	atapi = (acAtapiT)atah;
	if ( atapi->ap_done )
		atapi->ap_done(atapi, atah->a_arg, result);
}

// Error handling omitted

acAtapiT acAtapiSetup(acAtapiData *atapi, acAtapiDone done, void *arg, unsigned int tmout)
{
	if ( atapi )
	{
		atapi->ap_h.a_ops = &ops_48;
		atapi->ap_h.a_arg = arg;
		atapi->ap_h.a_tmout = tmout;
		atapi->ap_h.a_state = 0;
		atapi->ap_h.a_flag = 0;
		atapi->ap_done = done;
	}
	return atapi;
}

int acAtapiRequest(acAtapiT atapi, int flag, acAtapiPacketData *pkt, void *buf, int size)
{
	flag = flag | 8;
	if ( !atapi || !pkt )
	{
		return -22;
	}
	atapi->ap_h.a_flag = flag;
	atapi->ap_packet.u_w[0] = pkt->u_w[0];
	atapi->ap_packet.u_w[1] = pkt->u_w[1];
	atapi->ap_packet.u_w[2] = pkt->u_w[2];
	atapi->ap_h.a_buf = buf;
	atapi->ap_h.a_size = size;
	return ata_request(&atapi->ap_h, WakeupThread);
}

int acAtapiRequestI(acAtapiT atapi, int flag, acAtapiPacketData *pkt, void *buf, int size)
{
	flag = flag | 8;
	if ( !atapi || !pkt )
	{
		return -22;
	}
	atapi->ap_h.a_flag = flag;
	atapi->ap_packet.u_w[0] = pkt->u_w[0];
	atapi->ap_packet.u_w[1] = pkt->u_w[1];
	atapi->ap_packet.u_w[2] = pkt->u_w[2];
	atapi->ap_h.a_buf = buf;
	atapi->ap_h.a_size = size;
	return ata_request(&atapi->ap_h, iWakeupThread);
}

int acAtapiStatus(acAtapiT atapi)
{
	int state;

	if ( atapi == 0 )
	{
		return -22;
	}
	state = atapi->ap_h.a_state;
	if ( (unsigned int)(state - 1) >= 0x7E )
	{
		return 0;
	}
	if ( state != 1 )
		return 2;
	return 1;
}
