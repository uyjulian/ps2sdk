
#ifndef IPSTACK_H
#define IPSTACK_H

#ifdef BUILDING_SMAP_NETMAN
#include <netman.h>
#endif
#ifdef BUILDING_SMAP_PS2IP
#include <ps2ip.h>
#endif
#ifdef SMAP_ENABLE_UDPBD
#include "udpbd.h"
#include "ministack.h"
#endif

static inline int SMAPCommonTxPacketNext(void **data)
{
#ifdef SMAP_ENABLE_UDPBD
	{
		int buf_size;
		buf_size = smap_ministack_tx_get(data);
		if (buf_size != 0)
		{
			return buf_size;
		}
	}
#endif
#if defined(BUILDING_SMAP_NETMAN)
    return NetManTxPacketNext(data);
#elif defined(BUILDING_SMAP_PS2IP)
    return SMapTxPacketNext(data);
#else
    return 0;
#endif
}

static inline void SMAPCommonTxPacketDeQ(void **data)
{
	(void)data;
#ifdef SMAP_ENABLE_UDPBD
	if (smap_ministack_tx_dequeue(data)) {
		return;
	}
#endif
#if defined(BUILDING_SMAP_NETMAN)
    NetManTxPacketDeQ();
#elif defined(BUILDING_SMAP_PS2IP)
    SMapTxPacketDeQ();
#endif
}

static inline void SMapCommonLinkStateDown(struct SmapDriverData *SmapDrivPrivData)
{
#if defined(BUILDING_SMAP_NETMAN)
    NetManToggleNetIFLinkState(SmapDrivPrivData->NetIFID, NETMAN_NETIF_ETH_LINK_STATE_DOWN);
#elif defined(BUILDING_SMAP_PS2IP)
    (void)SmapDrivPrivData;

    PS2IPLinkStateDown();
#else
    (void)SmapDrivPrivData;
#endif
}

static inline void SMapCommonLinkStateUp(struct SmapDriverData *SmapDrivPrivData)
{
#if defined(BUILDING_SMAP_NETMAN)
    NetManToggleNetIFLinkState(SmapDrivPrivData->NetIFID, NETMAN_NETIF_ETH_LINK_STATE_UP);
#elif defined(BUILDING_SMAP_PS2IP)
    (void)SmapDrivPrivData;

    PS2IPLinkStateUp();
#else
    (void)SmapDrivPrivData;
#endif
#ifdef SMAP_ENABLE_UDPBD
    udpbd_init();
#endif
}

static inline void *SMapCommonStackAllocRxPacket(u16 LengthRounded, void **payload)
{
	void *pbuf;
#if defined(BUILDING_SMAP_NETMAN)
	pbuf = NetManNetProtStackAllocRxPacket(LengthRounded, payload);
#elif defined(BUILDING_SMAP_PS2IP)
	struct pbuf *pbuf_struct;

	pbuf_struct = pbuf_alloc(PBUF_RAW, LengthRounded, PBUF_POOL);
	pbuf = (void *)pbuf_struct;

	if (pbuf_struct != NULL && payload != NULL) {
		*payload = pbuf_struct->payload;
	}
#else
	(void)LengthRounded;
	if (payload != NULL)
	{
		*payload = NULL;
	}
	pbuf = NULL;
#endif
	return pbuf;
}

static inline void SMapStackEnQRxPacket(void *pbuf)
{
#if defined(BUILDING_SMAP_NETMAN)
	NetManNetProtStackEnQRxPacket(pbuf);
#elif defined(BUILDING_SMAP_PS2IP)
	SMapLowLevelInput(pbuf);
#else
	(void)pbuf;
#endif
}

#endif
