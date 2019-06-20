EXTRA_CFLAGS += -include $(src)/fs_conf.h
EXTRA_CFLAGS += -DNDEBUG -g0
EXTRA_CFLAGS += -DUFSD_DEVICE=ufsd
EXTRA_CFLAGS += -Wno-unused-local-typedefs
EXTRA_CFLAGS += -DUFSD_USE_NLS=y
EXTRA_CFLAGS += -DPACKAGE_TAG='"$(shell date "+,%m-%d-%Y %H:%M:%S")"'

ifeq ($(CONFIG_CPU_BIG_ENDIAN),y)
	ENDIAN := eb
else
	ENDIAN := el
endif

MODULE_NAME := ufsd
MODULE_OBJECTS := ufsdvfs.o vfsdebug.o fool.o libufsd_$(ARCH)$(ENDIAN).o

## obj-$(CONFIG_UFSD_FS) += $(MODULE_NAME).o

$(MODULE_NAME)-y := $(MODULE_OBJECTS)

obj-m := $(MODULE_NAME).o
