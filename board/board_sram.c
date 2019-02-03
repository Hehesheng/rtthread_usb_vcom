/*
 * ==============================
 * @File   board_sram.c
 * @Date   Created on 2019/01/21
 * @Author Hehesheng
 * ==============================
 */

#include "board_sram.h"
#include <stdio.h>
#include <stdlib.h>
#define LOG_TAG "sram"  //该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL \
    LOG_LVL_DBG  //该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>  //必须在 LOG_TAG 与 LOG_LVL 下面

#if defined(BSP_USING_EXT_FMC_IO)

/**
 * @brief  初始化SDRAM
 * @param  None
 * @retval None
 */
static struct rt_mempool sram_mem;
int board_sram_init(void) {
    uint32_t *p = (uint32_t *)SRAM_ADDRESS_BEGIN;
    FSMC_NORSRAM_TimingTypeDef Timing;
    SRAM_HandleTypeDef hsram1;
    /** Perform the SRAM1 memory initialization sequence
     */
    hsram1.Instance = FSMC_NORSRAM_DEVICE;
    hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
    /* hsram1.Init */
    hsram1.Init.NSBank = FSMC_NORSRAM_BANK3;
    hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
    hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
    hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
    hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
    hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
    hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
    hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
    hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
    hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
    hsram1.Init.PageSize = FSMC_PAGE_SIZE_NONE;
    /* Timing */
    Timing.AddressSetupTime = 0;
    Timing.AddressHoldTime = 15;
    Timing.DataSetupTime = SRAM_SPEED_SETUP;
    Timing.BusTurnAroundDuration = 0;
    Timing.CLKDivision = 16;
    Timing.DataLatency = 17;
    Timing.AccessMode = FSMC_ACCESS_MODE_A;
    /* ExtTiming */

    HAL_SRAM_MspInit(RT_NULL);
    HAL_SRAM_Init(&hsram1, &Timing, &Timing);

    for (uint32_t i = 0; i < 1024 / 4 * 1024; i++) {
        *p = 0;
        p++;
    }
    /* sram 1Mb size block_size = 1020 + 4 ~= 1kb */
    rt_mp_init(&sram_mem, "sram", (void *)SRAM_ADDRESS_BEGIN,
               SRAM_KB_SIZE * 1024, 1020);

    return 0;
}
INIT_DEVICE_EXPORT(board_sram_init);
MSH_CMD_EXPORT_ALIAS(board_sram_init, sram_init, init sram);

/**
 * @brief  检查1MSRAM内存是否可用
 * @param  None
 * @retval None
 */
void sram_test(void) {
    uint32_t *p = RT_NULL;

    p = (uint32_t *)SRAM_ADDRESS_BEGIN;

    for (int i = 0;; i++) {
        *p = 0xFFFF0000;
        if (0xFFFF0000 != *p) {
            log_e("sram error: %p", p);
            return;
        }
        p++;
        if ((SRAM_ADDRESS_BEGIN + SRAM_KB_SIZE * 1024) == (uint32_t)p) break;
    }
    log_i("sram ok");
}
MSH_CMD_EXPORT(sram_test, sram test);

/**
 * @brief  向SRAM偏移写入数据, 为4字节
 * @param  None
 * @retval None
 */
void sram_write(int argc, char **argv) {
    uint32_t offset = 0, data = 0;
    uint32_t *p = (uint32_t *)SRAM_ADDRESS_BEGIN;

    if (argc > 2) {
        offset = atoi(argv[1]);
        data = atoi(argv[2]);
        *(p + offset) = data;
        rt_kprintf("write %d at %p\n", data, p + offset);
    }
}
// MSH_CMD_EXPORT(sram_write, write sram<offset><data>);

/**
 * @brief  向SRAM偏移读取数据, 为4字节
 * @param  None
 * @retval None
 */
void sram_read(int argc, char **argv) {
    uint32_t offset = 0, data = 0;
    uint32_t *p = (uint32_t *)SRAM_ADDRESS_BEGIN;

    if (argc > 1) {
        offset = atoi(argv[1]);
        data = atoi(argv[2]);
        data = *(p + offset);
        rt_kprintf("read %d at %p\n", data, p + offset);
    }
}
// MSH_CMD_EXPORT(sram_read, read sram<offset>);

#endif  // BSP_USING_EXT_FMC_IO
