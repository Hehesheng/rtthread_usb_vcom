/*
 * ==============================
 * @File   board_sram.h
 * @Date   Created on 2019/01/21
 * @Author Hehesheng
 * ==============================
 */
#ifndef __SRAM_H__
#define __SRAM_H__

#include <rtthread.h>
#include <stm32f4xx.h>

#if defined(BSP_USING_EXT_FMC_IO)

#define SRAM_SPEED_SETUP 10
#define SRAM_ADDRESS_BEGIN 0x68000000
#define SRAM_KB_SIZE 1024

int board_sram_init(void);

#endif  // BSP_USING_EXT_FMC_IO

#endif	// __SRAM_H__
