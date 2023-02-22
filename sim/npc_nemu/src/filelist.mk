SRCS-y += $(SIM_NPC_NEMU)/src/nemu-main.c
DIRS-y += $(SIM_NPC_NEMU)/src/cpu $(SIM_NPC_NEMU)/src/monitor $(SIM_NPC_NEMU)/src/utils
DIRS-$(CONFIG_MODE_SYSTEM) += $(SIM_NPC_NEMU)/src/memory
DIRS-BLACKLIST-$(CONFIG_TARGET_AM) += $(SIM_NPC_NEMU)/src/monitor/sdb

SHARE = $(if $(CONFIG_TARGET_SHARE),1,0)
LIBS += $(if $(CONFIG_TARGET_NATIVE_ELF),-lreadline -ldl -pie,)

ifdef mainargs
ASFLAGS += -DBIN_PATH=\"$(mainargs)\"
endif
SRCS-$(CONFIG_TARGET_AM) += $(SIM_NPC_NEMU)/src/am-bin.S
.PHONY: $(SIM_NPC_NEMU)/src/am-bin.S
