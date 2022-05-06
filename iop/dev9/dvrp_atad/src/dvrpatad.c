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
 * ATAD shim over DVRP functions
 * This module intends to provide compatibility with the existing apa/pfs modules when using the HDD attached to the DVRP.
 */

#include <types.h>
#include <defs.h>
#include <irx.h>
#include <loadcore.h>
#include <stdio.h>
#include <sysclib.h>
#include <atad.h>
#include <iomanX.h>

#include <speedregs.h>

#include <hdd-ioctl.h>

#define MODNAME "atad"

IRX_ID(MODNAME, 2, 7);

#define M_PRINTF(format, args...) \
    printf(MODNAME ": " format, ##args)

#define BANNER  "ATAD shim over DVRP functions\n"
#define VERSION "v1.0"

static int ata_devinfo_init = 0;

/* Local device info kept for drives 0 and 1.  */
static ata_devinfo_t atad_devinfo[2];

extern struct irx_export_table _exp_atad;

int _start(int argc, char *argv[])
{
    USE_SPD_REGS;
    int res = 1;

    printf(BANNER, VERSION);

    if (!(SPD_REG16(SPD_R_REV_3) & SPD_CAPS_DVR))
    {
        M_PRINTF("DVRP not detected, exiting.\n");
        goto out;
    }

    if ((res = RegisterLibraryEntries(&_exp_atad)) != 0)
    {
        M_PRINTF("Library is already registered, exiting.\n");
        goto out;
    }

    res = 0;
    M_PRINTF("Driver loaded.\n");
out:
    return res;
}

int _exit(void)
{
    return 0;
}

/* Export 8 */
int ata_get_error(void)
{
    /* Not exposed */

    return 0;
}

/* Export 6 */
int ata_io_start(void *buf, u32 blkcount, u16 feature, u16 nsector, u16 sector, u16 lcyl, u16 hcyl, u16 select, u16 command)
{
    /* Not exposed */

    return 0;
}

/* Export 7 */
int ata_io_finish(void)
{
    /* Not exposed */

    return 0;
}

/* Export 5 */
int ata_reset_devices(void)
{
    /* Not exposed */

    return 0;
}

/* Export 17 */
int ata_device_flush_cache(int device)
{
    if (device == 0)
    {
        return devctl("dvr_hdd0:", HDIOC_FLUSH, NULL, 0, NULL, 0);  
    }

    return 0;
}

/* Export 13 */
int ata_device_idle(int device, int period)
{
#if 0
    if (device == 0)
    {
        // XXX: Do we need to byteswap for DVRP?
        int arg = period;
        return devctl("dvr_hdd0:", HDIOC_IDLE, &arg, sizeof(arg), NULL, 0);  
    }
#endif

    return 0;
}

/* Export 14 */
int ata_device_sce_identify_drive(int device, void *data)
{
    if (device == 0)
    {
        return devctl("dvr_hdd0:", HDIOC_SCEIDENTIFY, NULL, 512, data, 0);  
    }

    return 0;
}

/* Export 16 */
int ata_device_smart_save_attr(int device)
{
    /* not exposed */

    return 0;
}

/* Export 15 */
int ata_device_smart_get_status(int device)
{
    if (device == 0)
    {
        return devctl("dvr_hdd0:", HDIOC_SMARTSTAT, NULL, 0, NULL, 0);  
    }

    return 0;
}

static u8 SBUF[0x20000] __attribute__((aligned((64))));

/* Export 9 */
int ata_device_sector_io(int device, void *buf, u32 lba, u32 nsectors, int dir)
{
    int ret = ATA_RES_ERR_NODEV;
    u32 nbytes = nsectors * 512;

    if (device == 0)
    {
        // FIXME: Do we need to byteswap for DVRP?
        if (dir == ATA_DIR_READ)
        {
            hddAtaTransfer_t *args = (hddAtaTransfer_t *)SBUF;

            args->lba = lba;
            args->size = nsectors;

            ret = devctl("dvr_hdd0:", HDIOC_READSECTOR, args, sizeof(hddAtaTransfer_t), buf, nbytes);
        }
        else if (dir == ATA_DIR_WRITE)
        {
            if ((sizeof(hddAtaTransfer_t) + nbytes) > sizeof(SBUF))
            {
                return ATA_RES_ERR_IO;
            }

            hddAtaTransfer_t *args = (hddAtaTransfer_t *)SBUF;

            args->lba = lba;
            args->size = nsectors;
            memcpy(args->data, buf, nbytes);

            ret = devctl("dvr_hdd0:", HDIOC_WRITESECTOR, args, sizeof(hddAtaTransfer_t) + nbytes, NULL, 0);
        }        
    }

    return ret;
}

/* Export 10 */
int ata_device_sce_sec_set_password(int device, void *password)
{
    /* Not exposed */

    return 0;
}

/* Export 11 */
int ata_device_sce_sec_unlock(int device, void *password)
{
    /* Not exposed */

    return 0;
}

/* Export 12 */
int ata_device_sce_sec_erase(int device)
{
    /* Not exposed */

    return 0;
}

/* Export 4 */
ata_devinfo_t *ata_get_devinfo(int device)
{
    /* Not exposed */
    if (!ata_devinfo_init)
    {
        memset(atad_devinfo, 0, sizeof(atad_devinfo));
        atad_devinfo[0].exists = 1;
        atad_devinfo[0].has_packet = 0;
        atad_devinfo[0].total_sectors = devctl("dvr_hdd0:", HDIOC_GETMAXLBA48, NULL, 0, NULL, 0);
        atad_devinfo[0].security_status = 0;
        ata_devinfo_init = 1;
    }

    return &atad_devinfo[device];
}

/* Export 18 */
int ata_device_idle_immediate(int device)
{
#if 0
    if (device == 0)
    {
        return devctl("dvr_hdd0:", HDIOC_IDLEIMM, NULL, 0, NULL, 0);
    }
#endif

    return 0;
}
