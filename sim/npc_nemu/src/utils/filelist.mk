ifdef CONFIG_ITRACE
CXXSRC += $(SIM_NPC_NEMU)/src/utils/disasm.cc

# +CXXFLAGS += $(shell llvm-config --cxxflags) -fPIE
# +LIBS += $(shell llvm-config --libs)

 CXXFLAGS += $(shell llvm-config-13 --cxxflags) -fPIE
 LIBS += $(shell llvm-config-13 --libs)
endif
