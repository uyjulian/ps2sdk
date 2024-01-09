/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

#ifndef __PS2SDK_ENDIAN_H__
#define __PS2SDK_ENDIAN_H__

#include <tamtypes.h>

/* Assume host is little endian */

#define htobe16(x) (__builtin_bswap16(x))
#define htobe32(x) (__builtin_bswap32(x))
#define htobe64(x) (__builtin_bswap64(x))

#define be16toh(x) (__builtin_bswap16(x))
#define be32toh(x) (__builtin_bswap32(x))
#define be64toh(x) (__builtin_bswap64(x))

#define htole16(x) (x)
#define htole32(x) (x)
#define htole64(x) (x)

#define le16toh(x) (x)
#define le32toh(x) (x)
#define le64toh(x) (x)

#define betoh16(x) (be16toh(x))
#define betoh32(x) (be32toh(x))
#define betoh64(x) (be64toh(x))

#define letoh16(x) (le16toh(x))
#define letoh32(x) (le32toh(x))
#define letoh64(x) (le64toh(x))

#endif
