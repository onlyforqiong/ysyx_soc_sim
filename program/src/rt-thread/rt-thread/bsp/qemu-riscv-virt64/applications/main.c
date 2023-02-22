/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-20     bigmagic     first version
 */

#include <rtthread.h>
#include <rthw.h>
#include <stdio.h>
#include <string.h>

#define ARR_SIZE 0x2000

int main(void)
{
    rt_kprintf("Hello RISC-V!\n");
    // rt_kprintf("START TEST...");
    // uint16_t* data = (void*)(0x80200000);
    // for(int i = 0; i < ARR_SIZE / sizeof(uint16_t); i++){
    //     data[i] = i;
    // }
    // rt_kprintf("ALL DATA PREPARED\n");
    // for(int i = 0; i < ARR_SIZE / sizeof(uint16_t); i++){
    //     if(data[i] != i) {
    //         rt_kprintf("data[%d] is %d",i, data[i]);
    //     }
        
    // }
    // rt_kprintf("16 TESTS PASSED!!\n");

    // uint8_t* data_8 = (void*)(0x80200000);
    // for(int i = 0; i < 256; i++){
    //     data_8[i] = i;
    // }
    // rt_kprintf("ALL DATA PREPARED\n");
    // for(int i = 0; i < 256; i++){
    //     if(data_8[i] != i) {
    //         rt_kprintf("data8[%d] is %d",i, data_8[i]);
    //     }
        
    // }
    // rt_kprintf("8 TESTS PASSED!!\n");

    // uint32_t* data_32 = (void*)(0x80200000);
    // for(int i = 0; i < ARR_SIZE / sizeof(uint32_t); i++){
    //     data_32[i] = i;
    // }
    // rt_kprintf("ALL DATA PREPARED\n");
    // for(int i = 0; i < ARR_SIZE / sizeof(uint32_t); i++){
    //     if(data_32[i] != i) {
    //         rt_kprintf("data32[%d] is %d",i, data_32[i]);
    //     }
        
    // }
    // rt_kprintf("32 TESTS PASSED!!\n");


    // uint64_t* data_64 = (void*)(0x80200000);
    // for(int i = 0; i < ARR_SIZE / sizeof(uint64_t); i++){
    //     data_64[i] = i;
    // }
    // rt_kprintf("ALL DATA PREPARED\n");
    // for(int i = 0; i < ARR_SIZE / sizeof(uint64_t); i++){
    //     if(data_64[i] != i) {
    //         rt_kprintf("data64[%d] is %d",i, data_64[i]);
    //     }
        
    // }
    // rt_kprintf("64 TESTS PASSED!!\n");


    return 0;
    
    // return 0;
}
