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
#include <stdlib.h>
#include <stm32f4xx.h>
#include <ulog.h>  //必须在 LOG_TAG 与 LOG_LVL 下面
/* defined the LED0 pin: PF10 */
#define LED0_PIN GET_PIN(F, 10)
// #define FREQ_CAPTURE_PIN GET_PIN(D, 0)

#define DDS_WORK_FREQ0 (25000000u) /* 工作的中心频率 */
#define DDS_WORK_FREQM (10000000u) /* 频偏 */
#define DDS_CHANNAL_SELECT (0)     /* DDS通道的选择 */
#define VOLTAGE_SETUP (1.5f)       /* 抬高的电压量 */
#define VOLTAGE_MAX (4095u)        /* adc位数 */
#define REF_VOLTAGE (3.3f)         /* 参考电压 */
#define ADC_CHANNAL_SELECT (4)     /* adc通道选择 */
#define Convert_To_Float(x) ((double)(3.3 * (x) / (double)VOLTAGE_MAX))
#define Convert_To_Freq(x) ((uint32_t)((x)*DDS_WORK_FREQM + DDS_WORK_FREQ0))

uint32_t tim5_detal_val = 0;

int main(void) {
    double temp_freq = 0, current_freq = 2;
    rt_device_t uart_dev = RT_NULL;
    const char hmi_cmd_end[] = {"\xff\xff\xff"};
    char arr[30] = {0};
    char cache_buff[30] = {0};

    extern uint32_t fsk_f0_freq, fsk_fp_freq;
    extern void ad9954_change_speed(double input_freq);

    uart_dev = rt_device_find("uart3");
    if (uart_dev == RT_NULL) {
        log_e("uart3 open fail!!!");
        while (1) {
            rt_thread_delay(1000);
        }
    }
    rt_device_open(uart_dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    sprintf(cache_buff, "f0.txt=\"%.3fKHz\"", (double)fsk_f0_freq / 1000);
    rt_device_write(uart_dev, 0, cache_buff, rt_strlen(cache_buff));
    rt_device_write(uart_dev, 0, hmi_cmd_end, rt_strlen(hmi_cmd_end));

    sprintf(cache_buff, "fp.txt=\"%.3fKHz\"", (double)fsk_fp_freq / 1000);
    rt_device_write(uart_dev, 0, cache_buff, rt_strlen(cache_buff));
    rt_device_write(uart_dev, 0, hmi_cmd_end, rt_strlen(hmi_cmd_end));

    while (1) {
        temp_freq = (HAL_RCC_GetHCLKFreq() * 0.5 / (tim5_detal_val * 1.0));
        // rt_kprintf("freq: %d\n", (uint32_t)temp_freq);
        /* 发送结果到串口屏 先解决单位选择 */
        if (temp_freq < 10000 && temp_freq >= 0) {
            sprintf(arr, "freq.txt=\"%.3fHz\"", temp_freq);
        } else if (temp_freq >= 10000 && temp_freq < 1000000) {
            sprintf(arr, "freq.txt=\"%.3fKHz\"", temp_freq / 1000);
        } else {
            sprintf(arr, "freq.txt=\"error\"");
        }
        /* 发送以及清空 */
        rt_device_write(uart_dev, 0, arr, rt_strlen(arr));
        rt_device_write(uart_dev, 0, hmi_cmd_end, rt_strlen(hmi_cmd_end));
        for (int i = 0; i < 30; i++) arr[i] = 0;

        /* 判断是否需要更改频率输出 当目前输出频率与输入频率差到达1/1000 */
        if (current_freq < 1.5 || current_freq > 97000) {
            current_freq = 2; /* 数据异常处理 */
        }
        if (fabs(temp_freq - current_freq) > (current_freq * 0.001)) {
            log_d("freq change: temp: %.3f current:%.3f", temp_freq,
                  current_freq);
            ad9954_change_speed(temp_freq);
            current_freq = temp_freq;
        }
        rt_thread_mdelay(1000);
    }

    return RT_EOK;
}

/* 暂停/开始main的工作 */
static void main_control(int argc, char **argv) {
    rt_sem_t running = RT_NULL;

    if (argc > 1) {
        running = (rt_sem_t)rt_object_find("smain", RT_Object_Class_Semaphore);

        if (argv[1][0] == '1') {
            if (running->value == 0) {
                rt_sem_release(running);
            }
            rt_kprintf("main running\n");
        } else if (argv[1][0] == '0') {
            if (running->value == 1) {
                rt_sem_take(running, 0);
            }
            rt_kprintf("main stoped\n");
        }
    } else {
        rt_kprintf("cmd like: main <1|0>\n");
    }
}
MSH_CMD_EXPORT_ALIAS(main_control, main, main<1 | 0>);

static void change_clock(int argc, char **argv) {
    uint32_t sys_clk = 168, temp = 0, i = 1000;
    rt_base_t level;

    if (argc > 1) {
        sys_clk = atoi(argv[1]);

        if (sys_clk < 50 || sys_clk > 250) sys_clk = 168;
        /**Initializes the CPU, AHB and APB busses clocks
         */
        level = rt_hw_interrupt_disable();

        RCC->CFGR &= ~(0x3);
        RCC->CR &= ~0x01000000;  // PLLOFF
        temp = RCC->PLLCFGR;
        temp &= ~(uint32_t)(0x1FF << RCC_PLLCFGR_PLLN_Pos);
        temp |= sys_clk << RCC_PLLCFGR_PLLN_Pos;
        RCC->PLLCFGR = temp;
        RCC->CR |= 0x01000000;  // PLLON
        RCC->CFGR |= 0x2;
        while (~((RCC->CFGR >> 2) & 0x2) && (i-- > 0))
            ;

        SystemCoreClockUpdate();
        HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / RT_TICK_PER_SECOND);

        USART1->BRR = UART_BRR_SAMPLING16(HAL_RCC_GetPCLK2Freq(), 115200);
        USART3->BRR = UART_BRR_SAMPLING16(HAL_RCC_GetPCLK1Freq(), 115200);

        rt_hw_interrupt_enable(level);
        log_w("system clock have been changed %d MHz", sys_clk);
    }
}
MSH_CMD_EXPORT(change_clock, change system clock<100 - 250>);

TIM_HandleTypeDef TIM5_Handler;  //定时器5句柄

//定时器5通道1输入捕获配置
static int TIM5_CH1_Cap_Init(void) {
    TIM_IC_InitTypeDef TIM5_CH1Config;

    TIM5_Handler.Instance = TIM5;                        //通用定时器5
    TIM5_Handler.Init.Prescaler = 0;                     //分频
    TIM5_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;  //向上计数器
    TIM5_Handler.Init.Period = 0xFFFFFFFF;               //自动装载值
    TIM5_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&TIM5_Handler);

    TIM5_CH1Config.ICPolarity = TIM_ICPOLARITY_RISING;      //上升沿捕获
    TIM5_CH1Config.ICSelection = TIM_ICSELECTION_DIRECTTI;  //映射到TI1上
    TIM5_CH1Config.ICPrescaler = TIM_ICPSC_DIV1;  //配置输入分频，不分频
    TIM5_CH1Config.ICFilter = 0;  //配置输入滤波器，不滤波
    HAL_TIM_IC_ConfigChannel(&TIM5_Handler, &TIM5_CH1Config,
                             TIM_CHANNEL_1);            //配置TIM5通道1
    HAL_TIM_IC_Start_IT(&TIM5_Handler, TIM_CHANNEL_1);  //开始捕获TIM5的通道1
    TIM5->CNT = 0;
    __HAL_TIM_ENABLE_IT(&TIM5_Handler, TIM_IT_UPDATE);  //使能更新中断

    return 0;
}
INIT_APP_EXPORT(TIM5_CH1_Cap_Init);

//定时器5底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_IC_Init()调用
// htim:定时器5句柄
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim) {
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM5_CLK_ENABLE();   //使能TIM5时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();  //开启GPIOA时钟

    GPIO_Initure.Pin = GPIO_PIN_0;                   // PA0
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;             //复用推挽输出
    GPIO_Initure.Pull = GPIO_PULLDOWN;               //下拉
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;  //高速
    GPIO_Initure.Alternate = GPIO_AF2_TIM5;          // PA0复用为TIM5通道1
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);

    HAL_NVIC_SetPriority(TIM5_IRQn, 2,
                         0);  //设置中断优先级，抢占优先级2，子优先级0
    HAL_NVIC_EnableIRQ(TIM5_IRQn);  //开启ITM5中断
}

//定时器5中断服务函数
void TIM5_IRQHandler(void) {
    static uint32_t capture_status;
    static uint32_t before_val;

    rt_interrupt_enter();
    // HAL_TIM_IRQHandler(&TIM5_Handler);  //定时器共用处理函数
    TIM5->SR = ~(TIM_IT_CC1 | TIM_IT_UPDATE);
    if (capture_status) {
        capture_status = 0;
        tim5_detal_val = TIM5->CCR1 - before_val;
        TIM5->CNT = 0;
    } else {
        capture_status = 1;
        before_val = TIM5->CCR1;
    }

    rt_interrupt_leave();
}
