/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright (c) 2006-2007 Eugene Plotnikov <e-plotnikov@operamail.com>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
# Based on refernce software of MSSG
*/

#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <kernel.h>

#include "libmpeg.h"
#include "libmpeg_internal.h"

#if 0
u8 s_DMAPack[128];
u8 s_DataBuf[8];
u8 s_SetDMA[8];
u8 s_IPUState[32];
u8 s_pEOF[4];
u8 s_Sema[4];
u8 s_CSCParam[12];
u8 s_CSCID[4];
u8 s_CSCFlag[1];

u32 s_DefQM[20] = {
	0x13101008, 0x16161310, 0x16161616, 0x1B1A181A,
	0x1A1A1B1B, 0x1B1B1A1A, 0x1D1D1D1B, 0x1D222222,
	0x1B1B1D1D, 0x20201D1D, 0x26252222, 0x22232325,
	0x28262623, 0x30302828, 0x38382E2E, 0x5345453A,
	0x10101010, 0x10101010, 0x10101010, 0x10101010,
};

void _MPEG_Initialize ( _MPEGContext* arg0, int ( * arg1) ( void* ), void* arg2, int* arg3)
{

}

void _MPEG_Destroy ( void )
{

}

void _ipu_suspend ( void )
{
	
}

void _MPEG_Suspend ( void )
{
	return _ipu_suspend();
}

void _ipu_resume ( void )
{
	
}

void _MPEG_Resume ( void )
{
	return _ipu_resume();
}

void _mpeg_dmac_handler( void )
{

}

int _MPEG_CSCImage ( void* arg0, void* arg1, int arg2 )
{

}

void _ipu_sync( void )
{

}

void _ipu_sync_data( void )
{

}

unsigned int _ipu_get_bits( unsigned int arg0 )
{

}

unsigned int _MPEG_GetBits ( unsigned int arg0 )
{
	return _ipu_get_bits(arg0);
}

unsigned int _ipu_show_bits ( unsigned int arg0 )
{

}

unsigned int _MPEG_ShowBits ( unsigned int arg0 )
{
	return _ipu_show_bits(arg0);
}

void _ipu_align_bits( void )
{

}

void _MPEG_AlignBits ( void )
{
	return _ipu_align_bits();
}

unsigned int _MPEG_NextStartCode ( void )
{

}

void _MPEG_SetDefQM ( int arg0 )
{

}

void _MPEG_SetQM ( int arg0 )
{

}

int _MPEG_GetMBAI ( void )
{

}

int _MPEG_GetMBType ( void )
{

}

int _MPEG_GetMotionCode ( void )
{

}

int _MPEG_GetDMVector ( void )
{

}

void _MPEG_SetIDCP ( void )
{

}

void _MPEG_SetQSTIVFAS ( void )
{

}

void _MPEG_SetPCT ( unsigned int arg0 )
{

}

void _MPEG_BDEC ( int arg0, int arg1, int arg2, int arg3, void* arg4 )
{

}

int _MPEG_WaitBDEC ( void )
{

}

void _MPEG_dma_ref_image ( _MPEGMacroBlock8* arg0, _MPEGMotion* arg1, int arg2, int arg3 )
{

}
#endif

void _MPEG_put_block_fr ( _MPEGMotions* arg0 )
{
	u128 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10;
	asm __volatile__(
		"lw      %[tmp9], 0(%[arg0])\n"
		"lw      %[tmp10], 8(%[arg0])\n"
		"pnor    $v0, $zero, $zero\n"
		"addiu   $v1, $zero, 6\n"
		"psrlh   $v0, $v0, 8\n"
	"1:\n"
		"lq      %[tmp0],   0(%[tmp10])\n"
		"lq      %[tmp1],  16(%[tmp10])\n"
		"lq      %[tmp2],  32(%[tmp10])\n"
		"lq      %[tmp3],  48(%[tmp10])\n"
		"addiu   $v1, $v1, -1;\n"
		"lq      %[tmp4],  64(%[tmp10])\n"
		"lq      %[tmp5],  80(%[tmp10])\n"
		"lq      %[tmp6],  96(%[tmp10])\n"
		"lq      %[tmp7], 112(%[tmp10])\n"
		"addiu   %[tmp10], %[tmp10], 128\n"
		"pmaxh   %[tmp0], $zero, %[tmp0]\n"
		"pmaxh   %[tmp1], $zero, %[tmp1]\n"
		"pmaxh   %[tmp2], $zero, %[tmp2]\n"
		"pmaxh   %[tmp3], $zero, %[tmp3]\n"
		"pmaxh   %[tmp4], $zero, %[tmp4]\n"
		"pmaxh   %[tmp5], $zero, %[tmp5]\n"
		"pmaxh   %[tmp6], $zero, %[tmp6]\n"
		"pmaxh   %[tmp7], $zero, %[tmp7]\n"
		"pminh   %[tmp0], $v0, %[tmp0]\n"
		"pminh   %[tmp1], $v0, %[tmp1]\n"
		"pminh   %[tmp2], $v0, %[tmp2]\n"
		"pminh   %[tmp3], $v0, %[tmp3]\n"
		"pminh   %[tmp4], $v0, %[tmp4]\n"
		"pminh   %[tmp5], $v0, %[tmp5]\n"
		"pminh   %[tmp6], $v0, %[tmp6]\n"
		"pminh   %[tmp7], $v0, %[tmp7]\n"
		"ppacb   %[tmp0], %[tmp1], %[tmp0]\n"
		"ppacb   %[tmp2], %[tmp3], %[tmp2]\n"
		"ppacb   %[tmp4], %[tmp5], %[tmp4]\n"
		"ppacb   %[tmp6], %[tmp7], %[tmp6]\n"
		"sq      %[tmp0],  0(%[tmp9])\n"
		"sq      %[tmp2], 16(%[tmp9])\n"
		"sq      %[tmp4], 32(%[tmp9])\n"
		"sq      %[tmp6], 48(%[tmp9])\n"
		"bgtzl   $v1, 1b\n"
		"addiu   %[tmp9], %[tmp9], 64\n"
	: [tmp0] "=r"(tmp0), [tmp1] "=r"(tmp1), [tmp2] "=r"(tmp2), [tmp3] "=r"(tmp3), [tmp4] "=r"(tmp4), [tmp5] "=r"(tmp5), [tmp6] "=r"(tmp6), [tmp7] "=r"(tmp7), [tmp8] "=r"(tmp8), [tmp9] "=r"(tmp9), [tmp10] "=r"(tmp10)
	: [arg0] "r"(arg0)
	: "a1", "v0", "v1", "memory"
	);
}

void _MPEG_put_block_fl ( _MPEGMotions* arg0 )
{
	u128 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10;
	asm __volatile__(
		"pnor    $v0, $zero, $zero\n"
		"lw      %[tmp9], 0(%[arg0])\n"
		"lw      %[tmp10], 8(%[arg0])\n"
		"addiu   $v1, $zero, 4\n"
		"psrlh   $v0, $v0, 8\n"
	"1:\n"
		"lq      %[tmp0],   0(%[tmp10])\n"
		"lq      %[tmp1],  16(%[tmp10])\n"
		"lq      %[tmp2],  32(%[tmp10])\n"
		"lq      %[tmp3],  48(%[tmp10])\n"
		"addiu   $v1, $v1, -1\n"
		"lq      %[tmp4], 256(%[tmp10])\n"
		"lq      %[tmp5], 272(%[tmp10])\n"
		"lq      %[tmp6], 288(%[tmp10])\n"
		"lq      %[tmp7], 304(%[tmp10])\n"
		"addiu   %[tmp10], %[tmp10], 64\n"
		"pmaxh   %[tmp0], $zero, %[tmp0]\n"
		"pmaxh   %[tmp1], $zero, %[tmp1]\n"
		"pmaxh   %[tmp2], $zero, %[tmp2]\n"
		"pmaxh   %[tmp3], $zero, %[tmp3]\n"
		"pmaxh   %[tmp4], $zero, %[tmp4]\n"
		"pmaxh   %[tmp5], $zero, %[tmp5]\n"
		"pmaxh   %[tmp6], $zero, %[tmp6]\n"
		"pmaxh   %[tmp7], $zero, %[tmp7]\n"
		"pminh   %[tmp0], $v0, %[tmp0]\n"
		"pminh   %[tmp1], $v0, %[tmp1]\n"
		"pminh   %[tmp2], $v0, %[tmp2]\n"
		"pminh   %[tmp3], $v0, %[tmp3]\n"
		"pminh   %[tmp4], $v0, %[tmp4]\n"
		"pminh   %[tmp5], $v0, %[tmp5]\n"
		"pminh   %[tmp6], $v0, %[tmp6]\n"
		"pminh   %[tmp7], $v0, %[tmp7]\n"
		"ppacb   %[tmp0], %[tmp1], %[tmp0]\n"
		"ppacb   %[tmp2], %[tmp3], %[tmp2]\n"
		"ppacb   %[tmp4], %[tmp5], %[tmp4]\n"
		"ppacb   %[tmp6], %[tmp7], %[tmp6]\n"
		"sq      %[tmp0],  0(%[tmp9])\n"
		"sq      %[tmp4], 16(%[tmp9])\n"
		"sq      %[tmp2], 32(%[tmp9])\n"
		"sq      %[tmp6], 48(%[tmp9])\n"
		"bgtz    $v1, 1b\n"
		"addiu   %[tmp9], %[tmp9], 64\n"
		"addiu   $v1, $v1, 2\n"
	"2:\n"
		"lq      %[tmp0], 256(%[tmp10])\n"
		"lq      %[tmp1], 272(%[tmp10])\n"
		"lq      %[tmp2], 288(%[tmp10])\n"
		"lq      %[tmp3], 304(%[tmp10])\n"
		"addiu   $v1, $v1, -1\n"
		"lq      %[tmp4], 320(%[tmp10])\n"
		"lq      %[tmp5], 336(%[tmp10])\n"
		"lq      %[tmp6], 352(%[tmp10])\n"
		"lq      %[tmp7], 368(%[tmp10])\n"
		"addiu   %[tmp10], %[tmp10], 128\n"
		"pmaxh   %[tmp0], $zero, %[tmp0]\n"
		"pmaxh   %[tmp1], $zero, %[tmp1]\n"
		"pmaxh   %[tmp2], $zero, %[tmp2]\n"
		"pmaxh   %[tmp3], $zero, %[tmp3]\n"
		"pmaxh   %[tmp4], $zero, %[tmp4]\n"
		"pmaxh   %[tmp5], $zero, %[tmp5]\n"
		"pmaxh   %[tmp6], $zero, %[tmp6]\n"
		"pmaxh   %[tmp7], $zero, %[tmp7]\n"
		"pminh   %[tmp0], $v0, %[tmp0]\n"
		"pminh   %[tmp1], $v0, %[tmp1]\n"
		"pminh   %[tmp2], $v0, %[tmp2]\n"
		"pminh   %[tmp3], $v0, %[tmp3]\n"
		"pminh   %[tmp4], $v0, %[tmp4]\n"
		"pminh   %[tmp5], $v0, %[tmp5]\n"
		"pminh   %[tmp6], $v0, %[tmp6]\n"
		"pminh   %[tmp7], $v0, %[tmp7]\n"
		"ppacb   %[tmp0], %[tmp1], %[tmp0]\n"
		"ppacb   %[tmp2], %[tmp3], %[tmp2]\n"
		"ppacb   %[tmp4], %[tmp5], %[tmp4]\n"
		"ppacb   %[tmp6], %[tmp7], %[tmp6]\n"
		"sq      %[tmp0],  0(%[tmp9])\n"
		"sq      %[tmp2], 16(%[tmp9])\n"
		"sq      %[tmp4], 32(%[tmp9])\n"
		"sq      %[tmp6], 48(%[tmp9])\n"
		"bgtzl   $v1, 2b\n"
		"addiu   %[tmp9], %[tmp9], 64\n"
	: [tmp0] "=r"(tmp0), [tmp1] "=r"(tmp1), [tmp2] "=r"(tmp2), [tmp3] "=r"(tmp3), [tmp4] "=r"(tmp4), [tmp5] "=r"(tmp5), [tmp6] "=r"(tmp6), [tmp7] "=r"(tmp7), [tmp8] "=r"(tmp8), [tmp9] "=r"(tmp9), [tmp10] "=r"(tmp10)
	: [arg0] "r"(arg0)
	: "a1", "v0", "v1", "memory"
	);
}

void _MPEG_put_block_il ( _MPEGMotions* arg0 )
{
	u128 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10;
	asm __volatile__(
		"pnor    $v0, $zero, $zero\n"
		"lw      %[tmp9],  0(%[arg0])\n"
		"lw      %[tmp10],  8(%[arg0])\n"
		"lw      %[tmp8], 24(%[arg0])\n"
		"addiu   $v1, $zero, 4\n"
		"psrlh   $v0, $v0, 8\n"
		"addu    %[tmp8], %[tmp8], %[tmp9]\n"
	"1:\n"
		"lq      %[tmp0],   0(%[tmp10])\n"
		"lq      %[tmp1],  16(%[tmp10])\n"
		"lq      %[tmp2],  32(%[tmp10])\n"
		"lq      %[tmp3],  48(%[tmp10])\n"
		"addiu   $v1, $v1, -1\n"
		"lq      %[tmp4], 256(%[tmp10])\n"
		"lq      %[tmp5], 272(%[tmp10])\n"
		"lq      %[tmp6], 288(%[tmp10])\n"
		"lq      %[tmp7], 304(%[tmp10])\n"
		"addiu   %[tmp10], %[tmp10], 64\n"
		"pmaxh   %[tmp0], $zero, %[tmp0]\n"
		"pmaxh   %[tmp1], $zero, %[tmp1]\n"
		"pmaxh   %[tmp2], $zero, %[tmp2]\n"
		"pmaxh   %[tmp3], $zero, %[tmp3]\n"
		"pmaxh   %[tmp4], $zero, %[tmp4]\n"
		"pmaxh   %[tmp5], $zero, %[tmp5]\n"
		"pmaxh   %[tmp6], $zero, %[tmp6]\n"
		"pmaxh   %[tmp7], $zero, %[tmp7]\n"
		"pminh   %[tmp0], $v0, %[tmp0]\n"
		"pminh   %[tmp1], $v0, %[tmp1]\n"
		"pminh   %[tmp2], $v0, %[tmp2]\n"
		"pminh   %[tmp3], $v0, %[tmp3]\n"
		"pminh   %[tmp4], $v0, %[tmp4]\n"
		"pminh   %[tmp5], $v0, %[tmp5]\n"
		"pminh   %[tmp6], $v0, %[tmp6]\n"
		"pminh   %[tmp7], $v0, %[tmp7]\n"
		"ppacb   %[tmp0], %[tmp1], %[tmp0]\n"
		"ppacb   %[tmp2], %[tmp3], %[tmp2]\n"
		"ppacb   %[tmp4], %[tmp5], %[tmp4]\n"
		"ppacb   %[tmp6], %[tmp7], %[tmp6]\n"
		"sq      %[tmp0],  0(%[tmp9])\n"
		"sq      %[tmp2], 32(%[tmp9])\n"
		"addiu   %[tmp9], %[tmp9], 64\n"
		"sq      %[tmp4],  0(%[tmp8])\n"
		"sq      %[tmp6], 32(%[tmp8])\n"
		"bgtzl   $v1, 1b\n"
		"addiu   %[tmp8], %[tmp8], 64\n"
		"lw      %[tmp9],  4(%[arg0])\n"
		"lw      %[tmp8], 24(%[arg0])\n"
		"addiu   $v1, $zero, 2\n"
		"addu    %[tmp8], %[tmp8], %[tmp9]\n"
	"2:\n"
		"lq      %[tmp0], 256(%[tmp10])\n"
		"lq      %[tmp1], 272(%[tmp10])\n"
		"lq      %[tmp2], 288(%[tmp10])\n"
		"lq      %[tmp3], 304(%[tmp10])\n"
		"addiu   $v1, $v1, -1\n"
		"lq      %[tmp4], 320(%[tmp10])\n"
		"lq      %[tmp5], 336(%[tmp10])\n"
		"lq      %[tmp6], 352(%[tmp10])\n"
		"lq      %[tmp7], 368(%[tmp10])\n"
		"addiu   %[tmp10], %[tmp10], 128\n"
		"pmaxh   %[tmp0], $zero, %[tmp0]\n"
		"pmaxh   %[tmp1], $zero, %[tmp1]\n"
		"pmaxh   %[tmp2], $zero, %[tmp2]\n"
		"pmaxh   %[tmp3], $zero, %[tmp3]\n"
		"pmaxh   %[tmp4], $zero, %[tmp4]\n"
		"pmaxh   %[tmp5], $zero, %[tmp5]\n"
		"pmaxh   %[tmp6], $zero, %[tmp6]\n"
		"pmaxh   %[tmp7], $zero, %[tmp7]\n"
		"pminh   %[tmp0], $v0, %[tmp0]\n"
		"pminh   %[tmp1], $v0, %[tmp1]\n"
		"pminh   %[tmp2], $v0, %[tmp2]\n"
		"pminh   %[tmp3], $v0, %[tmp3]\n"
		"pminh   %[tmp4], $v0, %[tmp4]\n"
		"pminh   %[tmp5], $v0, %[tmp5]\n"
		"pminh   %[tmp6], $v0, %[tmp6]\n"
		"pminh   %[tmp7], $v0, %[tmp7]\n"
		"ppacb   %[tmp0], $zero, %[tmp0]\n"
		"ppacb   %[tmp1], $zero, %[tmp1]\n"
		"ppacb   %[tmp2], $zero, %[tmp2]\n"
		"ppacb   %[tmp3], $zero, %[tmp3]\n"
		"ppacb   %[tmp4], $zero, %[tmp4]\n"
		"ppacb   %[tmp5], $zero, %[tmp5]\n"
		"ppacb   %[tmp6], $zero, %[tmp6]\n"
		"ppacb   %[tmp7], $zero, %[tmp7]\n"
		"sd      %[tmp0],  0(%[tmp9])\n"
		"sd      %[tmp1], 16(%[tmp9])\n"
		"sd      %[tmp2], 32(%[tmp9])\n"
		"sd      %[tmp3], 48(%[tmp9])\n"
		"sd      %[tmp4],  0(%[tmp8])\n"
		"sd      %[tmp5], 16(%[tmp8])\n"
		"sd      %[tmp6], 32(%[tmp8])\n"
		"sd      %[tmp7], 48(%[tmp8])\n"
		"addiu   %[tmp9], %[tmp9], 64\n"
		"bgtzl   $v1, 2b\n"
		"addiu   %[tmp8], %[tmp8], 64\n"
	: [tmp0] "=r"(tmp0), [tmp1] "=r"(tmp1), [tmp2] "=r"(tmp2), [tmp3] "=r"(tmp3), [tmp4] "=r"(tmp4), [tmp5] "=r"(tmp5), [tmp6] "=r"(tmp6), [tmp7] "=r"(tmp7), [tmp8] "=r"(tmp8), [tmp9] "=r"(tmp9), [tmp10] "=r"(tmp10)
	: [arg0] "r"(arg0)
	: "a1", "v0", "v1", "memory"
	);
}

void _MPEG_add_block_frfr ( _MPEGMotions* arg0 )
{
	u128 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10;
	asm __volatile__(
		"pnor    $v0, $zero, $zero\n"
		"lw      %[tmp9],  0(%[arg0])\n"
		"lw      %[tmp10], 12(%[arg0])\n"
		"lw      $a1, 16($a1)\n"
		"addiu   $v1, $zero, 6\n"
		"psrlh   $v0, $v0, 8\n"
	"1:\n"
		"lq      %[tmp0],   0(%[tmp10])\n"
		"lq      %[tmp1],  16(%[tmp10])\n"
		"lq      %[tmp2],  32(%[tmp10])\n"
		"lq      %[tmp3],  48(%[tmp10])\n"
		"addiu   $v1, $v1, -1\n"
		"lq      %[tmp4],   0($a1)\n"
		"lq      %[tmp5],  16($a1)\n"
		"lq      %[tmp6],  32($a1)\n"
		"lq      %[tmp7],  48($a1)\n"
		"paddh   %[tmp0], %[tmp0], %[tmp4]\n"
		"paddh   %[tmp1], %[tmp1], %[tmp5]\n"
		"paddh   %[tmp2], %[tmp2], %[tmp6]\n"
		"paddh   %[tmp3], %[tmp3], %[tmp7]\n"
		"pmaxh   %[tmp0], $zero, %[tmp0]\n"
		"pmaxh   %[tmp1], $zero, %[tmp1]\n"
		"pmaxh   %[tmp2], $zero, %[tmp2]\n"
		"pmaxh   %[tmp3], $zero, %[tmp3]\n"
		"pminh   %[tmp0], $v0, %[tmp0]\n"
		"pminh   %[tmp1], $v0, %[tmp1]\n"
		"pminh   %[tmp2], $v0, %[tmp2]\n"
		"pminh   %[tmp3], $v0, %[tmp3]\n"
		"ppacb   %[tmp0], %[tmp1], %[tmp0]\n"
		"ppacb   %[tmp2], %[tmp3], %[tmp2]\n"
		"sq      %[tmp0],  0(%[tmp9])\n"
		"sq      %[tmp2], 16(%[tmp9])\n"
		"lq      %[tmp4],  64(%[tmp10])\n"
		"lq      %[tmp5],  80(%[tmp10])\n"
		"lq      %[tmp6],  96(%[tmp10])\n"
		"lq      %[tmp7], 112(%[tmp10])\n"
		"addiu   %[tmp10], %[tmp10], 128\n"
		"lq      %[tmp0],  64($a1)\n"
		"lq      %[tmp1],  80($a1)\n"
		"lq      %[tmp2],  96($a1)\n"
		"lq      %[tmp3], 112($a1)\n"
		"addiu   $a1, $a1, 128\n"
		"paddh   %[tmp4], %[tmp4], %[tmp0]\n"
		"paddh   %[tmp5], %[tmp5], %[tmp1]\n"
		"paddh   %[tmp6], %[tmp6], %[tmp2]\n"
		"paddh   %[tmp7], %[tmp7], %[tmp3]\n"
		"pmaxh   %[tmp4], $zero, %[tmp4]\n"
		"pmaxh   %[tmp5], $zero, %[tmp5]\n"
		"pmaxh   %[tmp6], $zero, %[tmp6]\n"
		"pmaxh   %[tmp7], $zero, %[tmp7]\n"
		"pminh   %[tmp4], $v0, %[tmp4]\n"
		"pminh   %[tmp5], $v0, %[tmp5]\n"
		"pminh   %[tmp6], $v0, %[tmp6]\n"
		"pminh   %[tmp7], $v0, %[tmp7]\n"
		"ppacb   %[tmp4], %[tmp5], %[tmp4]\n"
		"ppacb   %[tmp6], %[tmp7], %[tmp6]\n"
		"sq      %[tmp4], 32(%[tmp9])\n"
		"sq      %[tmp6], 48(%[tmp9])\n"
		"bgtzl   $v1, 1b\n"
		"addiu   %[tmp9], %[tmp9], 64\n"
	: [tmp0] "=r"(tmp0), [tmp1] "=r"(tmp1), [tmp2] "=r"(tmp2), [tmp3] "=r"(tmp3), [tmp4] "=r"(tmp4), [tmp5] "=r"(tmp5), [tmp6] "=r"(tmp6), [tmp7] "=r"(tmp7), [tmp8] "=r"(tmp8), [tmp9] "=r"(tmp9), [tmp10] "=r"(tmp10)
	: [arg0] "r"(arg0)
	: "a1", "v0", "v1", "memory"
	);
}

void _MPEG_add_block_ilfl ( _MPEGMotions* arg0 )
{
	u128 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10;
	asm __volatile__(
		"pnor    $v0, $zero, $zero\n"
		"lw      %[tmp9],  0(%[arg0])\n"
		"lw      %[tmp10], 12(%[arg0])\n"
		"lw      %[tmp8], 24(%[arg0])\n"
		"lw      $a1, 16(%[arg0])\n"
		"addiu   $v1, $zero, 4\n"
		"psrlh   $v0, $v0, 8\n"
		"addu    %[tmp8], %[tmp8], %[tmp9]\n"
	"1:\n"
		"lq      %[tmp0],   0(%[tmp10])\n"
		"lq      %[tmp1],  16(%[tmp10])\n"
		"lq      %[tmp2],  32(%[tmp10])\n"
		"lq      %[tmp3],  48(%[tmp10])\n"
		"addiu   $v1, $v1, -1\n"
		"lq      %[tmp4],   0($a1)\n"
		"lq      %[tmp5],  16($a1)\n"
		"lq      %[tmp6],  32($a1)\n"
		"lq      %[tmp7],  48($a1)\n"
		"paddh   %[tmp0], %[tmp0], %[tmp4]\n"
		"paddh   %[tmp1], %[tmp1], %[tmp5]\n"
		"paddh   %[tmp2], %[tmp2], %[tmp6]\n"
		"paddh   %[tmp3], %[tmp3], %[tmp7]\n"
		"pmaxh   %[tmp0], $zero, %[tmp0]\n"
		"pmaxh   %[tmp1], $zero, %[tmp1]\n"
		"pmaxh   %[tmp2], $zero, %[tmp2]\n"
		"pmaxh   %[tmp3], $zero, %[tmp3]\n"
		"pminh   %[tmp0], $v0, %[tmp0]\n"
		"pminh   %[tmp1], $v0, %[tmp1]\n"
		"pminh   %[tmp2], $v0, %[tmp2]\n"
		"pminh   %[tmp3], $v0, %[tmp3]\n"
		"ppacb   %[tmp0], %[tmp1], %[tmp0]\n"
		"ppacb   %[tmp2], %[tmp3], %[tmp2]\n"
		"sq      %[tmp0],   0(%[tmp9])\n"
		"sq      %[tmp2],  32(%[tmp9])\n"
		"lq      %[tmp4], 256(%[tmp10])\n"
		"lq      %[tmp5], 272(%[tmp10])\n"
		"lq      %[tmp6], 288(%[tmp10])\n"
		"lq      %[tmp7], 304(%[tmp10])\n"
		"addiu   %[tmp10], %[tmp10], 64\n"
		"lq      %[tmp0], 256($a1)\n"
		"lq      %[tmp1], 272($a1)\n"
		"lq      %[tmp2], 288($a1)\n"
		"lq      %[tmp3], 304($a1)\n"
		"paddh   %[tmp4], %[tmp4], %[tmp0]\n"
		"paddh   %[tmp5], %[tmp5], %[tmp1]\n"
		"paddh   %[tmp6], %[tmp6], %[tmp2]\n"
		"paddh   %[tmp7], %[tmp7], %[tmp3]\n"
		"pmaxh   %[tmp4], $zero, %[tmp4]\n"
		"pmaxh   %[tmp5], $zero, %[tmp5]\n"
		"pmaxh   %[tmp6], $zero, %[tmp6]\n"
		"pmaxh   %[tmp7], $zero, %[tmp7]\n"
		"pminh   %[tmp4], $v0, %[tmp4]\n"
		"pminh   %[tmp5], $v0, %[tmp5]\n"
		"pminh   %[tmp6], $v0, %[tmp6]\n"
		"pminh   %[tmp7], $v0, %[tmp7]\n"
		"ppacb   %[tmp4], %[tmp5], %[tmp4]\n"
		"ppacb   %[tmp6], %[tmp7], %[tmp6]\n"
		"sq      %[tmp4],  0(%[tmp8])\n"
		"sq      %[tmp6], 32(%[tmp8])\n"
		"addiu   %[tmp8], %[tmp8], 64\n"
		"addiu   $a1, $a1, 64\n"
		"bgtzl   $v1, 1b\n"
		"addiu   %[tmp9], %[tmp9], 64\n"
		"lw      %[tmp9],  4(%[arg0])\n"
		"lw      %[tmp8], 24(%[arg0])\n"
		"addiu   $v1, $zero, 2\n"
		"addu    %[tmp8], %[tmp8], %[tmp9]\n"
	"2:\n"
		"lq      %[tmp0], 256(%[tmp10])\n"
		"lq      %[tmp1], 272(%[tmp10])\n"
		"lq      %[tmp2], 288(%[tmp10])\n"
		"lq      %[tmp3], 304(%[tmp10])\n"
		"addiu   $v1, $v1, -1\n"
		"lq      %[tmp4], 256($a1)\n"
		"lq      %[tmp5], 272($a1)\n"
		"lq      %[tmp6], 288($a1)\n"
		"lq      %[tmp7], 304($a1)\n"
		"paddh   %[tmp0], %[tmp0], %[tmp4]\n"
		"paddh   %[tmp1], %[tmp1], %[tmp5]\n"
		"paddh   %[tmp2], %[tmp2], %[tmp6]\n"
		"paddh   %[tmp3], %[tmp3], %[tmp7]\n"
		"pmaxh   %[tmp0], $zero, %[tmp0]\n"
		"pmaxh   %[tmp1], $zero, %[tmp1]\n"
		"pmaxh   %[tmp2], $zero, %[tmp2]\n"
		"pmaxh   %[tmp3], $zero, %[tmp3]\n"
		"pminh   %[tmp0], $v0, %[tmp0]\n"
		"pminh   %[tmp1], $v0, %[tmp1]\n"
		"pminh   %[tmp2], $v0, %[tmp2]\n"
		"pminh   %[tmp3], $v0, %[tmp3]\n"
		"ppacb   %[tmp0], $zero, %[tmp0]\n"
		"ppacb   %[tmp1], $zero, %[tmp1]\n"
		"ppacb   %[tmp2], $zero, %[tmp2]\n"
		"ppacb   %[tmp3], $zero, %[tmp3]\n"
		"sd      %[tmp0],  0(%[tmp9])\n"
		"sd      %[tmp1], 16(%[tmp9])\n"
		"sd      %[tmp2], 32(%[tmp9])\n"
		"sd      %[tmp3], 48(%[tmp9])\n"
		"lq      %[tmp4], 320(%[tmp10])\n"
		"lq      %[tmp5], 336(%[tmp10])\n"
		"lq      %[tmp6], 352(%[tmp10])\n"
		"lq      %[tmp7], 368(%[tmp10])\n"
		"addiu   %[tmp10], %[tmp10], 128\n"
		"lq      %[tmp0], 320($a1)\n"
		"lq      %[tmp1], 336($a1)\n"
		"lq      %[tmp2], 352($a1)\n"
		"lq      %[tmp3], 368($a1)\n"
		"paddh   %[tmp4], %[tmp4], %[tmp0]\n"
		"paddh   %[tmp5], %[tmp5], %[tmp1]\n"
		"paddh   %[tmp6], %[tmp6], %[tmp2]\n"
		"paddh   %[tmp7], %[tmp7], %[tmp3]\n"
		"pmaxh   %[tmp4], $zero, %[tmp4]\n"
		"pmaxh   %[tmp5], $zero, %[tmp5]\n"
		"pmaxh   %[tmp6], $zero, %[tmp6]\n"
		"pmaxh   %[tmp7], $zero, %[tmp7]\n"
		"pminh   %[tmp4], $v0, %[tmp4]\n"
		"pminh   %[tmp5], $v0, %[tmp5]\n"
		"pminh   %[tmp6], $v0, %[tmp6]\n"
		"pminh   %[tmp7], $v0, %[tmp7]\n"
		"ppacb   %[tmp4], $zero, %[tmp4]\n"
		"ppacb   %[tmp5], $zero, %[tmp5]\n"
		"ppacb   %[tmp6], $zero, %[tmp6]\n"
		"ppacb   %[tmp7], $zero, %[tmp7]\n"
		"sd      %[tmp4],  0(%[tmp8])\n"
		"sd      %[tmp5], 16(%[tmp8])\n"
		"sd      %[tmp6], 32(%[tmp8])\n"
		"sd      %[tmp7], 48(%[tmp8])\n"
		"addiu   %[tmp9], %[tmp9], 64\n"
		"addiu   %[tmp8], %[tmp8], 64\n"
		"bgtzl   $v1, 2b\n"
		"addiu   $a1, $a1, 128\n"
	: [tmp0] "=r"(tmp0), [tmp1] "=r"(tmp1), [tmp2] "=r"(tmp2), [tmp3] "=r"(tmp3), [tmp4] "=r"(tmp4), [tmp5] "=r"(tmp5), [tmp6] "=r"(tmp6), [tmp7] "=r"(tmp7), [tmp8] "=r"(tmp8), [tmp9] "=r"(tmp9), [tmp10] "=r"(tmp10)
	: [arg0] "r"(arg0)
	: "a1", "v0", "v1", "memory"
	);
}

void _MPEG_add_block_frfl ( _MPEGMotions* arg0 )
{
	u128 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10;
	asm __volatile__(
		"pnor    $v0, $zero, $zero\n"
		"lw      %[tmp9],  0(%[arg0])\n"
		"lw      %[tmp10], 12(%[arg0])\n"
		"lw      $a1, 16(%[arg0])\n"
		"addiu   $v1, $zero, 4\n"
		"psrlh   $v0, $v0, 8\n"
	"1:\n"
		"lq      %[tmp0],   0(%[tmp10])\n"
		"lq      %[tmp1],  16(%[tmp10])\n"
		"lq      %[tmp2],  32(%[tmp10])\n"
		"lq      %[tmp3],  48(%[tmp10])\n"
		"addiu   $v1, $v1, -1\n"
		"lq      %[tmp4],   0($a1)\n"
		"lq      %[tmp5],  16($a1)\n"
		"lq      %[tmp6], 256($a1)\n"
		"lq      %[tmp7], 272($a1)\n"
		"paddh   %[tmp0], %[tmp0], %[tmp4]\n"
		"paddh   %[tmp1], %[tmp1], %[tmp5]\n"
		"paddh   %[tmp2], %[tmp2], %[tmp6]\n"
		"paddh   %[tmp3], %[tmp3], %[tmp7]\n"
		"pmaxh   %[tmp0], $zero, %[tmp0]\n"
		"pmaxh   %[tmp1], $zero, %[tmp1]\n"
		"pmaxh   %[tmp2], $zero, %[tmp2]\n"
		"pmaxh   %[tmp3], $zero, %[tmp3]\n"
		"pminh   %[tmp0], $v0, %[tmp0]\n"
		"pminh   %[tmp1], $v0, %[tmp1]\n"
		"pminh   %[tmp2], $v0, %[tmp2]\n"
		"pminh   %[tmp3], $v0, %[tmp3]\n"
		"ppacb   %[tmp0], %[tmp1], %[tmp0]\n"
		"ppacb   %[tmp2], %[tmp3], %[tmp2]\n"
		"sq      %[tmp0],   0(%[tmp9])\n"
		"sq      %[tmp2],  16(%[tmp9])\n"
		"lq      %[tmp4],  64(%[tmp10])\n"
		"lq      %[tmp5],  80(%[tmp10])\n"
		"lq      %[tmp6],  96(%[tmp10])\n"
		"lq      %[tmp7], 112(%[tmp10])\n"
		"addiu   %[tmp10], %[tmp10], 128\n"
		"lq      %[tmp0],  32($a1)\n"
		"lq      %[tmp1],  48($a1)\n"
		"lq      %[tmp2], 288($a1)\n"
		"lq      %[tmp3], 304($a1)\n"
		"paddh   %[tmp4], %[tmp4], %[tmp0]\n"
		"paddh   %[tmp5], %[tmp5], %[tmp1]\n"
		"paddh   %[tmp6], %[tmp6], %[tmp2]\n"
		"paddh   %[tmp7], %[tmp7], %[tmp3]\n"
		"pmaxh   %[tmp4], $zero, %[tmp4]\n"
		"pmaxh   %[tmp5], $zero, %[tmp5]\n"
		"pmaxh   %[tmp6], $zero, %[tmp6]\n"
		"pmaxh   %[tmp7], $zero, %[tmp7]\n"
		"pminh   %[tmp4], $v0, %[tmp4]\n"
		"pminh   %[tmp5], $v0, %[tmp5]\n"
		"pminh   %[tmp6], $v0, %[tmp6]\n"
		"pminh   %[tmp7], $v0, %[tmp7]\n"
		"ppacb   %[tmp4], %[tmp5], %[tmp4]\n"
		"ppacb   %[tmp6], %[tmp7], %[tmp6]\n"
		"sq      %[tmp4], 32(%[tmp9])\n"
		"sq      %[tmp6], 48(%[tmp9])\n"
		"addiu   $a1, $a1, 64\n"
		"bgtzl   $v1, 1b\n"
		"addiu   %[tmp9], %[tmp9], 64\n"
		"lw      %[tmp9], 4(%[arg0])\n"
		"addiu   $v1, $zero, 2\n"
	"2:\n"
		"lq      %[tmp0],   0(%[tmp10])\n"
		"lq      %[tmp1],  16(%[tmp10])\n"
		"lq      %[tmp2],  32(%[tmp10])\n"
		"lq      %[tmp3],  48(%[tmp10])\n"
		"addiu   $v1, $v1, -1\n"
		"lq      %[tmp4], 256($a1)\n"
		"lq      %[tmp5], 320($a1)\n"
		"lq      %[tmp6], 272($a1)\n"
		"lq      %[tmp7], 336($a1)\n"
		"paddh   %[tmp0], %[tmp0], %[tmp4]\n"
		"paddh   %[tmp1], %[tmp1], %[tmp5]\n"
		"paddh   %[tmp2], %[tmp2], %[tmp6]\n"
		"paddh   %[tmp3], %[tmp3], %[tmp7]\n"
		"pmaxh   %[tmp0], $zero, %[tmp0]\n"
		"pmaxh   %[tmp1], $zero, %[tmp1]\n"
		"pmaxh   %[tmp2], $zero, %[tmp2]\n"
		"pmaxh   %[tmp3], $zero, %[tmp3]\n"
		"pminh   %[tmp0], $v0, %[tmp0]\n"
		"pminh   %[tmp1], $v0, %[tmp1]\n"
		"pminh   %[tmp2], $v0, %[tmp2]\n"
		"pminh   %[tmp3], $v0, %[tmp3]\n"
		"ppacb   %[tmp0], %[tmp1], %[tmp0]\n"
		"ppacb   %[tmp2], %[tmp3], %[tmp2]\n"
		"sq      %[tmp0],  0(%[tmp9])\n"
		"sq      %[tmp2], 16(%[tmp9])\n"
		"lq      %[tmp4],  64(%[tmp10])\n"
		"lq      %[tmp5],  80(%[tmp10])\n"
		"lq      %[tmp6],  96(%[tmp10])\n"
		"lq      %[tmp7], 112(%[tmp10])\n"
		"addiu   %[tmp10], %[tmp10], 128\n"
		"lq      %[tmp0], 288($a1)\n"
		"lq      %[tmp1], 352($a1)\n"
		"lq      %[tmp2], 304($a1)\n"
		"lq      %[tmp3], 368($a1)\n"
		"paddh   %[tmp4], %[tmp4], %[tmp0]\n"
		"paddh   %[tmp5], %[tmp5], %[tmp1]\n"
		"paddh   %[tmp6], %[tmp6], %[tmp2]\n"
		"paddh   %[tmp7], %[tmp7], %[tmp3]\n"
		"pmaxh   %[tmp4], $zero, %[tmp4]\n"
		"pmaxh   %[tmp5], $zero, %[tmp5]\n"
		"pmaxh   %[tmp6], $zero, %[tmp6]\n"
		"pmaxh   %[tmp7], $zero, %[tmp7]\n"
		"pminh   %[tmp4], $v0, %[tmp4]\n"
		"pminh   %[tmp5], $v0, %[tmp5]\n"
		"pminh   %[tmp6], $v0, %[tmp6]\n"
		"pminh   %[tmp7], $v0, %[tmp7]\n"
		"ppacb   %[tmp4], %[tmp5], %[tmp4]\n"
		"ppacb   %[tmp6], %[tmp7], %[tmp6]\n"
		"sq      %[tmp4], 32(%[tmp9])\n"
		"sq      %[tmp6], 48(%[tmp9])\n"
		"addiu   %[tmp9], %[tmp9], 64\n"
		"bgtzl   $v1, 2b\n"
		"addiu   $a1, $a1, 128\n"
	: [tmp0] "=r"(tmp0), [tmp1] "=r"(tmp1), [tmp2] "=r"(tmp2), [tmp3] "=r"(tmp3), [tmp4] "=r"(tmp4), [tmp5] "=r"(tmp5), [tmp6] "=r"(tmp6), [tmp7] "=r"(tmp7), [tmp8] "=r"(tmp8), [tmp9] "=r"(tmp9), [tmp10] "=r"(tmp10)
	: [arg0] "r"(arg0)
	: "a1", "v0", "v1", "memory"
	);
}

#if 0
void _MPEG_do_mc ( _MPEGMotion* arg0 )
{
	// TODO
}

void _MPEG_put_luma ( int arg0, void* arg1, void* arg2, u8 arg3 )
{
	// TODO
}

void _MPEG_put_chroma ( int arg0, void* arg1, void* arg2, u8 arg3 )
{
	// TODO
}

void _MPEG_put_luma_X ( int arg0, void* arg1, void* arg2, u8 arg3 )
{
	// TODO
}

void _MPEG_put_chroma_X ( int arg0, void* arg1, void* arg2, u8 arg3 )
{
	// TODO
}

void _MPEG_put_luma_Y ( int arg0, void* arg1, void* arg2, u8 arg3 )
{
	// TODO
}

void _MPEG_put_chroma_Y ( int arg0, void* arg1, void* arg2, u8 arg3 )
{
	// TODO
}

void _MPEG_put_luma_XY ( int arg0, void* arg1, void* arg2, u8 arg3 )
{
	// TODO
}

void _MPEG_put_chroma_XY ( int arg0, void* arg1, void* arg2, u8 arg3 )
{
	// TODO
}

void _MPEG_avg_luma ( int arg0, void* arg1, void* arg2, u8 arg3 )
{
	// TODO
}

void _MPEG_avg_chroma ( int arg0, void* arg1, void* arg2, u8 arg3 )
{
	// TODO
}

void _MPEG_avg_luma_X ( int arg0, void* arg1, void* arg2, u8 arg3 )
{
	// TODO
}

void _MPEG_avg_chroma_X ( int arg0, void* arg1, void* arg2, u8 arg3 )
{
	// TODO
}

void _MPEG_avg_luma_Y ( int arg0, void* arg1, void* arg2, u8 arg3 )
{
	// TODO
}

void _MPEG_avg_chroma_Y ( int arg0, void* arg1, void* arg2, u8 arg3 )
{
	// TODO
}

void _MPEG_avg_luma_XY ( int arg0, void* arg1, void* arg2, u8 arg3 )
{
	// TODO
}

void _MPEG_avg_chroma_XY ( int arg0, void* arg1, void* arg2, u8 arg3 )
{
	// TODO
}
#endif
