#include <errno.h>
#include <bdm.h>
#include <thevent.h>
#include <smapregs.h>
#include <dmacman.h>
#include <dev9.h>
#include "xfer.h"
#include "udpbd.h"

//#define M_PRINTF(format, args...) printf("UDPBD: " format, ## args)
//#define M_DEBUG M_PRINTF
#define M_DEBUG(...)

#define UDPBD_MAX_RETRIES 4
static struct block_device g_udpbd;
static udpbd_pkt_t g_pkt;
static u8 g_cmdid = 0;
static int g_read_done = 0;
static int g_read_cmdpkt = 0;
static int bdm_connected = 0;

static void* g_buffer;
static unsigned int g_read_size;


static inline u16 htons(u16 n)
{
	return ((n & 0xff) << 8) | ((n & 0xff00) >> 8);
}

static u16 checksum(void *buf, size_t size)
{
	u16 *data = (u16 *)buf;
	u32 csum;

	for (csum = 0; size > 1; size -= 2, data++)
		csum += *data;

	if (size == 1)
		csum += htons((u16)((*(u8 *)data) & 0xff) << 8);

	csum = (csum >> 16) + (csum & 0xffff);
	if (csum & 0xffff0000)
		csum = (csum >> 16) + (csum & 0xffff);

	return (u16)csum;
}

static int udpbd_send(udpbd_pkt_t *udp_pkt)
{
	size_t size = 16+2; /* Size of data inside UDP packet */
	size_t pktsize = sizeof(udpbd_pkt_t);
	u32 csum;

	M_DEBUG("%s\n", __func__);

	udp_pkt->ip_len = htons(pktsize - 14);	/* Subtract the ethernet header.  */
	udp_pkt->ip_csum = 0;
	csum = checksum(&udp_pkt->ip_hlen, 20);	/* Checksum the IP header (20 bytes).  */
	while (csum >> 16)
		csum = (csum & 0xffff) + (csum >> 16);
	udp_pkt->ip_csum = ~(csum & 0xffff);
	udp_pkt->udp_len = htons(size + 8);
	udp_pkt->udp_csum = 0;			/* Don't bother.  */

	return smap_transmit(udp_pkt, pktsize);
}

//
// Block device interface
//
static int _udpbd_read(struct block_device* bd, u32 sector, void* buffer, u16 count)
{
	u32 EFBits;

	//M_DEBUG("%s: sector=%d, count=%d\n", __func__, sector, count);

    g_cmdid++;
    g_buffer = buffer;
    g_read_size = count * 512;
    g_read_cmdpkt = 1; // First reply packet should be cmdpkt==1

    g_pkt.bd.magic  = UDPBD_HEADER_MAGIC;
    g_pkt.bd.cmd    = UDPBD_CMD_READ;
    g_pkt.bd.cmdid  = g_cmdid;
    g_pkt.bd.cmdpkt = 0;
    g_pkt.bd.count  = count;
    g_pkt.bd.par1   = sector;
    g_pkt.bd.par2   = 0;

    udpbd_send(&g_pkt);

    //wait for data...
    WaitEventFlag(g_read_done, 2|1, WEF_OR|WEF_CLEAR, &EFBits);

    g_buffer = NULL;
    g_read_size = 0;
    g_read_cmdpkt = 0;

    if (EFBits & 1) { // done
        return count;
    }

	return -EIO;
}

static int udpbd_read(struct block_device* bd, u32 sector, void* buffer, u16 count)
{
	int retries;
	u16 count_left = count;

	M_DEBUG("%s: sector=%d, count=%d\n", __func__, sector, count);

	if (bdm_connected == 0)
        return -EIO;

	while (count_left > 0) {
        u16 count_block = count_left > UDPBD_MAX_SECTOR_READ ? UDPBD_MAX_SECTOR_READ : count_left;

        for (retries = 0; retries < UDPBD_MAX_RETRIES; retries++) {
            if (_udpbd_read(bd, sector, buffer, count_block) == count_block)
                break;
        }

        if (retries == UDPBD_MAX_RETRIES) {
#ifndef NO_BDM
            bdm_disconnect_bd(&g_udpbd);
#endif
            bdm_connected = 0;
            return -EIO;
        }

        count_left -= count_block;
        sector += count_block;
	}

	return count;
}

static int udpbd_write(struct block_device* bd, u32 sector, const void* buffer, u16 count)
{
	return -EIO;
}

static void udpbd_flush(struct block_device* bd)
{
    // Dummy function
}

//
// Public functions
//
int udpbd_init(void)
{
	iop_event_t EventFlagData;

	//M_DEBUG("%s\n", __func__);

	EventFlagData.attr=0;
	EventFlagData.option=0;
	EventFlagData.bits=0;
	if (g_read_done <= 0)
        g_read_done = CreateEventFlag(&EventFlagData);

    g_udpbd.name  = "udp";
    g_udpbd.devNr = 0;
    g_udpbd.parNr = 0;
    g_udpbd.sectorOffset = 0;
    g_udpbd.priv  = NULL;
    g_udpbd.read  = udpbd_read;
    g_udpbd.write = udpbd_write;
    g_udpbd.flush = udpbd_flush;

	// Ethernet
	g_pkt.eth_addr_dst[0] = 0xff; //0x54;
	g_pkt.eth_addr_dst[1] = 0xff; //0x9f;
	g_pkt.eth_addr_dst[2] = 0xff; //0x35;
	g_pkt.eth_addr_dst[3] = 0xff; //0x1e;
	g_pkt.eth_addr_dst[4] = 0xff; //0x28;
	g_pkt.eth_addr_dst[5] = 0xff; //0xa6;
	g_pkt.eth_addr_src[0] = 0x00;
	g_pkt.eth_addr_src[1] = 0x1d;
	g_pkt.eth_addr_src[2] = 0x0d;
	g_pkt.eth_addr_src[3] = 0xda;
	g_pkt.eth_addr_src[4] = 0xbf;
	g_pkt.eth_addr_src[5] = 0xca;
	g_pkt.eth_type        = 0x0008; /* Network byte order: 0x800 */
	// IP
	g_pkt.ip_hlen         = 0x45;
	g_pkt.ip_tos          = 0;
	//g_pkt.ip_len          = ;
	g_pkt.ip_id           = 0;
	g_pkt.ip_flags        = 0;
	g_pkt.ip_frag_offset  = 0;
	g_pkt.ip_ttl          = 64;
	g_pkt.ip_proto        = 0x11;
	//g_pkt.ip_csum         = ;
	g_pkt.ip_addr_src.addr[0] = 192;
	g_pkt.ip_addr_src.addr[1] = 168;
	g_pkt.ip_addr_src.addr[2] = 1;
	g_pkt.ip_addr_src.addr[3] = 10;
	g_pkt.ip_addr_dst.addr[0] = 255; //192;
	g_pkt.ip_addr_dst.addr[1] = 255; //168;
	g_pkt.ip_addr_dst.addr[2] = 255; //1;
	g_pkt.ip_addr_dst.addr[3] = 255; //198;
	// UDP
	g_pkt.udp_port_src    = IP_PORT(UDPBD_PORT);
	g_pkt.udp_port_dst    = IP_PORT(UDPBD_PORT);
	//g_pkt.udp_len         = ;
	//g_pkt.udp_csum        = ;

    // Broadcast request for block device information
    g_pkt.bd.magic  = UDPBD_HEADER_MAGIC;
    g_pkt.bd.cmd    = UDPBD_CMD_INFO;
    g_pkt.bd.cmdid  = g_cmdid;
    g_pkt.bd.cmdpkt = 0;
    g_pkt.bd.count  = 0;
    g_pkt.bd.par1   = 0;
    g_pkt.bd.par2   = 0;
    udpbd_send(&g_pkt);

	return 0;
}

extern struct SmapDriverData SmapDriverData;
void udpbd_rx(u16 pointer)
{
	USE_SMAP_REGS;
	udpbd_header_t hdr;
	u32 * phdr = (u32 *)&hdr;

	//phdr[0] = UDPBD_HEADER_MAGIC;
	phdr[1] = SMAP_REG32(SMAP_R_RXFIFO_DATA); // CMD
	phdr[2] = SMAP_REG32(SMAP_R_RXFIFO_DATA); // PAR1
	phdr[3] = SMAP_REG32(SMAP_R_RXFIFO_DATA); // PAR2

	switch (hdr.cmd) {
		case UDPBD_CMD_INFO:
            if (bdm_connected == 0) {
                g_udpbd.sectorSize  = hdr.par1;
                g_udpbd.sectorCount = hdr.par2;
#ifndef NO_BDM
                bdm_connect_bd(&g_udpbd);
#endif
                bdm_connected = 1;
            }
			break;
		case UDPBD_CMD_READ:
			if ((g_buffer != NULL) && (g_read_size >= hdr.par1) && (g_cmdid == hdr.cmdid)) {
				// Validate packet order
				if (hdr.cmdpkt != g_read_cmdpkt) {
					SmapDriverData.RuntimeStats.RxFrameOverrunCount++;
					// Error, wakeup caller
					g_read_size = 0;
					SetEventFlag(g_read_done, 2);
					break;
				}
				g_read_cmdpkt++;

				// Validate packet data size
				if ((hdr.par1 > UDPBD_MAX_DATA) || (hdr.par1 & 127)) {
					SmapDriverData.RuntimeStats.RxFrameBadLengthCount++;
					// Error, wakeup caller
					g_read_size = 0;
					SetEventFlag(g_read_done, 2);
					break;
				}

				// Directly DMA the packet data into the user buffer
				dev9DmaTransfer(1, (u8*)g_buffer + ((hdr.cmdpkt-1) * UDPBD_MAX_DATA), (hdr.par1>>7)<<16|0x20, DMAC_TO_MEM);

				g_read_size -= hdr.par1;
				if (g_read_size == 0) {
					// Done, wakeup caller
					SetEventFlag(g_read_done, 1);
					break;
				}
			}
			else {
				SmapDriverData.RuntimeStats.RxFrameBadFCSCount++;
			}
			break;
		case UDPBD_CMD_WRITE:
			break;
	};
}

