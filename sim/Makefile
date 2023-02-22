ID = ysyxSoCFull
TOP = ysyxSoCFull
TOPNAME = $(TOP)
V_INC_PATH  = $(abspath ../peripheral/uart16550/rtl) 
V_INC_PATH += $(abspath ../peripheral/spi/rtl )
INC_PATH = $(abspath ./include )
INC_PATH += $(abspath ../peripheral/spiFlash/include)


VERILATOR = verilator
VERILATOR_CFLAGS += -MMD --build -cc  --timescale "1ns/1ns" \
				-O3 --x-assign fast --x-initial fast --noassert
VERILATOR_CFLAGS += --trace-fst -j 16
VERILATOR_CFLAGS += --threads 16

NOW_PLACE = $(shell pwd)

BUILD_DIR = $(NOW_PLACE)/build
OBJ_DIR = $(BUILD_DIR)/obj_dir
BIN = $(BUILD_DIR)/$(TOPNAME)
default: $(BIN)

include $(NOW_PLACE)/npc_nemu/nemu_npc.mk

$(shell mkdir -p $(BUILD_DIR))

# project source
VSRCS = $(shell find $(abspath ../riscv64_ysyx) -name "*.v")
VSRCS += $(shell find $(abspath ../ram) -name "*.v")
VSRCS += $(shell find $(abspath ../soc) -name "ysyxSoCFull.v")
VSRCS += $(shell find $(abspath ../peripheral) -name "*.v")
VSRCS += $(shell find $(abspath ./vsrc) -name "*.v")

# CSRCS = $(shell find $(abspath ./csrc) -name "*.c" -or -name "*.cc" -or -name "*.cpp")
# CSRCS += $(shell find $(abspath .) -name "main.cpp")
CSRCS += $(shell find $(abspath ../peripheral/spiFlash) -name "spiFlash.cpp")
CSRCS += $(shell find $(abspath ./src) -name "*.cpp" -or -name "*.cc")

CSRCS += $(SRCS) $(CXXSRC)
# CSRCS = $(SIM_NPC_NEMU)/nemu-main.c

# rules for lib
# INC_PATH += $(YSYX_SIM_HOME)/csrc/c_lib/include
# CSRCS += $(shell find $(abspath ./csrc/c_lib) -name "*.c" -or -name "*.cc" -or -name "*.cpp") 

# rules for verilator
INCFLAGS = $(addprefix -I, $(INC_PATH))
CFLAGS += $(INCFLAGS) -DTOP_NAME="\"V$(TOPNAME)\"" $(CXXFLAGS) -fpermissive
LDFLAGS += -lSDL2 -lSDL2_image $(LIBS) 

$(BIN): $(VSRCS) $(CSRCS)
	-rm $(SIM_NPC_NEMU)/include/generated/autoconf.h 
	-cp $(YSYX_SIM_HOME)/include/generated/autoconf.h $(NPC_NEMU_HOME)/include/generated/
	$(VERILATOR) $(VERILATOR_CFLAGS) \
		-top $(TOPNAME) $^  $(addprefix -I, $(V_INC_PATH))\
		$(addprefix -CFLAGS , $(CFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS))  \
		--Mdir $(OBJ_DIR) --exe -o $(abspath $(BIN)) 
		$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!

# run: $(BIN)
# 	@$^

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean run
