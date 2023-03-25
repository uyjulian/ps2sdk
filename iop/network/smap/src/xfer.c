
#include <errno.h>
#include <stdio.h>
#include <dmacman.h>
#include <dev9.h>
#include <intrman.h>
#include <loadcore.h>
#include <modload.h>
#include <stdio.h>
#include <sysclib.h>
#include <thbase.h>
#include <thevent.h>
#include <thsemap.h>
#include <irx.h>

#include <smapregs.h>
#include <speedregs.h>

#include "main.h"

#include "xfer.h"
#include "ipstack.h"

static int SmapDmaTransfer(volatile u8 *smap_regbase, void *buffer, unsigned int size, int direction)
{
    unsigned int NumBlocks;
    int result;

    (void)smap_regbase;

    /*  Non-Sony: the original block size was (32*4 = 128) bytes.
        However, that resulted in slightly lower performance due to the IOP needing to copy more data.    */
    if ((NumBlocks = size >> 6) > 0) {
        if (dev9DmaTransfer(1, buffer, NumBlocks << 16 | 0x10, direction) >= 0) {
            result = NumBlocks << 6;
        } else
            result = 0;
    } else
        result = 0;

    return result;
}

static inline void CopyFromFIFO(volatile u8 *smap_regbase, void *buffer, unsigned int length, u16 RxBdPtr)
{
    int i, result;

    if (buffer == NULL) {
        return;
    }

    SMAP_REG16(SMAP_R_RXFIFO_RD_PTR) = RxBdPtr;

    result = SmapDmaTransfer(smap_regbase, buffer, length, DMAC_TO_MEM);

    for (i = result; (unsigned int)i < length; i += 4) {
        ((u32 *)buffer)[i / 4] = SMAP_REG32(SMAP_R_RXFIFO_DATA);
    }
}

static inline void CopyToFIFO(volatile u8 *smap_regbase, const void *buffer, unsigned int length)
{
    int i, result;

    if (buffer == NULL) {
        return;
    }

    result = SmapDmaTransfer(smap_regbase, (void *)buffer, length, DMAC_FROM_MEM);

    for (i = result; (unsigned int)i < length; i += 4) {
        SMAP_REG32(SMAP_R_TXFIFO_DATA) = ((u32 *)buffer)[i / 4];
    }
}

int HandleRxIntr(struct SmapDriverData *SmapDrivPrivData)
{
    USE_SMAP_RX_BD;
    int NumPacketsReceived, i;
    volatile smap_bd_t *PktBdPtr;
    volatile u8 *smap_regbase;
    u16 ctrl_stat, length, pointer, LengthRounded;

    smap_regbase = SmapDrivPrivData->smap_regbase;

    NumPacketsReceived = 0;

    /*  Non-Sony: Workaround for the hardware BUG whereby the Rx FIFO of the MAL becomes unresponsive or loses frames when under load.
        Check that there are frames to process, before accessing the BD registers. */
    while (SMAP_REG8(SMAP_R_RXFIFO_FRAME_CNT) > 0) {
        PktBdPtr  = &rx_bd[SmapDrivPrivData->RxBDIndex % SMAP_BD_MAX_ENTRY];
        ctrl_stat = PktBdPtr->ctrl_stat;
        if (!(ctrl_stat & SMAP_BD_RX_EMPTY)) {
            length        = PktBdPtr->length;
            LengthRounded = (length + 3) & ~3;
            pointer       = PktBdPtr->pointer;

            if (ctrl_stat & (SMAP_BD_RX_INRANGE | SMAP_BD_RX_OUTRANGE | SMAP_BD_RX_FRMTOOLONG | SMAP_BD_RX_BADFCS | SMAP_BD_RX_ALIGNERR | SMAP_BD_RX_SHORTEVNT | SMAP_BD_RX_RUNTFRM | SMAP_BD_RX_OVERRUN)) {
                for (i = 0; i < 16; i++)
                    if ((ctrl_stat >> i) & 1)
                        SmapDrivPrivData->RuntimeStats.RxErrorCount++;

                SmapDrivPrivData->RuntimeStats.RxDroppedFrameCount++;

                if (ctrl_stat & SMAP_BD_RX_OVERRUN)
                    SmapDrivPrivData->RuntimeStats.RxFrameOverrunCount++;
                if (ctrl_stat & (SMAP_BD_RX_INRANGE | SMAP_BD_RX_OUTRANGE | SMAP_BD_RX_FRMTOOLONG | SMAP_BD_RX_SHORTEVNT | SMAP_BD_RX_RUNTFRM))
                    SmapDrivPrivData->RuntimeStats.RxFrameBadLengthCount++;
                if (ctrl_stat & SMAP_BD_RX_BADFCS)
                    SmapDrivPrivData->RuntimeStats.RxFrameBadFCSCount++;
                if (ctrl_stat & SMAP_BD_RX_ALIGNERR)
                    SmapDrivPrivData->RuntimeStats.RxFrameBadAlignmentCount++;

                // Original did this whenever a frame is dropped.
                SMAP_REG16(SMAP_R_RXFIFO_RD_PTR) = pointer + LengthRounded;
            } else {
                void *pbuf, *payload;

                if ((pbuf = SMapCommonStackAllocRxPacket(SmapDrivPrivData, LengthRounded, &payload, pointer)) != NULL) {
                    CopyFromFIFO(SmapDrivPrivData->smap_regbase, payload, length, pointer);
                    SMapStackEnQRxPacket(SmapDrivPrivData, pbuf);
                    NumPacketsReceived++;
                } else {
                    SmapDrivPrivData->RuntimeStats.RxAllocFail++;
                    // Original did this whenever a frame is dropped.
                    SMAP_REG16(SMAP_R_RXFIFO_RD_PTR) = pointer + LengthRounded;
                }
            }

            SMAP_REG8(SMAP_R_RXFIFO_FRAME_DEC) = 0;
            PktBdPtr->ctrl_stat                = SMAP_BD_RX_EMPTY;
            SmapDrivPrivData->RxBDIndex++;
        } else
            break;
    }

    return NumPacketsReceived;
}

static int tx_sema = -1;
static int tx_completion_ef = -1;
static const void **tx_datas = NULL;
static const u16 *tx_data_sizes = NULL;
static int tx_data_count = 0;

void xfer_init(void)
{
    iop_sema_t sema_info;

    sema_info.attr    = 0;
    sema_info.initial = 1; /* Unlocked.  */
    sema_info.max     = 1;
    if (tx_sema <= 0)
        tx_sema = CreateSema(&sema_info);

    iop_event_t event_info;

    event_info.attr   = 0;
    event_info.option = 0;
    event_info.bits   = 0;
    if (tx_completion_ef <= 0)
        tx_completion_ef = CreateEventFlag(&event_info);
}

int HandleTxReqs(struct SmapDriverData *SmapDrivPrivData)
{
    int result;
    void *data;
    USE_SMAP_TX_BD;
    volatile smap_bd_t *BD_ptr;
    u16 BD_data_ptr;
    unsigned int SizeRounded;
    const void **my_datas;
    const u16 *my_data_sizes;
    int my_data_count;
    int my_data_sent;

    result = 0;
    my_datas = tx_datas;
    my_data_sizes = tx_data_sizes;
    my_data_count = tx_data_count;
    my_data_sent = 0;
    while (1) {
        int length;

        if (my_data_count != 0 && my_data_sent != 1)
        {
            int i;

            length = 0;
            for (i = 0; i < my_data_count; i += 1)
            {
                length += my_data_sizes[i];
            }
        }
        else
        {
            data = NULL;
            if ((length = SMAPCommonTxPacketNext(SmapDrivPrivData, &data)) < 1) {
                break;
            }
            SmapDrivPrivData->packetToSend = data;
        }


        if (SmapDrivPrivData->NumPacketsInTx < SMAP_BD_MAX_ENTRY) {
            {
                SizeRounded = (length + 3) & ~3;

                if (SmapDrivPrivData->TxBufferSpaceAvailable >= SizeRounded) {
                    volatile u8 *smap_regbase;

                    smap_regbase = SmapDrivPrivData->smap_regbase;

                    BD_data_ptr = SMAP_REG16(SMAP_R_TXFIFO_WR_PTR) + SMAP_TX_BASE;
                    BD_ptr      = &tx_bd[SmapDrivPrivData->TxBDIndex % SMAP_BD_MAX_ENTRY];

                    if (my_data_count != 0 && my_data_sent != 1)
                    {
                        int i;

                        for (i = 0; i < my_data_count; i += 1)
                        {
                            if (my_data_sizes[i] != 0)
                            {
                                CopyToFIFO(SmapDrivPrivData->smap_regbase, my_datas[i], my_data_sizes[i]);
                            }
                        }
                        my_data_sent = 1;
                        tx_datas = NULL;
                        tx_data_sizes = NULL;
                        tx_data_count = 0;
                    }
                    else
                    {
                        CopyToFIFO(SmapDrivPrivData->smap_regbase, data, length);
                    }

                    result++;
                    BD_ptr->length                     = length;
                    BD_ptr->pointer                    = BD_data_ptr;
                    SMAP_REG8(SMAP_R_TXFIFO_FRAME_INC) = 0;
                    BD_ptr->ctrl_stat                  = SMAP_BD_TX_READY | SMAP_BD_TX_GENFCS | SMAP_BD_TX_GENPAD;
                    SmapDrivPrivData->TxBDIndex++;
                    SmapDrivPrivData->NumPacketsInTx++;
                    SmapDrivPrivData->TxBufferSpaceAvailable -= SizeRounded;
                } else
                    break; // Out of FIFO space
            }
        } else
            break; // Queue full

        SmapDrivPrivData->packetToSend = NULL;
        SMAPCommonTxPacketDeQ(SmapDrivPrivData, &data);
    }
    if (my_data_sent)
    {
        tx_datas = NULL;
        tx_data_sizes = NULL;
        tx_data_count = 0;
        SetEventFlag(tx_completion_ef, 1);
    }
    return result;
}

int smap_transmit(const void **datas, const u16 *data_sizes, int data_count)
{
    WaitSema(tx_sema);

    tx_datas = datas;
    tx_data_sizes = data_sizes;
    tx_data_count = data_count;

    SMAPXmit();

    {
        u32 EFBits;

        // wait for done...
        WaitEventFlag(tx_completion_ef, 1, WEF_OR | WEF_CLEAR, &EFBits);
    }

    SignalSema(tx_sema);

    return 0;
}
