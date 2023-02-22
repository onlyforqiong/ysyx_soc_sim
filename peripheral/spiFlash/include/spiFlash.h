#ifndef __SPI_FLSH_HPP__
#define __SPI_FLSH_HPP__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <cstring>
#include <svdpi.h>

extern "C" void flash_read(uint64_t addr, uint64_t *data);
extern "C" void flash_init(char *img);
extern "C" void flash_memcpy(uint8_t* src, size_t len);
#endif