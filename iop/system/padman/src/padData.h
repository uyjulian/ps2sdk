/*
 * Copyright (c) 2007 Lukasz Bruun <mail@lukasz.dk>
 *
 * See the file LICENSE included with this distribution for licensing terms.
 */

/**
 * @file
 * IOP pad driver
 */

#ifndef __FREEPAD_PADDATA_H__
#define __FREEPAD_PADDATA_H__


extern void pdReset(void);
extern void pdTransfer(void);

extern u32 pdSetCtrl1(u32 port, u32 slot, u32 ctrl);
extern u32 pdSetCtrl2(u32 port, u32 slot, u32 ctrl);

extern u32 pdSetInBuffer(u32 port, u32 slot, u32 size, const u8 *buf);
extern u32 pdGetOutBuffer(u32 port, u32 slot, u32 size, u8 *buf);

extern u32 pdSetInSize(u32 port, u32 slot, u32 size);
extern u32 pdSetOutSize(u32 port, u32 slot, u32 size);

extern u32 pdGetInSize(u8 id);
extern u32 pdGetOutSize(u8 id);

extern u32 pdSetRegData(u32 port, u32 slot, u32 reg_data);
extern u32 pdGetRegData(u32 id);

extern u32 pdIsActive(u32 port, u32 slot);
extern u32 pdSetActive(u32 port, u32 slot, u32 active);

extern u32 pdGetStat70bit(u32 port, u32 slot);
extern u32 pdSetStat70bit(u32 port, u32 slot, u32 val);

extern s32 pdGetError(u32 port, u32 slot);

extern u32 pdCheckConnection(u32 port, u32 slot);




#endif

