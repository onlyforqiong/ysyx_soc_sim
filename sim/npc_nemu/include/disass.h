#ifndef __DISASS_H__
#define __DISASS_H__

#include <common.h>


extern "C" void init_disasm(const char *triple);
extern "C" void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);

#endif
