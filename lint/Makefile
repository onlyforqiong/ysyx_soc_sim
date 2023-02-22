# modify to your ID. DO NOT add any trailing space!!!
ID = 220886

TOP = ysyx_$(ID)
V_FILE = $(TOP).v 
# V_FILE += ysyxSoCFull.v
V_FILE += ../peripheral/spiFlash/spiFlash.v
# V_FILE += $(YSYX_SIM_HOME)/difftest_lib.v
V_OTHER_PATH = ../riscv64_ysyx/$(TOP).v
RAM_FILE = ../ram/S011HD1P_X32Y2D128_BW.v

LINT_FLAGS = -Wall -Wno-DECLFILENAME

INC_PATH = ../peripheral/uart16550/rtl 
INC_PATH += ../peripheral/spi/rtl

INCFLAGS = $(addprefix -I, $(INC_PATH))

# lint without UNUSED
V_FILE : $(V_OTHER_PATH)
	cp ../riscv64_ysyx/$(TOP).v ./

lint: $(V_FILE) $(RAM_FILE)
	verilator --lint-only --timescale "1ns/1ns"  $(INCFLAGS)  --top-module $(TOP) $^  $(LINT_FLAGS) -Wno-UNUSED 

# lint with UNUSED
lint-unused: $(V_FILE) $(RAM_FILE)
	verilator --lint-only --timescale "1ns/1ns" --top-module $(TOP)  $^  $(LINT_FLAGS)

.PHONY: lint lint-unused
