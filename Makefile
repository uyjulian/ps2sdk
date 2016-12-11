# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# $Id$

ifeq (x$(PS2SDKSRC), x)
  export PS2SDKSRC=$(shell pwd)
endif

SUBDIRS = tools iop ee common samples

all: build
	@$(ECHO) .;
	@$(ECHO) .PS2SDK Built.;
	@$(ECHO) .;

# Common rules shared by all build targets.

.PHONY: dummy

# Use SUBDIRS to descend into subdirectories.
subdir_list  = $(patsubst %,all-%,$(SUBDIRS))
subdir_clean = $(patsubst %,clean-%,$(SUBDIRS))
subdir_release = $(patsubst %,release-%,$(SUBDIRS))
subdirs: dummy $(subdir_list)

$(subdir_list): dummy
	$(MAKEREC) $(patsubst all-%,%,$@)
$(subdir_clean): dummy
	$(MAKEREC) $(patsubst clean-%,%,$@) clean
$(subdir_release): dummy
	$(MAKEREC) $(patsubst release-%,%,$@) release


build: env_build_check $(subdir_list)

clean: env_build_check $(subdir_clean)

release-clean:
	make -C common release-clean
	make -C iop release-clean
	make -C ee release-clean
	make -C samples release-clean
	make -C tools release-clean
	
rebuild: env_build_check $(subdir_clean) $(subdir_list)

$(PS2SDKUJ)/common/include:
	$(MKDIR) -p $(PS2SDKUJ)/common
	$(MKDIR) -p $(PS2SDKUJ)/common/include
	cp -f $(PS2SDKSRC)/common/include/*.h $(PS2SDKUJ)/common/include/

$(PS2SDKUJ)/ports:
	$(MKDIR) -p $(PS2SDKUJ)/ports

install: release

release: build release_base release-clean $(PS2SDKUJ)/common/include $(PS2SDKUJ)/ports $(subdir_release)

release_base: env_release_check
	@if test ! -d $(PS2SDKUJ) ; then \
	  $(MKDIR) -p $(PS2SDKUJ) ; \
	fi
	cp -f README.md $(PS2SDKUJ)
	cp -f CHANGELOG $(PS2SDKUJ)
	cp -f AUTHORS $(PS2SDKUJ)
	cp -f LICENSE $(PS2SDKUJ)
	cp -f ID $(PS2SDKUJ)
	cp -f Defs.make $(PS2SDKUJ)

env_build_check:
	@if test -z $(PS2SDKSRC) ; \
	then \
	  $(ECHO) PS2SDKSRC environment variable should be defined. ; \
	fi

env_release_check:
	@if test -z $(PS2SDKUJ) ; \
	then \
	  $(ECHO) PS2SDKUJ environment variable must be defined. ; \
	  exit 1; \
	fi

docs:
	doxygen doxy.conf

include Defs.make
