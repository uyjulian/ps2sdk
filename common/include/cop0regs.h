/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

/**
 * @file
 * Definitions for COP0 registers for both EE and IOP.
 * For more information, see the following:
 * * "IDT R30xx Family Software Reference Manual"
 * * "TX System RISC TX79 Core Architecture"
 * * "EE Core User's Manual"
 */

#ifndef __COP0REGS_H__
#define __COP0REGS_H__

register unsigned int __cop0reg_nr00 __asm__ ("c0r0");
register unsigned int __cop0reg_nr01 __asm__ ("c0r1");
register unsigned int __cop0reg_nr02 __asm__ ("c0r2");
register unsigned int __cop0reg_nr03 __asm__ ("c0r3");
register unsigned int __cop0reg_nr04 __asm__ ("c0r4");
register unsigned int __cop0reg_nr05 __asm__ ("c0r5");
register unsigned int __cop0reg_nr06 __asm__ ("c0r6");
register unsigned int __cop0reg_nr07 __asm__ ("c0r7");
register unsigned int __cop0reg_nr08 __asm__ ("c0r8");
register unsigned int __cop0reg_nr09 __asm__ ("c0r9");
register unsigned int __cop0reg_nr10 __asm__ ("c0r10");
register unsigned int __cop0reg_nr11 __asm__ ("c0r11");
register unsigned int __cop0reg_nr12 __asm__ ("c0r12");
register unsigned int __cop0reg_nr13 __asm__ ("c0r13");
register unsigned int __cop0reg_nr14 __asm__ ("c0r14");
register unsigned int __cop0reg_nr15 __asm__ ("c0r15");
register unsigned int __cop0reg_nr16 __asm__ ("c0r16");
register unsigned int __cop0reg_nr17 __asm__ ("c0r17");
register unsigned int __cop0reg_nr18 __asm__ ("c0r18");
register unsigned int __cop0reg_nr19 __asm__ ("c0r19");
register unsigned int __cop0reg_nr20 __asm__ ("c0r20");
register unsigned int __cop0reg_nr21 __asm__ ("c0r21");
register unsigned int __cop0reg_nr22 __asm__ ("c0r22");
register unsigned int __cop0reg_nr23 __asm__ ("c0r23");
register unsigned int __cop0reg_nr24 __asm__ ("c0r24");
register unsigned int __cop0reg_nr25 __asm__ ("c0r25");
register unsigned int __cop0reg_nr26 __asm__ ("c0r26");
register unsigned int __cop0reg_nr27 __asm__ ("c0r27");
register unsigned int __cop0reg_nr28 __asm__ ("c0r28");
register unsigned int __cop0reg_nr29 __asm__ ("c0r29");
register unsigned int __cop0reg_nr30 __asm__ ("c0r30");
register unsigned int __cop0reg_nr31 __asm__ ("c0r31");

/** Bad Virtual Address / Bad virtual address (purpose: Exception) (R) */
#define COP0REG_BadVaddr __cop0reg_nr08
/** Bad Virtual Address / Bad virtual address (purpose: Exception) (R) */
#define COP0REG_BadVAddr __cop0reg_nr08
/** System status register / status register / Processor Status Register (purpose: Exception) (R/W) */
#define COP0REG_SR __cop0reg_nr12
/** System status register / status register / Processor Status Register (purpose: Exception) (R/W) */
#define COP0REG_Status __cop0reg_nr12
/** Describes the most recently recognised exception / exception cause / Cause of the last exception taken (purpose: Exception) (R) */
#define COP0REG_CAUSE __cop0reg_nr13
/** Describes the most recently recognised exception / exception cause / Cause of the last exception taken (purpose: Exception) (R) */
#define COP0REG_Cause __cop0reg_nr13
/** Return Address from Trap / exception pc / Exception Program Counter (purpose: Exception) (R) */
#define COP0REG_EPC __cop0reg_nr14
/** Processor ID / revision identifier / Processor Revision Identifier (purpose: MMU) (R) */
#define COP0REG_PRID __cop0reg_nr15
/** Processor ID / revision identifier / Processor Revision Identifier (purpose: MMU) (R) */
#define COP0REG_PRId __cop0reg_nr15

#ifdef _IOP
/** Breakpoint on execute / breakpoint program counter register (R/W) */
#define COP0REG_BPC __cop0reg_nr03
/** Breakpoint on data access / breakpoint data address register (R/W) */
#define COP0REG_BDA __cop0reg_nr05
/** Randomly memorized jump address / target address register (R) */
#define COP0REG_JUMPDEST __cop0reg_nr06
/** Randomly memorized jump address / target address register (R) */
#define COP0REG_TAR __cop0reg_nr06
/** Breakpoint control / debug and cache invalidate control register (R/W) */
#define COP0REG_DCIC __cop0reg_nr07
/** Data Access breakpoint mask / breakpoint data address mask register (R/W) */
#define COP0REG_BDAM __cop0reg_nr09
/** Execute breakpoint mask / breakpoint program counter mask register (R/W) */
#define COP0REG_BPCM __cop0reg_nr11
#endif

// FIXME: ee_cop0_defs.h and TX79 Core Architecture manual disagree about the definitions of PageMask and Wired

#ifdef _EE
/** Programmable register to select TLB entry for reading or writing (purpose: MMU) */
#define COP0REG_Index __cop0reg_nr00
/** Pseudo-random counter for TLB replacement (purpose: MMU) */
#define COP0REG_Random __cop0reg_nr01
/** Low half of TLB entry for even PFN (Physical page number) (purpose: MMU) */
#define COP0REG_EntryLo0 __cop0reg_nr02
/** Low half of TLB entry for odd PFN (Physical page number) (purpose: MMU) */
#define COP0REG_EntryLo1 __cop0reg_nr03
/** Pointer to kernel virtual PTE table (purpose: Exception) */
#define COP0REG_Context __cop0reg_nr04
/** Mask that sets the TLB page size (purpose: MMU) */
#define COP0REG_PageMask __cop0reg_nr05
/** Number of wired TLB entries (purpose: MMU) */
#define COP0REG_Wired __cop0reg_nr06
/** Timer compare (purpose: Exception) */
#define COP0REG_Count __cop0reg_nr09
/** High half of TLB entry(Virtual page number and ASID) (purpose: MMU) */
#define COP0REG_EntryHi __cop0reg_nr10
/** Timer compare (purpose: Exception) */
#define COP0REG_Compare __cop0reg_nr11
/** Configuration Register (purpose: MMU) */
#define COP0REG_Config  __cop0reg_nr16
/** Bad Physical Address (purpose: Exception) */
#define COP0REG_BadPAddr __cop0reg_nr23
/** This is used for Debug function (purpose: Debug) */
#define COP0REG_Debug __cop0reg_nr24
/** Performance Counter and Control Register (purpose: Exception) */
#define COP0REG_Perf __cop0reg_nr25
/** Cache Tag register(low bits) (purpose: MMU) */
#define COP0REG_TagLo __cop0reg_nr28
/** Cache Tag register(high bits) (purpose: MMU) */
#define COP0REG_TagHi __cop0reg_nr29
/** Error Exception Program Counter (purpose: Exception) */
#define COP0REG_ErrorPC __cop0reg_nr30
/** Error Exception Program Counter (purpose: Exception) */
#define COP0REG_ErrorEPC __cop0reg_nr30
#endif

#endif /* __COP0REGS_H__ */
