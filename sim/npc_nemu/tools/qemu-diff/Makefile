NAME  = $(GUEST_ISA)-qemu
SRCS  = $(shell find src/ -name "*.c")

SHARE = 1
CFLAGS += -DNEMU_HOME=\"$(SIM_NPC_NEMU)\" -DCONFIG_ISA_$(GUEST_ISA)
INC_PATH += $(SIM_NPC_NEMU)/include

include $(SIM_NPC_NEMU)/scripts/build.mk
