#ifndef MINISTACK_H
#define MINISTACK_H


#include "tamtypes.h"


/* These automatically convert the address and port to network order.  */
#define IP_ADDR(a, b, c, d)	(((d & 0xff) << 24) | ((c & 0xff) << 16) | \
				((b & 0xff) << 8) | ((a & 0xff)))
#define IP_PORT(port)	(((port & 0xff00) >> 8) | ((port & 0xff) << 8))

typedef struct {
	u8 addr[4];
} __attribute__((packed)) ip_addr_t;

/* Ethernet header (14 bytes) */
typedef struct {
	u8	addr_dst[6];
	u8	addr_src[6];
	u16	type;
} __attribute__((packed)) eth_header_t;

/* IP header (20 bytes) */
typedef struct {
	u8	hlen;
	u8	tos;
	u16	len;
	u16	id;
	u8	flags;
	u8	frag_offset;
	u8	ttl;
	u8	proto;
	u16	csum;
	ip_addr_t addr_src;
	ip_addr_t addr_dst;
} __attribute__((packed)) ip_header_t;

/* UDP header (8 bytes) */
typedef struct {
	u16	port_src;
	u16	port_dst;
	u16	len;
	u16	csum;
} __attribute__((packed)) udp_header_t;

typedef struct {
	eth_header_t	eth; // 14 bytes
	ip_header_t	ip;  // 20 bytes
	udp_header_t	udp; //  8 bytes
	char payload[1472]; // maximum payload
} __attribute__((packed)) udp_packet_t;


#endif
