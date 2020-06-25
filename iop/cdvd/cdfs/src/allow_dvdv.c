#include <ioman.h>
#include <sysclib.h>
#include <secrman.h>

/**
 * @file
 * DVD video disc reading enabler for PS2 retail models and DESR consoles
 */

/** Buffer for the encrypted stub module to be read by the MECHACON */
static u8 buf[137] __attribute((aligned(16)));

/** Path to the file determining if the ROM is that of a DESR console */
static u8 psxver[11] __attribute((aligned(16))) = "rom0:PSXVER";

/** Header of the encrypted stub module */
static u8 Header[32] __attribute((aligned(16))) = {0x01, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x4A, 0x00, 0x01, 0x02, 0x19, 0x00, 0x00, 0x00, 0x56,
                                                   0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x07, 0x2C, 0x02, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00};


/** First part of the encrypted stub module for PS2 retail models */
static u8 ps2Part1[8] __attribute((aligned(16))) = {0xFA, 0xA2, 0x47, 0xE9, 0xFA, 0x20, 0x89, 0x19};
/** Second part of the encrypted stub module for PS2 retail models */
static u8 ps2Part2[49] __attribute((aligned(16))) = {
    0x25, 0xD1, 0x26, 0x8E, 0x3E, 0xB5, 0x38, 0xC2,
    0x87, 0xC0, 0xA9, 0xD6, 0xD5, 0x7B, 0x56, 0x87, 0x7B, 0x95, 0x6E, 0x17, 0xF6, 0xBD, 0x76, 0x51,
    0xB8, 0x5D, 0x0D, 0x6F, 0x3C, 0x7F, 0x51, 0x5A, 0xC2, 0xC7, 0x53, 0x3B, 0x46, 0x8E, 0x0C, 0x04,
    0x00, 0x94, 0xF6, 0xB2, 0x6D, 0xA8, 0x8F, 0x76, 0x96,
};


/** First part of the encrypted stub module for DESR consoles */
static u8 psxPart1[8] __attribute((aligned(16))) = {0x98, 0xF7, 0x5C, 0x84, 0x38, 0xC5, 0xBD, 0x3C};
/** Second part of the encrypted stub module for DESR consoles */
static u8 psxPart2[49] __attribute((aligned(16))) = {
    0x2F, 0xDC, 0xA6, 0x35, 0x8D, 0xE7, 0x5B, 0x77,
    0x02, 0x47, 0xD9, 0xD2, 0x6E, 0xCC, 0x4D, 0x60, 0x07, 0xBC, 0xCC, 0x1A, 0x7C, 0x55, 0x0E, 0x48,
    0x91, 0x64, 0x3B, 0xAA, 0xB9, 0x16, 0x6E, 0x09, 0x6E, 0xDD, 0xA4, 0x2B, 0x19, 0x37, 0x20, 0x79,
    0x00, 0xBF, 0x0D, 0x19, 0x40, 0xDA, 0xB1, 0x99, 0xFF,
};

/** Common portion of the encrypted stub module */
static u8 CT[16] __attribute((aligned(16))) = {
    0xDE, 0xF0, 0x76, 0x8C, 0x1B, 0x36, 0xF8, 0x07,
    0xE0, 0x4B, 0x39, 0x33, 0xF9, 0x6D, 0x4F, 0xFF,
};


void allow_dvdv(void)
{
	register int file;

	memset(buf, 0, 137);

	memcpy(buf, Header, 32);
	memcpy(buf + 48, psxver, 11);

	if ((file = open((char *)buf + 48, O_RDONLY)) <= 0)
	{
		buf[16] = 0xC0;
		buf[17] = 0xF9;

		memcpy(buf + 32, ps2Part1, 8);
		memcpy(buf + 88, ps2Part2, 49);
	}
	else
	{
		close(file);
		buf[16] = 0x90;
		buf[17] = 0xCC;
		buf[18] = 0x01;
		buf[22] = 0x01;
		memcpy(buf + 32, psxPart1, 8);
		memcpy(buf + 88, psxPart2, 49);
	}
	memcpy(buf + 72, CT, 16);

	memset(ps2Part1, 0, 8);
	memset(ps2Part2, 0, 49);
	memset(psxPart1, 0, 8);
	memset(psxPart2, 0, 49);
	memset(CT, 0, 16);

	SecrDiskBootFile(buf);

	memset(buf, 0, 137);
}
