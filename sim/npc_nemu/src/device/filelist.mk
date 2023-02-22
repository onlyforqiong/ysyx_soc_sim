DIRS-y += $(SIM_NPC_NEMU)/src/device/io
SRCS-$(CONFIG_DEVICE) += $(SIM_NPC_NEMU)/src/device/device.c $(SIM_NPC_NEMU)/src/device/alarm.c $(SIM_NPC_NEMU)/src/device/intr.c
SRCS-$(CONFIG_HAS_SERIAL) += $(SIM_NPC_NEMU)/src/device/serial.c
SRCS-$(CONFIG_HAS_TIMER) += $(SIM_NPC_NEMU)/src/device/timer.c 
SRCS-$(CONFIG_HAS_KEYBOARD) += $(SIM_NPC_NEMU)/src/device/keyboard.c
SRCS-$(CONFIG_HAS_VGA) += $(SIM_NPC_NEMU)/src/device/vga.c
SRCS-$(CONFIG_HAS_AUDIO) += $(SIM_NPC_NEMU)/src/device/audio.c
SRCS-$(CONFIG_HAS_DISK) += $(SIM_NPC_NEMU)/src/device/disk.c
SRCS-$(CONFIG_HAS_SDCARD) += $(SIM_NPC_NEMU)/src/device/sdcard.c

SRCS-BLACKLIST-$(CONFIG_TARGET_AM) += $(SIM_NPC_NEMU)/src/device/alarm.c

ifdef CONFIG_DEVICE
ifndef CONFIG_TARGET_AM
LIBS += -lSDL2
endif
endif
