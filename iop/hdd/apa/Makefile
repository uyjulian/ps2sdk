# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.

# Depend on dev9 module for poweroff callback?
APA_USE_DEV9 ?= 1

# Depend on atad module for block device interface?
APA_USE_ATAD ?= 1

# Depend on bdm module for block device interface?
APA_USE_BDM ?= 0

# Depend on iomanX module for block device interface?
APA_USE_IOMANX ?= 0

LIBAPA_PATH = $(PS2SDKSRC)/iop/hdd/libapa/

LIBAPA_SRC_DIR = $(LIBAPA_PATH)src/

IOP_BIN ?= ps2hdd.irx

IOP_CFLAGS += -DIOMANX_OLD_NAME_COMPATIBILITY=0 -DIOMANX_OLD_NAME_ADDDELDRV=0

IOP_IMPORT_INCS += \
	cdvd/cdvdman \
	system/intrman \
	system/iomanx \
	system/loadcore \
	system/stdio \
	system/sysclib \
	system/sysmem \
	system/threadman

IOP_INCS += -I$(LIBAPA_PATH)include

APA_OBJS = password.o misc.o cache.o apa.o journal.o free.o
IOP_OBJS = hdd_fio.o hdd_blkio.o hdd.o imports.o $(APA_OBJS)

ifneq (x$(APA_USE_DEV9),x0)
IOP_CFLAGS += -DAPA_USE_DEV9
IOP_IMPORT_INCS += dev9/dev9
endif

ifneq (x$(APA_USE_ATAD),x0)
IOP_CFLAGS += -DAPA_USE_ATAD
IOP_IMPORT_INCS += dev9/atad
endif

ifneq (x$(APA_USE_BDM),x0)
IOP_CFLAGS += -DAPA_USE_BDM
IOP_IMPORT_INCS += fs/bdm
endif

ifneq (x$(APA_USE_IOMANX),x0)
IOP_CFLAGS += -DAPA_USE_IOMANX
endif

include $(PS2SDKSRC)/Defs.make
include $(PS2SDKSRC)/iop/Rules.bin.make
include $(PS2SDKSRC)/iop/Rules.make
include $(PS2SDKSRC)/iop/Rules.release

$(IOP_OBJS_DIR)%.o: $(LIBAPA_SRC_DIR)%.c
	$(IOP_CC) $(IOP_CFLAGS) -c $< -o $@
