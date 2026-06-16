/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

#include "ioptrap.h"
#include <cop0regs.h>

void set_dba(u32 v)
{
    COP0REG_BDA = v;
}

void set_dbam(u32 v)
{
    COP0REG_BDAM = v;
}

void set_dcic(u32 v)
{
    COP0REG_DCIC = v;
}

u32 get_dba()
{
    u32 v;
    v = COP0REG_BDA;
    return v;
}

u32 get_dbam()
{
    u32 v;
    v = COP0REG_BDAM;
    return v;
}

u32 get_dcic()
{
    u32 v;
    v = COP0REG_DCIC;
    return v;
}
