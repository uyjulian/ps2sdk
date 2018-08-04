/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright (c) 2003 Marcus R. Brown <mrbrown@0xd6.org>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

/**
 * @file
 * IOPkit standard definitions.
 */

#ifndef __DEFS_H__
#define __DEFS_H__

#include <types.h>

#ifndef NULL
#define NULL	((void *)0)
#endif

#define ALIGN(x, align)	(((x)+((align)-1))&~((align)-1))

#define PHYSADDR(a)	(((u32)(a)) & 0x1fffffff)

#define KSEG1		0xa0000000
#define KSEG1ADDR(a)	((__typeof__(a))(((u32)(a) & 0x1fffffff) | KSEG1))

static __inline__ void *ChangeGP(void *gp)
{
	// void *OldGP;

	// __asm__ volatile(	"move %0, $gp\n"
	// 			"move $gp, %1"
	// 			: "=&r"(OldGP)
	// 			: "r"(gp)
	// 			: "gp", "memory");

	return gp;
}

static __inline__ void SetGP(void *gp)
{
	// __asm__ volatile(	"move $gp, %0"
	// 			:
	// 			: "r"(gp)
	// 			: "gp", "memory");
}

extern void *_gp;
#define SetModuleGP()	ChangeGP(&_gp)

static __inline__ void *GetGP(void)
{
	// void *gp;

	// __asm__ volatile(	"move %0, $gp"
	// 			: "=r"(gp)
	// 			:
	// 			: "memory");

	return NULL;
}

static inline void *iop_memcpy(void *dest, const void *src, int size)
{
	u8 *d = (u8 *)dest, *s = (u8 *)src;

	while (size--)
		*d++ = *s++;
	return dest;
}

#endif /* __DEFS_H__ */
