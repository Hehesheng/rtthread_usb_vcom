#ifndef __BOARD_AD9959_H__
#define __BOARD_AD9959_H__

#include <main.h>
#include <rtthread.h>
#include <stm32f4xx.h>
#define LOG_TAG "ad9959"  //该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL \
    LOG_LVL_DBG  //该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>  //必须在 LOG_TAG 与 LOG_LVL 下面

#define CSR_ADD 0x00    // CSR 通道选择寄存器
#define FR1_ADD 0x01    // FR1 功能寄存器1
#define FR2_ADD 0x02    // FR2 功能寄存器2
#define CFR_ADD 0x03    // CFR 通道功能寄存器
#define CFTW0_ADD 0x04  // CTW0 通道频率转换字寄存器
#define CPOW0_ADD 0x05  // CPW0 通道相位转换字寄存器
#define ACR_ADD 0x06    // ACR 幅度控制寄存器
#define LSRR_ADD 0x07   // LSR 通道线性扫描寄存器
#define RDW_ADD 0x08    // RDW 通道线性向上扫描寄存器
#define FDW_ADD 0x09    // FDW 通道线性向下扫描寄存器
// AD9959管脚宏定义PC0~7, PD0~4
#if sys
#define CS PDout(3)
#define SCLK PDout(2)
#define UPDATE PDout(0)
#define PS0 PAout(0)
#define PS1 PAout(1)
#define PS2 PAout(2)
#define PS3 PAout(3)
#define SDIO0 PDout(4)
#define SDIO1 PAout(4)
#define SDIO2 PAout(5)
#define SDIO3 PAout(6)
#define AD9959_PWR PAout(7)
#define RESET PDout(1)
#else
#define CS_SET (AD9959_CS_GPIO_Port->BSRR = AD9959_CS_Pin)
#define CS_RESET (AD9959_CS_GPIO_Port->BSRR = (uint32_t)AD9959_CS_Pin << 16)
#define SCLK_SET (AD9959_SCK_GPIO_Port->BSRR = AD9959_SCK_Pin)
#define SCLK_RESET (AD9959_SCK_GPIO_Port->BSRR = (uint32_t)AD9959_SCK_Pin << 16)
#define UPDATE_SET (AD9959_UPDATE_GPIO_Port->BSRR = AD9959_UPDATE_Pin)
#define UPDATE_RESET (AD9959_UPDATE_GPIO_Port->BSRR = (uint32_t)AD9959_UPDATE_Pin << 16)
#define PS0_RESET
#define PS1_RESET
#define PS2_RESET
#define PS3_RESET
#define SDIO0_SET (AD9959_SDIO_GPIO_Port->BSRR = AD9959_SDIO_Pin)
#define SDIO0_RESET (AD9959_SDIO_GPIO_Port->BSRR = (uint32_t)AD9959_SDIO_Pin << 16)
#define SDIO1_RESET
#define SDIO2_RESET
#define SDIO3_RESET
#define AD9959_PWR_RESET
#define RESET_SET (AD9959_RST_GPIO_Port->BSRR = AD9959_RST_Pin)
#define RESET_RESET (AD9959_RST_GPIO_Port->BSRR = (uint32_t)AD9959_RST_Pin << 16)
#endif

int Init_AD9959(void);
void WriteData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters,
                      uint8_t *RegisterData, uint8_t temp);
void Write_frequence(uint8_t Channel, uint32_t Freq);
void Write_Amplitude(uint8_t Channel, uint16_t Ampli);
void Write_Phase(uint8_t Channel, uint16_t Phase);

#endif /* AD9959_H */
