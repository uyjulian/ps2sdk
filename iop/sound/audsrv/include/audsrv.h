/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2005, ps2dev - http://www.ps2dev.org
# Licenced under GNU Library General Public License version 2
*/

/**
 * @file
 * audsrv IOP server
 */

#ifndef __AUDSRV_H__
#define __AUDSRV_H__

#include <types.h>
#include <irx.h>

/** intialization and destruction functions */
#define AUDSRV_INIT                 0x0000
#define AUDSRV_QUIT                 0x0001

/** audio control functions */
#define AUDSRV_FORMAT_OK            0x0002
#define AUDSRV_SET_FORMAT           0x0003
#define AUDSRV_PLAY_AUDIO           0x0004
#define AUDSRV_WAIT_AUDIO           0x0005
#define AUDSRV_STOP_AUDIO           0x0006
#define AUDSRV_SET_VOLUME           0x0007
#define AUDSRV_SET_THRESHOLD        0x0008

/** cdrom functions */
#define AUDSRV_PLAY_CD              0x0009
#define AUDSRV_STOP_CD              0x000a
#define AUDSRV_GET_CDPOS            0x000b
#define AUDSRV_GET_TRACKPOS         0x000c
#define AUDSRV_GET_NUMTRACKS        0x000d
#define AUDSRV_GET_TRACKOFFSET      0x000e
#define AUDSRV_PAUSE_CD             0x0011
#define AUDSRV_RESUME_CD            0x0012
#define AUDSRV_PLAY_SECTORS         0x0013
#define AUDSRV_GET_CD_STATUS        0x0014
#define AUDSRV_GET_CD_TYPE          0x0015

#define AUDSRV_INIT_ADPCM           0x0016
#define AUDSRV_LOAD_ADPCM           0x0017
#define AUDSRV_PLAY_ADPCM           0x0018
#define AUDSRV_SET_ADPCM_VOL        0x0019
#define AUDSRV_FREE_ADPCM           0x001c
#define AUDSRV_IS_ADPCM_PLAYING     0x001d

#define AUDSRV_AVAILABLE            0x001a
#define AUDSRV_QUEUED               0x001b

#define AUDSRV_FILLBUF_CALLBACK     0x0001
#define AUDSRV_CDDA_CALLBACK        0x0002

/* error codes */
#define AUDSRV_ERR_NOERROR                 0x0000
#define AUDSRV_ERR_NOT_INITIALIZED         0x0001
#define AUDSRV_ERR_RPC_FAILED              0x0002
#define AUDSRV_ERR_FORMAT_NOT_SUPPORTED    0x0003
#define AUDSRV_ERR_OUT_OF_MEMORY           0x0004
#define AUDSRV_ERR_ARGS                    0x0005
#define AUDSRV_ERR_NO_DISC                 0x0006
#define AUDSRV_ERR_NO_MORE_CHANNELS        0x0007

extern int audsrv_init();
extern int audsrv_quit();

/* audio streaming functions */
extern int audsrv_format_ok(int freq, int bits, int channels);
extern int audsrv_set_format(int freq, int bits, int channels);
extern int audsrv_wait_audio(int buflen);
extern int audsrv_play_audio(const char *buf, int buflen);
extern int audsrv_stop_audio();
extern int audsrv_set_volume(int vol);
extern int audsrv_available();
extern int audsrv_queued();

/* cdda playing functions */
extern int audsrv_play_cd(int track);
extern int audsrv_stop_cd();
extern int audsrv_set_threshold(int amount);
extern int audsrv_get_cdpos();
extern int audsrv_get_trackpos();
extern int audsrv_get_numtracks();
extern int audsrv_get_track_offset(int track);
extern int audsrv_cd_pause();
extern int audsrv_cd_resume();
extern int audsrv_cd_play_sectors(int start, int end);
extern int audsrv_get_cd_status();
extern int audsrv_get_cd_type();

/* adpcm functions */
extern int audsrv_adpcm_init();
extern int audsrv_adpcm_set_volume(int ch, int voll, int volr);
extern void *audsrv_load_adpcm(u32 *buffer, int size, int id);
#define audsrv_play_adpcm(id)      audsrv_ch_play_adpcm(-1, id) //For backward-compatibility
extern int audsrv_ch_play_adpcm(int ch, u32 id);
extern int audsrv_is_adpcm_playing(int ch, u32 id);
extern int free_sample(u32 id);

#define audsrv_IMPORTS_start DECLARE_IMPORT_TABLE(audsrv, 1, 4)
#define audsrv_IMPORTS_end END_IMPORT_TABLE

#define I_audsrv_init              DECLARE_IMPORT( 4, audsrv_init)
#define I_audsrv_quit              DECLARE_IMPORT( 5, audsrv_quit)

/* audio streaming functions */
#define I_audsrv_format_ok         DECLARE_IMPORT( 6, audsrv_format_ok)
#define I_audsrv_set_format        DECLARE_IMPORT( 7, audsrv_set_format)
#define I_audsrv_wait_audio        DECLARE_IMPORT( 8, audsrv_wait_audio)
#define I_audsrv_play_audio        DECLARE_IMPORT( 9, audsrv_play_audio)
#define I_audsrv_stop_audio        DECLARE_IMPORT(10, audsrv_stop_audio)
#define I_audsrv_set_volume        DECLARE_IMPORT(11, audsrv_set_volume)

/* cdda playing functions */
#define I_audsrv_play_cd           DECLARE_IMPORT(12, audsrv_play_cd)
#define I_audsrv_stop_cd           DECLARE_IMPORT(13, audsrv_stop_cd)
#define I_audsrv_set_threshold     DECLARE_IMPORT(14, audsrv_set_threshold)
#define I_audsrv_get_cdpos         DECLARE_IMPORT(15, audsrv_get_cdpos)
#define I_audsrv_get_trackpos      DECLARE_IMPORT(16, audsrv_get_trackpos)
#define I_audsrv_get_numtracks     DECLARE_IMPORT(17, audsrv_get_numtracks)
#define I_audsrv_get_track_offset  DECLARE_IMPORT(18, audsrv_get_track_offset)
#define I_audsrv_cd_pause          DECLARE_IMPORT(19, audsrv_cd_pause)
#define I_audsrv_cd_resume         DECLARE_IMPORT(20, audsrv_cd_resume)
#define I_audsrv_cd_play_sectors   DECLARE_IMPORT(21, audsrv_cd_play_sectors)
#define I_audsrv_get_cd_status     DECLARE_IMPORT(22, audsrv_get_cd_status)
#define I_audsrv_get_cd_type       DECLARE_IMPORT(23, audsrv_get_cd_type)

#define I_audsrv_adpcm_init        DECLARE_IMPORT(24, audsrv_adpcm_init)
#define I_audsrv_load_adpcm        DECLARE_IMPORT(25, audsrv_load_adpcm)
#define I_audsrv_play_adpcm        I_audsrv_ch_play_adpcm //For backward-compatibility
#define I_audsrv_ch_play_adpcm     DECLARE_IMPORT(26, audsrv_ch_play_adpcm)
#define I_audsrv_adpcm_set_volume  DECLARE_IMPORT(27, audsrv_adpcm_set_volume)
#define I_audsrv_available         DECLARE_IMPORT(28, audsrv_available)
#define I_audsrv_queued            DECLARE_IMPORT(29, audsrv_queued)

#endif /* __AUDSRV_H__ */
