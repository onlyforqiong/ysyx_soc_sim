#ifndef __MEMORY_PADDR_H__
#define __MEMORY_PADDR_H__

#include <common.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RESET_VECTOR (CONFIG_MBASE + CONFIG_PC_RESET_OFFSET)

#define mem_inst_fec 1
#define mem_others   0


/* convert the guest physical address in the guest program to host virtual address in NEMU */
uint8_t* guest_to_host(paddr_t paddr);
/* convert the host virtual address in NEMU to guest physical address in the guest program */
paddr_t host_to_guest(uint8_t *haddr);

static inline bool in_pmem(paddr_t addr) {
  return (addr >= CONFIG_MBASE) && (addr < (paddr_t)CONFIG_MBASE + CONFIG_MSIZE);
}

word_t paddr_read(paddr_t addr, int len ,uint8_t mem_type,int skip);
void paddr_write(paddr_t addr, int len, word_t data,int skip);

typedef struct {
  size_t debug_mem_size;
  int debug_mem_write_state_get;
  size_t debug_mem_addr ;
  size_t debug_mem_pc;
  size_t debug_mem_data;
  int debug_mem_state;
  int debug_mem_cache;
}mem_debug_handle ;
extern mem_debug_handle mtrace_debuger;
#ifdef CONFIG_ITRACE_COND
void mtrace_printf();

#endif

#ifdef __cplusplus
}
#endif
#endif
