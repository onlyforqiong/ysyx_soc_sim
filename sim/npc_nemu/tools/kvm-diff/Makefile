NAME  = x86-kvm
SRCS  = $(shell find src/ -name "*.c")

SHARE = 1
INC_PATH += $(SIM_NPC_NEMU)/include $(SIM_NPC_NEMU)/src/isa/x86/include
GUEST_ISA = x86

include $(SIM_NPC_NEMU)/scripts/build.mk
