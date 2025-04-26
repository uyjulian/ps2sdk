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
#include <atahw.h>

static int ata_ops_command(struct ac_ata_h *atah, int cmdpri, int pri);
static void ata_ops_done(struct ac_ata_h *atah, int result);

static struct ac_ata_ops ops_37 = {&ata_ops_command, &ata_ops_done, NULL};

static int ata_ops_command(struct ac_ata_h *atah, int cmdpri, int pri)
{
	int count;
	acAtaT ata;
	u8 regcontents[16];

	ata = (acAtaT)atah;
	if ( atah->a_state >= 0x1FFu )
	{
		return -116;
	}
	atah->a_state = 7;
	// DMA flag: atah->a_flag & 1
	// Wait for interrupt w/ SleepThread flag: atah->a_flag & 2
	// Write flag: atah->a_flag & 4
	// ??? flag: atah->a_flag & 0x10
	// DMA ready stuff removed
	ChangeThreadPriority(0, cmdpri);
	// MMIO setting removed here
	for ( count = 0; count < 6; count += 1 )
	{
		int xib;
		xib = ((ata->ac_command[count] >> 8) & 0xF);
		regcontents[xib] = ata->ac_command[count] & 0xFF;
		if ( xib == 7 )
			break;
	}
	ChangeThreadPriority(0, pri);
	// cppcheck-suppress knownConditionTrueFalse
	if ( atah->a_state >= 0x1FFu )
	{
		return -116;
	}
	atah->a_state = 31;
	switch(regcontents[7])
	{
	case ATA_C_IDENTIFY_PACKET_DEVICE: // accdvd
		{
			u16 *datau16;
			if ( (atah->a_flag & 4) )
				return -1;
			if ( !atah->a_buf || atah->a_size < 512 )
				return -1;
			datau16 = (u16 *)(atah->a_buf);
			memset(datau16, 0, 512);
			// General configuration bit significant info
			// ATAPI device bit set, command packet set field set
			datau16[0] = 0x8500;
			// Ultra and multi word DMA capabilities support. Just enable everything
			datau16[62] = 0xFF;
			// Multi word DMA capabilities support. Just enable everything
			datau16[63] = 0x07;
			// Ultra DMA capabilities support. Just enable everything
			datau16[88] = 0x7f;
		}
	case ATA_C_CHECK_POWER_MODE: // accdvd
		{
			if ( (atah->a_flag & 4) )
				return -1;
			if ( atah->a_buf || atah->a_size )
				return -1;
			// Count register: Device is in Active mode or Idle mode.
			regcontents[2] = 0xFF;
		}
		break;
	case ATA_C_IDENTIFY_DEVICE: // acatad
		{
			u16 *datau16;
			u32 maxlba;
			if ( (atah->a_flag & 4) )
				return -1;
			if ( !atah->a_buf || atah->a_size < 512 )
				return -1;
			datau16 = (u16 *)(atah->a_buf);
			memset(datau16, 0, 512);
			// Device name from 16 bit words 27-47 are only used for output
			// FIXME: Set the total sectors (in 512 byte units) here
			maxlba = 0xFFFFFFFF;
			// Total sectors hi
			datau16[60] = (maxlba) & 0xFFFF;
			// Total sectors lo
			datau16[61] = (maxlba >> 16) & 0xFFFF;
		}
		break;
	case ATA_C_SET_FEATURES: // acatad, accdvd
		// Just do nothing here
		break;
	case ATA_C_READ_DMA_WITHOUT_RETRIES: // acatad
		{
			u32 readlba;
			u32 readlen;

			readlba = regcontents[3] | (regcontents[4] << 8) | (regcontents[5] << 16) | ((regcontents[6] & 0xF) << 24);
			readlen = regcontents[2];
			if ( (atah->a_flag & 4) )
				return -1;
			if ( !atah->a_size )
				return 0;
			if ( !atah->a_buf || (atah->a_size >> 9) != readlen || !(regcontents[6] & 0x40) )
				return -1;
			// FIXME: READ to atah->a_buf
			(void)readlba;
			// lba (512 byte sectors) = readlba
			// count (512 byte sectors) = readlen
		}
		break;
	case ATA_C_WRITE_DMA: // acatad
		{
			u32 writelba;
			u32 writelen;

			writelba = regcontents[3] | (regcontents[4] << 8) | (regcontents[5] << 16) | ((regcontents[6] & 0xF) << 24);
			writelen = regcontents[2];
			if ( !(atah->a_flag & 4) )
				return -1;
			if ( !atah->a_size )
				return 0;
			if ( !atah->a_buf || (atah->a_size >> 9) != writelen || !(regcontents[6] & 0x40) )
				return -1;
			// FIXME: WRITE from atah->a_buf
			(void)writelba;
			// lba (512 byte sectors) = writelba
			// count (512 byte sectors) = writelen
		}
		break;
	case ATA_C_FLUSH_CACHE: // acatad
		// Just do nothing here
		break;
	default:
		break;
	}
	// PIO/DMA stuff removed
	// Wait for IO finish removed
	// MMIO checking removed here
	if ( atah->a_state >= 0x1FFu )
	{
		return -116;
	}
	atah->a_state = 127;
	for ( count = 0; count < 6; count += 1 )
	{
		int xir;
		xir = (((int)(ata->ac_command[count]) >> 12) & 0xF);
		if ( xir == 0 )
			break;
		ata->ac_command[count] = (xir << 12) | regcontents[xir];
	}
	return 6 - count;
}

static void ata_ops_done(struct ac_ata_h *atah, int result)
{
	acAtaT ata;

	ata = (acAtaT)atah;
	if ( ata->ac_done )
		ata->ac_done(ata, atah->a_arg, result);
}

acAtaT acAtaSetup(acAtaData *ata, acAtaDone done, void *arg, unsigned int tmout)
{
	if ( ata )
	{
		ata->ac_h.a_ops = &ops_37;
		ata->ac_h.a_arg = arg;
		ata->ac_h.a_tmout = tmout;
		ata->ac_h.a_state = 0;
		ata->ac_h.a_flag = 0;
		ata->ac_done = done;
	}
	return ata;
}

acAtaCommandData *acAtaReply(acAtaT ata)
{
	return ata ? ata->ac_command : 0;
}

int acAtaRequest(acAtaT ata, int flag, acAtaCommandData *cmd, int item, void *buf, int size)
{
	int v7;

	flag = (flag | 8) ^ 8;
	if ( !ata || !cmd )
	{
		return -22;
	}
	if ( (unsigned int)item >= 7 )
	{
		return -34;
	}
	v7 = item - 1;
	ata->ac_h.a_flag = flag;
	while ( v7 >= 0 )
	{
		ata->ac_command[v7] = cmd[v7];
		--v7;
	}
	ata->ac_h.a_buf = buf;
	ata->ac_h.a_size = size;
	return ata_request(&ata->ac_h, WakeupThread);
}

int acAtaRequestI(acAtaT ata, int flag, acAtaCommandData *cmd, int item, void *buf, int size)
{
	int v7;

	flag = (flag | 8) ^ 8;
	if ( !ata || !cmd )
	{
		return -22;
	}
	if ( (unsigned int)item >= 7 )
	{
		return -34;
	}
	v7 = item - 1;
	ata->ac_h.a_flag = flag;
	while ( v7 >= 0 )
	{
		ata->ac_command[v7] = cmd[v7];
		--v7;
	}
	ata->ac_h.a_buf = buf;
	ata->ac_h.a_size = size;
	return ata_request(&ata->ac_h, iWakeupThread);
}

int acAtaStatus(acAtaT ata)
{
	int state;

	if ( ata == 0 )
	{
		return -22;
	}
	state = ata->ac_h.a_state;
	if ( (unsigned int)(state - 1) >= 0x7E )
	{
		return 0;
	}
	if ( state != 1 )
		return 2;
	return 1;
}
