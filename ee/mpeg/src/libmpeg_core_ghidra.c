
void _ipu_suspend(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  do {
    DI();
    SYNC(0x10);
  } while ((Status & 0x10000) != 0);
  uVar2 = read_volatile_4(REG_DMAC_ENABLER);
  write_volatile_4(REG_DMAC_ENABLEW,uVar2 | 0x10000);
  uVar2 = read_volatile_4(REG_DMAC_4_IPU_TO_CHCR);
  write_volatile_4(REG_DMAC_4_IPU_TO_CHCR,uVar2 & 0xfffffeff);
  uVar1 = read_volatile_4(REG_DMAC_ENABLER);
  write_volatile_4(REG_DMAC_ENABLEW,uVar1 & 0xfffeffff);
  EI();
  s_IPUState + 4 = read_volatile_4(REG_DMAC_4_IPU_TO_MADR);
  s_IPUState + 8 = read_volatile_4(REG_DMAC_4_IPU_TO_QWC);
  do {
    uVar1 = read_volatile_4(REG_IPU_CTRL);
  } while ((uVar1 & 0xf0) != 0);
  do {
    DI();
    SYNC(0x10);
  } while ((Status & 0x10000) != 0);
  uVar1 = read_volatile_4(REG_DMAC_ENABLER);
  write_volatile_4(REG_DMAC_ENABLEW,uVar1 | 0x10000);
  uVar1 = read_volatile_4(REG_DMAC_3_IPU_FROM_CHCR);
  write_volatile_4(REG_DMAC_3_IPU_FROM_CHCR,uVar1 & 0xfffffeff);
  uVar3 = read_volatile_4(REG_DMAC_ENABLER);
  write_volatile_4(REG_DMAC_ENABLEW,uVar3 & 0xfffeffff);
  EI();
  s_IPUState + 16 = read_volatile_4(REG_DMAC_3_IPU_FROM_MADR);
  s_IPUState + 20 = read_volatile_4(REG_DMAC_3_IPU_FROM_QWC);
  s_IPUState + 24 = read_volatile_4(REG_IPU_CTRL);
  s_IPUState + 28 = read_volatile_4(REG_IPU_BP);
  s_IPUState + 0 = uVar2 & 0xfffffeff;
  s_IPUState + 12 = uVar1 & 0xfffffeff;
  return;
}


void _ipu_sync(uint param_1)

{
  int iVar1;
  int in_at_lo;
  long lVar2;
  uint in_v1_lo;
  undefined4 unaff_retaddr;
  
  do {
    while( true ) {
      if ((in_v1_lo & 0x4000) != 0) {
        return;
      }
      if ((int)((((param_1 & 0xff00) >> 1) + ((param_1 & 0x30000) >> 9)) - (param_1 & 0x7f)) < 0x20)
      break;
LAB_0001041c:
      in_v1_lo = *(uint *)(in_at_lo + 0x2010);
      if (-1 < (int)in_v1_lo) {
        return;
      }
      param_1 = *(uint *)(in_at_lo + 0x2020);
    }
    iVar1 = read_volatile_4(REG_DMAC_4_IPU_TO_QWC);
    if (iVar1 < 1) {
      *(undefined4 *)((int)register0x000001d0 + 0xfff0) = unaff_retaddr;
      lVar2 = (*s_SetDMA + 0)(s_SetDMA + 4);
      unaff_retaddr = *(undefined4 *)((int)register0x000001d0 + 0xfff0);
      register0x000001d0 = (BADSPACEBASE *)((int)register0x000001d0 + 0x10000);
      if (lVar2 == 0) {
        *s_pEOF = 0x20;
        s_DataBuf = 0x20;
        s_DataBuf + 4 = 0x1b7;
        return;
      }
      in_at_lo = 0x10000000;
      goto LAB_0001041c;
    }
    param_1 = *(uint *)(in_at_lo + 0x2020);
    in_v1_lo = *(uint *)(in_at_lo + 0x2010);
  } while( true );
}


void _ipu_sync_data(void)

{
  int iVar1;
  long lVar2;
  uint uVar3;
  undefined4 unaff_retaddr;
  
  lVar2 = read_volatile_8(REG_IPU_CMD);
  if (-1 < lVar2) {
    return;
  }
  uVar3 = read_volatile_4(REG_IPU_BP);
  do {
    while (0x1f < (((uVar3 & 0xff00) >> 1) + ((uVar3 & 0x30000) >> 9)) - (uVar3 & 0x7f)) {
LAB_000104b8:
      lVar2 = read_volatile_8(REG_IPU_CMD);
      if (-1 < lVar2) {
        return;
      }
      uVar3 = read_volatile_4(REG_IPU_BP);
    }
    iVar1 = read_volatile_4(REG_DMAC_4_IPU_TO_QWC);
    if (iVar1 < 1) {
      *(undefined4 *)((int)register0x000001d0 + 0xfff0) = unaff_retaddr;
      lVar2 = (*s_SetDMA + 0)(s_SetDMA + 4);
      unaff_retaddr = *(undefined4 *)((int)register0x000001d0 + 0xfff0);
      register0x000001d0 = (BADSPACEBASE *)((int)register0x000001d0 + 0x10000);
      if (lVar2 == 0) {
        *s_pEOF + 0 = 0x20;
        s_SetDMA + 0 = 0x20;
        s_SetDMA + 4 = 0x1b7;
        return;
      }
      goto LAB_000104b8;
    }
    uVar3 = read_volatile_4(REG_IPU_BP);
  } while( true );
}


/* WARNING: Instruction at (ram,0x000105d4) overlaps instruction at (ram,0x000105d0)
    */

void _MPEG_AlignBits(void)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  
  iVar2 = 0x10000000;
  iVar1 = read_volatile_4(REG_IPU_CTRL);
  if (iVar1 < 0) {
    uVar3 = read_volatile_4(REG_IPU_BP);
    iVar2 = 0x10000000;
    _ipu_sync(uVar3);
  }
  uVar3 = -(*(uint *)(iVar2 + 0x2020) & 7) & 7;
  if (uVar3 != 0) {
    _MPEG_GetBits(uVar3);
    return;
  }
  return;
}


void _MPEG_BDEC(int param_1,int param_2,int param_3,int param_4,void *param_5)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  
  iVar3 = 0x10000000;
  iVar1 = read_volatile_4(REG_IPU_CTRL);
  write_volatile_4(REG_DMAC_3_IPU_FROM_MADR,(uint)param_5 & 0xfffffff | 0x80000000);
  write_volatile_4(REG_DMAC_3_IPU_FROM_QWC,0x30);
  write_volatile_4(REG_DMAC_3_IPU_FROM_CHCR,0x100);
  if (iVar1 < 0) {
    uVar2 = read_volatile_4(REG_IPU_BP);
    iVar3 = 0x10000000;
    _ipu_sync(uVar2);
  }
  *(uint *)(iVar3 + 0x2000) =
       param_1 << 0x1b | 0x20000000U | param_2 << 0x1a | param_3 << 0x19 | param_4 << 0x10;
  return;
}


int _MPEG_CSCImage(void *param_1,void *param_2,int param_3)

{
  int extraout_a1_lo;
  uint uVar1;
  int extraout_t1_lo;
  
  _ipu_suspend();
  *(undefined4 *)(extraout_t1_lo + 0x2000) = 0;
  *(undefined4 *)(extraout_a1_lo + -0x1ff0) = 8;
  uVar1 = param_3;
  if (0x3fe < param_3) {
    uVar1 = 0x3ff;
  }
  s_CSCParam + 8 = param_3 - uVar1;
  *(void **)(extraout_a1_lo + -0x4ff0) = param_2;
  *(void **)(extraout_a1_lo + -0x4bf0) = param_1;
  s_CSCParam + 0 = (void *)((int)param_1 + uVar1 * 0x180);
  s_CSCParam + 4 = (void *)((int)param_2 + uVar1 * 0x400);
  *(uint *)(extraout_a1_lo + -0x4be0) = uVar1 * 0x180 >> 4;
  *(uint *)(extraout_a1_lo + -0x4fe0) = uVar1 * 0x400 >> 4;
  syscall(0);
  uVar1 = uVar1 | 0x70000000;
  write_volatile_4(REG_DMAC_4_IPU_TO_CHCR,0x101);
  write_volatile_4(REG_IPU_CMD,uVar1);
  write_volatile_4(REG_DMAC_3_IPU_FROM_CHCR,0x100);
  s_CSCFlag = 0x44; // probably should be the result of syscall instead…
  syscall(0);
  _MPEG_Resume();
  return (int)uVar1;
}


void _MPEG_dma_ref_image(_MPEGMacroBlock8 *param_1,_MPEGMotion *param_2,int param_3,int param_4)

{
  uchar *puVar1;
  undefined4 *puVar2;
  uint uVar3;
  _MPEGMotion *p_Var4;
  ulong uVar5;
  undefined4 *puVar6;
  
  uVar3 = 4;
  if (param_3 < 5) {
    uVar3 = param_3;
  }
  if (param_3 >> 0x1f < 1) {
    uVar5 = CONCAT44(param_3 >> 0x1f,uVar3);
  }
  else {
    uVar5 = (ulong)uVar3;
  }
  if (0 < (long)uVar5) {
    do {
      uVar3 = read_volatile_4(REG_DMAC_9_SPR_TO_CHCR);
    } while ((uVar3 & 0x100) != 0);
    write_volatile_4(REG_DMAC_9_SPR_TO_QWC,0);
    write_volatile_4(REG_DMAC_9_SPR_TO_SADR,(uint)param_1 & 0xfffffff);
    write_volatile_4(REG_DMAC_9_SPR_TO_TADR,&gp0x0000c000);
    puVar2 = (undefined4 *)((uint)&gp0x0000c000 | 0x20000000);
    do {
      p_Var4 = param_2;
      puVar6 = puVar2;
      puVar1 = p_Var4->m_pSrc;
      uVar5 = (ulong)((int)uVar5 + 0xffff);
      *puVar6 = 0x30000030;
      puVar6[1] = puVar1;
      puVar6[4] = 0x30000030;
      puVar6[5] = puVar1 + param_4 * 0x180;
      p_Var4->m_pSrc = (uchar *)param_1;
      param_1 = param_1 + 4;
      puVar2 = puVar6 + 8;
      param_2 = p_Var4 + 1;
    } while (uVar5 != 0);
    puVar6[4] = 0x30;
    p_Var4[1].MC_Luma = (_func_1 *)0x0;
    SYNC(0);
    write_volatile_4(REG_DMAC_9_SPR_TO_CHCR,0x105);
    return;
  }
  return;
}


uint _MPEG_GetBits(uint param_1)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  
  iVar2 = 0x10000000;
  iVar1 = read_volatile_4(REG_IPU_CTRL);
  if (iVar1 < 0) {
    uVar3 = read_volatile_4(REG_IPU_BP);
    iVar2 = 0x10000000;
    _ipu_sync(uVar3);
  }
  if (s_DataBuf + 0 < (int)param_1) {
    iVar2 = 0x10000000;
    write_volatile_4(REG_IPU_CMD,0x40000000);
    s_DataBuf + 4 = _ipu_sync_data();
    s_DataBuf + 0 = 0x20;
  }
  *(uint *)(iVar2 + 0x2000) = param_1 | 0x40000000;
  uVar3 = s_DataBuf + 4 >> (-param_1 & 0x1f);
  s_DataBuf + 0 = s_DataBuf + 0 - param_1;
  s_DataBuf + 4 = s_DataBuf + 4 << (param_1 & 0x1f);
  return uVar3;
}


int _MPEG_GetDMVector(void)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  long lVar4;
  
  iVar3 = 0x10000000;
  iVar1 = read_volatile_4(REG_IPU_CTRL);
  if (iVar1 < 0) {
    uVar2 = read_volatile_4(REG_IPU_BP);
    iVar3 = 0x10000000;
    _ipu_sync(uVar2);
  }
  *(undefined4 *)(iVar3 + 0x2000) = 0x3c000000;
  lVar4 = _ipu_sync_data();
  s_DataBuf + 0 = 0x20;
  s_DataBuf + 4 = (int)*(undefined8 *)(iVar3 + 0x2030);
  return (int)((lVar4 << 0x30) >> 0x30);
}


int _MPEG_GetMBAI(void)

{
  int iVar1;
  uint uVar2;
  undefined8 uVar3;
  ulong uVar4;
  int iVar5;
  
  uVar3 = 0x10000000;
  iVar1 = read_volatile_4(REG_IPU_CTRL);
  iVar5 = 0;
  if (iVar1 < 0) {
    uVar2 = read_volatile_4(REG_IPU_BP);
    _ipu_sync(uVar2);
  }
  while( true ) {
    *(undefined4 *)((int)uVar3 + 0x2000) = 0x30000000;
    uVar4 = _ipu_sync_data();
    if (uVar4 == 0) {
      return 0;
    }
    uVar4 = uVar4 & 0xffff;
    if (uVar4 < 0x22) break;
    if (uVar4 == 0x23) {
      iVar5 = iVar5 + 0x21;
    }
  }
  s_DataBuf + 0 = 0x20;
  s_DataBuf + 4 = (int)*(undefined8 *)((int)uVar3 + 0x2030);
  return iVar5 + (int)uVar4;
}


int _MPEG_GetMBType(void)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  ulong uVar4;
  
  iVar3 = 0x10000000;
  iVar1 = read_volatile_4(REG_IPU_CTRL);
  if (iVar1 < 0) {
    uVar2 = read_volatile_4(REG_IPU_BP);
    iVar3 = 0x10000000;
    _ipu_sync(uVar2);
  }
  *(undefined4 *)(iVar3 + 0x2000) = 0x34000000;
  uVar4 = _ipu_sync_data();
  if (uVar4 != 0) {
    uVar4 = uVar4 & 0xffff;
    s_DataBuf + 0 = 0x20;
    s_DataBuf + 4 = (undefined4)*(undefined8 *)(iVar3 + 0x2030);
  }
  return (int)uVar4;
}


int _MPEG_GetMotionCode(void)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  ulong uVar4;
  
  iVar3 = 0x10000000;
  iVar1 = read_volatile_4(REG_IPU_CTRL);
  if (iVar1 < 0) {
    uVar2 = read_volatile_4(REG_IPU_BP);
    iVar3 = 0x10000000;
    _ipu_sync(uVar2);
  }
  *(undefined4 *)(iVar3 + 0x2000) = 0x38000000;
  uVar4 = _ipu_sync_data();
  if (uVar4 == 0) {
    uVar4 = 0x8000;
  }
  else {
    uVar4 = uVar4 & 0xffff;
    s_DataBuf + 0 = 0x20;
    s_DataBuf + 4 = (undefined4)*(undefined8 *)(iVar3 + 0x2030);
  }
  return (int)((long)(uVar4 << 0x30) >> 0x30);
}


void _MPEG_Initialize(_MPEGContext *param_1,int *param_2,void *param_3,int *param_4)

{
  int iVar1;
  uint uVar2;
  
  write_volatile_4(REG_IPU_CTRL,0x40000000);
  do {
    iVar1 = read_volatile_4(REG_IPU_CTRL);
  } while (iVar1 < 0);
  write_volatile_4(REG_IPU_CMD,0);
  do {
    uVar2 = read_volatile_4(REG_IPU_CTRL);
  } while ((int)uVar2 < 0);
  write_volatile_4(REG_IPU_CTRL,uVar2 | 0x800000);
  write_volatile_4(REG_DMAC_3_IPU_FROM_QWC,0);
  write_volatile_4(REG_DMAC_4_IPU_TO_QWC,0);
  *param_4 = 0;
  syscall(0);
  syscall(0);
  s_DataBuf + 0 = 0;
  s_SetDMA + 0 = param_2;
  s_SetDMA + 4 = param_3;
  s_pEOF + 0 = param_4;
  s_Sema = 0x1001;
  s_CSCID = 0x1001;
  s_DataBuf = 0x00000000;
  // s_CSCParam and _mpeg_dmac_handler ???
  return;
}


uint _MPEG_NextStartCode(void)

{
  uint uVar1;
  
  _MPEG_AlignBits();
  while (uVar1 = _MPEG_ShowBits(0x18), uVar1 != 1) {
    _MPEG_GetBits(8);
  }
  uVar1 = _MPEG_ShowBits(0x20);
  return uVar1;
}


void _MPEG_Resume(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  if (iGpffffc0a4 != 0) {
    write_volatile_4(REG_DMAC_3_IPU_FROM_MADR,s_IPUState + 16);
    write_volatile_4(REG_DMAC_3_IPU_FROM_QWC,iGpffffc0a4);
    write_volatile_4(REG_DMAC_3_IPU_FROM_CHCR,s_IPUState + 12 | 0x100);
  }
  iVar2 = (s_IPUState + 28 >> 0x10 & 3) + (s_IPUState + 28 >> 8 & 0xf);
  iVar3 = iGpffffc098 + iVar2;
  if (iVar3 != 0) {
    write_volatile_4(REG_IPU_CMD,s_IPUState + 28 & 0x7f);
    do {
      iVar1 = read_volatile_4(REG_IPU_CTRL);
    } while (iVar1 < 0);
    write_volatile_4(REG_IPU_CTRL,s_IPUState + 24);
    write_volatile_4(REG_DMAC_4_IPU_TO_MADR,iGpffffc094 + iVar2 * -0x10);
    write_volatile_4(REG_DMAC_4_IPU_TO_QWC,iVar3);
    write_volatile_4(REG_DMAC_4_IPU_TO_CHCR,s_IPUState + 0 | 0x100);
  }
  return;
}


void _MPEG_SetDefQM(int param_1)

{
  int iVar1;
  
  _ipu_suspend();
  write_volatile_4(REG_IPU_CMD,0);
  do {
    iVar1 = read_volatile_4(REG_IPU_CTRL);
  } while (iVar1 < 0);
  write_volatile_4(REG_IPU_IN_FIFO,(int)s_DefQM);
  write_volatile_4(DAT_10007014,(int)((ulong)s_DefQM >> 0x20));
  write_volatile_4(DAT_10007018,s_DefQM + 0x8);
  write_volatile_4(DAT_1000701c,s_DefQM + 0xc);
  write_volatile_4(REG_IPU_IN_FIFO,s_DefQM + 0x10);
  write_volatile_4(DAT_10007014,s_DefQM + 0x14);
  write_volatile_4(DAT_10007018,s_DefQM + 0x18);
  write_volatile_4(DAT_1000701c,s_DefQM + 0x1c);
  write_volatile_4(REG_IPU_IN_FIFO,s_DefQM + 0x20);
  write_volatile_4(DAT_10007014,s_DefQM + 0x24);
  write_volatile_4(DAT_10007018,s_DefQM + 0x28);
  write_volatile_4(DAT_1000701c,s_DefQM + 0x2c);
  write_volatile_4(REG_IPU_IN_FIFO,s_DefQM + 0x30);
  write_volatile_4(DAT_10007014,s_DefQM + 0x34);
  write_volatile_4(DAT_10007018,s_DefQM + 0x38);
  write_volatile_4(DAT_1000701c,s_DefQM + 0x3c);
  write_volatile_4(REG_IPU_CMD,0x50000000);
  do {
    iVar1 = read_volatile_4(REG_IPU_CTRL);
  } while (iVar1 < 0);
  write_volatile_4(REG_IPU_IN_FIFO,s_DefQM + 0x40);
  write_volatile_4(DAT_10007014,s_DefQM + 0x44);
  write_volatile_4(DAT_10007018,s_DefQM + 0x48);
  write_volatile_4(DAT_1000701c,s_DefQM + 0x4c);
  write_volatile_4(REG_IPU_IN_FIFO,s_DefQM + 0x40);
  write_volatile_4(DAT_10007014,s_DefQM + 0x44);
  write_volatile_4(DAT_10007018,s_DefQM + 0x48);
  write_volatile_4(DAT_1000701c,s_DefQM + 0x4c);
  write_volatile_4(REG_IPU_IN_FIFO,s_DefQM + 0x40);
  write_volatile_4(DAT_10007014,s_DefQM + 0x44);
  write_volatile_4(DAT_10007018,s_DefQM + 0x48);
  write_volatile_4(DAT_1000701c,s_DefQM + 0x4c);
  write_volatile_4(REG_IPU_IN_FIFO,s_DefQM + 0x40);
  write_volatile_4(DAT_10007014,s_DefQM + 0x44);
  write_volatile_4(DAT_10007018,s_DefQM + 0x48);
  write_volatile_4(DAT_1000701c,s_DefQM + 0x4c);
  write_volatile_4(REG_IPU_CMD,0x58000000);
  do {
    iVar1 = read_volatile_4(REG_IPU_CTRL);
  } while (iVar1 < 0);
  _MPEG_Resume();
  return;
}


void _MPEG_SetIDCP(void)

{
  int in_at_lo;
  uint uVar1;
  
  uVar1 = _MPEG_GetBits(2);
  *(uint *)(in_at_lo + 0x2010) = *(uint *)(in_at_lo + 0x2010) & 0xfffcffff | uVar1 << 0x10;
  return;
}


void _MPEG_SetPCT(uint param_1)

{
  int iVar1;
  long lVar2;
  uint uVar3;
  uint uVar4;
  undefined *puVar5;
  undefined4 unaff_retaddr;
  
  puVar5 = &stack0x0000fff0;
  uVar3 = read_volatile_4(REG_IPU_CTRL);
  if (-1 < (int)uVar3) {
    write_volatile_4(REG_IPU_CTRL,uVar3 & 0xf8ffffff | param_1 << 0x18);
    return;
  }
  uVar4 = read_volatile_4(REG_IPU_BP);
  do {
    while( true ) {
      if ((uVar3 & 0x4000) != 0) {
        return;
      }
      if ((int)((((uVar4 & 0xff00) >> 1) + ((uVar4 & 0x30000) >> 9)) - (uVar4 & 0x7f)) < 0x20)
      break;
LAB_0001041c:
      uVar3 = read_volatile_4(REG_IPU_CTRL);
      if (-1 < (int)uVar3) {
        return;
      }
      uVar4 = read_volatile_4(REG_IPU_BP);
    }
    iVar1 = read_volatile_4(REG_DMAC_4_IPU_TO_QWC);
    if (iVar1 < 1) {
      *(undefined4 *)(puVar5 + 0xfff0) = unaff_retaddr;
      lVar2 = (*s_SetDMA + 0)(s_SetDMA + 4);
      unaff_retaddr = *(undefined4 *)(puVar5 + 0xfff0);
      puVar5 = puVar5 + 0x10000;
      if (lVar2 == 0) {
        *s_pEOF + 0 = 0x20;
        s_DataBuf + 0 = 0x20;
        s_DataBuf + 4 = 0x1b7;
        return;
      }
      goto LAB_0001041c;
    }
    uVar4 = read_volatile_4(REG_IPU_BP);
    uVar3 = read_volatile_4(REG_IPU_CTRL);
  } while( true );
}


void _MPEG_SetQM(int param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  
  iVar3 = 0x10000000;
  iVar1 = read_volatile_4(REG_IPU_CTRL);
  if (iVar1 < 0) {
    uVar2 = read_volatile_4(REG_IPU_BP);
    iVar3 = 0x10000000;
    _ipu_sync(uVar2);
  }
  *(uint *)(iVar3 + 0x2000) = param_1 << 0x1b | 0x50000000;
  s_DataBuf = 0;
  return;
}


void _MPEG_SetQSTIVFAS(void)

{
  int in_at_lo;
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  uVar1 = _MPEG_GetBits(1);
  uVar2 = _MPEG_GetBits(1);
  uVar3 = _MPEG_GetBits(1);
  *(uint *)(in_at_lo + 0x2010) =
       *(uint *)(in_at_lo + 0x2010) & 0xff8fffff | uVar1 << 0x16 | uVar2 << 0x15 | uVar3 << 0x14;
  return;
}


uint _MPEG_ShowBits(uint param_1)

{
  int iVar1;
  uint uVar2;
  
  if (s_DataBuf + 0 < (int)param_1) {
    iVar1 = read_volatile_4(REG_IPU_CTRL);
    if (iVar1 < 0) {
      uVar2 = read_volatile_4(REG_IPU_BP);
      _ipu_sync(uVar2);
    }
    write_volatile_4(REG_IPU_CMD,0x40000000);
    s_DataBuf + 4 = _ipu_sync_data();
    s_DataBuf + 0 = 0x20;
  }
  return s_DataBuf + 4 >> (-param_1 & 0x1f);
}


void _MPEG_Suspend(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  do {
  } while (s_CSCFlag != '\0');
  do {
    DI();
    SYNC(0x10);
  } while ((Status & 0x10000) != 0);
  uVar2 = read_volatile_4(REG_DMAC_ENABLER);
  write_volatile_4(REG_DMAC_ENABLEW,uVar2 | 0x10000);
  uVar2 = read_volatile_4(REG_DMAC_4_IPU_TO_CHCR);
  write_volatile_4(REG_DMAC_4_IPU_TO_CHCR,uVar2 & 0xfffffeff);
  uVar1 = read_volatile_4(REG_DMAC_ENABLER);
  write_volatile_4(REG_DMAC_ENABLEW,uVar1 & 0xfffeffff);
  EI();
  s_IPUState + 4 = read_volatile_4(REG_DMAC_4_IPU_TO_MADR);
  s_IPUState + 8 = read_volatile_4(REG_DMAC_4_IPU_TO_QWC);
  do {
    uVar1 = read_volatile_4(REG_IPU_CTRL);
  } while ((uVar1 & 0xf0) != 0);
  do {
    DI();
    SYNC(0x10);
  } while ((Status & 0x10000) != 0);
  uVar1 = read_volatile_4(REG_DMAC_ENABLER);
  write_volatile_4(REG_DMAC_ENABLEW,uVar1 | 0x10000);
  uVar1 = read_volatile_4(REG_DMAC_3_IPU_FROM_CHCR);
  write_volatile_4(REG_DMAC_3_IPU_FROM_CHCR,uVar1 & 0xfffffeff);
  uVar3 = read_volatile_4(REG_DMAC_ENABLER);
  write_volatile_4(REG_DMAC_ENABLEW,uVar3 & 0xfffeffff);
  EI();
  s_IPUState + 16 = read_volatile_4(REG_DMAC_3_IPU_FROM_MADR);
  s_IPUState + 20 = read_volatile_4(REG_DMAC_3_IPU_FROM_QWC);
  s_IPUState + 24 = read_volatile_4(REG_IPU_CTRL);
  s_IPUState + 28 = read_volatile_4(REG_IPU_BP);
  s_IPUState + 0 = uVar2 & 0xfffffeff;
  s_IPUState + 12 = uVar1 & 0xfffffeff;
  return;
}


int _MPEG_WaitBDEC(void)

{
  int iVar1;
  int iVar2;
  undefined8 uVar3;
  int extraout_a0_lo;
  undefined4 extraout_t0_lo;
  int extraout_t1_lo;
  
  uVar3 = 0x10000000;
  iVar2 = read_volatile_4(REG_IPU_CTRL);
  while( true ) {
    if (iVar2 < 0) {
      _ipu_sync(*(uint *)((int)uVar3 + 0x2020));
    }
    iVar2 = (int)uVar3;
    if ((*s_pEOF != 0) ||
       (iVar1 = read_volatile_4(REG_DMAC_3_IPU_FROM_QWC), (*(uint *)(iVar2 + 0x2010) & 0x4000) != 0)
       ) break;
    if (iVar1 == 0) {
      s_DataBuf = CONCAT44((int)*(undefined8 *)(iVar2 + 0x2030),0x20);
      return 1;
    }
    iVar2 = *(int *)(iVar2 + 0x2010);
  }
  _ipu_suspend();
  *(undefined4 *)(extraout_t1_lo + 0x2010) = extraout_t0_lo;
  _MPEG_Resume();
  do {
    DI();
    SYNC(0x10);
  } while ((Status & 0x10000) != 0);
  *(uint *)(extraout_a0_lo + -0xa70) = *(uint *)(extraout_a0_lo + -0xae0) | 0x10000;
  *(undefined4 *)(extraout_a0_lo + -0x5000) = 0;
  *(uint *)(extraout_a0_lo + -0xa70) = *(uint *)(extraout_a0_lo + -0xae0) & 0xfffeffff;
  EI();
  *(undefined4 *)(extraout_a0_lo + -0x4fe0) = 0;
  s_DataBuf = 0;
  return 0;
}


undefined _MPEG_Destroy(void)

{
  do {
  } while (s_CSCFlag != '\0');
  syscall(0);
  syscall(0);
  return 0;
}

