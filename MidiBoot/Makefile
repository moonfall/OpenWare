PROJECT = MidiBoot
BUILDROOT = .
OPENWARE ?= $(BUILDROOT)/..

LDSCRIPT = STM32F427ZI-boot.ld
include $(OPENWARE)/Hardware/owl2.mk

C_SRC = $(notdir $(wildcard Src/*.c))
CPP_SRC = $(notdir $(wildcard Src/*.cpp))

C_SRC += usbd_midi.c
C_SRC += usbd_midi_if.c
C_SRC += sdram.c
C_SRC += sysex.c
C_SRC += crc32.c
C_SRC += eepromcontrol.c
C_SRC += message.c
# S_SRC = startup_openboot.s

ifndef PLATFORM
PLATFORM = Alchemist
endif

C_SRC += usbd_desc.c usbd_conf.c

ifeq ($(PLATFORM),Magus)
  CPPFLAGS += -Iusbd-fs -DOWL_MAGUS
  vpath %.c usbd-fs
else ifeq ($(PLATFORM),Alchemist)
  CPPFLAGS += -Iusbd-hs -DOWL_ALCHEMIST -DUSE_USBD_HS
  vpath %.c usbd-hs
else ifeq ($(PLATFORM),Wizard)
  CPPFLAGS += -Iusbd-hs -DOWL_WIZARD -DUSE_USBD_HS
  vpath %.c usbd-hs
else ifeq ($(PLATFORM),EuroWizard)
  CPPFLAGS += -Iusbd-fs -DOWL_ALCHEMIST -DUSE_USBD_FS
  vpath %.c usbd-fs
else ifeq ($(PLATFORM),Rack)
  CPPFLAGS += -Iusbd-hs -DOWL_RACK
  vpath %.c usbd-hs
else ifeq ($(PLATFORM),Noctua)
  CPPFLAGS += -Iusbd-fs -DOWL_NOCTUA
  vpath %.c usbd-fs
else ifeq ($(PLATFORM),BioSignals)
  CPPFLAGS += -Iusbd-hs -DOWL_BIOSIGNALS
  vpath %.c usbd-hs
endif

# CPP_SRC += MidiReader.cpp
CPP_SRC += MidiController.cpp

OBJS = $(C_SRC:%.c=Build/%.o)
OBJS += $(CPP_SRC:%.cpp=Build/%.o)
OBJS += $(S_SRC:%.s=Build/%.o)

OBJS += $(OBJS_SDRAM)
OBJS += $(OBJS_USBD)
OBJS += $(OBJS_UART)

include $(OPENWARE)/Hardware/f4.mk

.PHONY: clean deploy unlock erase upload lock

info:
	openocd -f openocd.cfg -c "init" -c "flash info 0" -c "exit"

unlock:
	openocd -f openocd.cfg -c "init" -c "halt" -c "flash protect 0 0 11 off" -c "flash info 0" -c "exit"

erase:
	openocd -f openocd.cfg -c "init" -c "halt" -c "stm32f2x mass_erase 0" -c "flash info 0" -c "exit"

erase-boot: # flash erase_sector num first last
	openocd -f openocd.cfg -c "init" -c "halt" -c "flash erase_sector 0 0 3" -c "exit"

erase-storage: # flash erase_sector num first last
	openocd -f openocd.cfg -c "init" -c "halt" -c "flash erase_sector 0 7 11" -c "exit"

dump: # flash read_bank num filename [offset [length]]
	openocd -f openocd.cfg -c "init" -c "halt" -c "flash read_bank 0 dump.bin" -c "exit"

upload:
	openocd -f openocd.cfg -c "program Build/MidiBoot.elf verify reset exit"

lock:
	openocd -f openocd.cfg -c "init" -c "halt" -c "flash protect 0 0 1 on" -c "flash info 0" -c "exit"

deploy: upload lock
