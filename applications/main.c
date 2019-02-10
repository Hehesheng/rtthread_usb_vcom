/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   change to new framework
 * 2018-11-19     flybreak     add stm32f407-atk-explorer bsp
 */

#include <board.h>
#include <rtdevice.h>
#include <rtthread.h>
#include "drv_gpio.h"
#define LOG_TAG "main"  //该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL \
    LOG_LVL_DBG  //该模块对应的日志输出级别。不定义时，默认：调试级别
#include <math.h>
#include <shell.h>
#include <stdlib.h>
#include <stm32f4xx.h>
#include <ulog.h>  //必须在 LOG_TAG 与 LOG_LVL 下面
/* defined the LED0 pin: PF10 */
#define LED0_PIN GET_PIN(F, 10)

int main(void) {
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    extern void button_cycle_check_thread(void *parameter);
    rt_thread_t tid = rt_thread_create("tBtn", button_cycle_check_thread,
                                       RT_NULL, 2048, 25, 20);
    if (tid != RT_NULL) {
        rt_thread_startup(tid);
    }

    rt_device_t usb_vcom = rt_device_find("vcom");
    rt_device_open(usb_vcom, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);

    while (1) {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}

static void change_clock(int argc, char **argv) {
    uint32_t sys_clk = 168, temp = 0, i = 1000;
    rt_base_t level;

    if (argc > 1) {
        sys_clk = atoi(argv[1]);

        if (sys_clk < 50 || sys_clk > 250) sys_clk = 168;

        level = rt_hw_interrupt_disable();
        /* Initializes the CPU, AHB and APB busses clocks */
        RCC->CFGR &= ~(0x3);
        RCC->CR &= ~0x01000000; /* PLLOFF */
        temp = RCC->PLLCFGR;
        temp &= ~(uint32_t)(0x1FF << RCC_PLLCFGR_PLLN_Pos);
        temp |= sys_clk << RCC_PLLCFGR_PLLN_Pos;
        RCC->PLLCFGR = temp;
        RCC->CR |= 0x01000000; /* PLLON */
        RCC->CFGR |= 0x2;
        while (~((RCC->CFGR >> 2) & 0x2) && (i-- > 0)) /* 等待时钟修改生效 */
            ;

        SystemCoreClockUpdate();
        HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / RT_TICK_PER_SECOND);
        /* 重新计算波特率 */
        USART1->BRR = UART_BRR_SAMPLING16(HAL_RCC_GetPCLK2Freq(), 115200);
        USART3->BRR = UART_BRR_SAMPLING16(HAL_RCC_GetPCLK1Freq(), 115200);

        rt_hw_interrupt_enable(level);
        log_w("system clock have been changed %d MHz", sys_clk);
    }
}
MSH_CMD_EXPORT(change_clock, change system clock<100 - 250>);

static void change_print(int argc, char **argv) {
    if (argc > 1) {
        rt_device_t new = RT_NULL;
        new = rt_device_find(argv[1]);
        if (new != RT_NULL) {
            finsh_set_device("vcom");
            rt_console_set_device("vcom");
            rt_device_close(new);
            rt_device_open(new, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_STREAM |
                                    RT_DEVICE_FLAG_INT_RX);
            rt_kprintf("Console have been changed to %s\n", argv[1]);
        } else {
            rt_kprintf("Error Device Name\n");
        }
    }
}
MSH_CMD_EXPORT(change_print, change console device);