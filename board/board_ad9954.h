#ifndef __AD9954_H__
#define __AD9954_H__

#include <main.h>
#include <rtthread.h>
#include <stm32f4xx.h>
#define LOG_TAG "ad9954"  //该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL \
    LOG_LVL_DBG  //该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>  //必须在 LOG_TAG 与 LOG_LVL 下面
#include <stdlib.h>

#ifdef RTTHREAD_VERSION
int pass;

#define AD9954_PWR pass
#define AD9954_RES_SET (AD9954_RESET_GPIO_Port->BSRR = AD9954_RESET_Pin)
#define AD9954_RES_RESET (AD9954_RESET_GPIO_Port->BSRR = (uint32_t)AD9954_RESET_Pin << 16)
#define AD9954_IOSY pass
#define AD9954_SDO pass
#define AD9954_CS pass
#define AD9954_SCLK_SET (AD9954_SCLK_GPIO_Port->BSRR = AD9954_SCLK_Pin)
#define AD9954_SCLK_RESET (AD9954_SCLK_GPIO_Port->BSRR = (uint32_t)AD9954_SCLK_Pin << 16)
#define AD9954_SDIO_SET (AD9954_SDIO_GPIO_Port->BSRR = AD9954_SDIO_Pin)
#define AD9954_SDIO_RESET (AD9954_SDIO_GPIO_Port->BSRR = (uint32_t)AD9954_SDIO_Pin << 16)
#define AD9954_OSK_SET (AD9954_P0_GPIO_Port->BSRR = AD9954_P0_Pin)
#define AD9954_OSK_RESET (AD9954_P0_GPIO_Port->BSRR = (uint32_t)AD9954_P0_Pin << 16)
#define AD9954_PS0_SET (AD9954_P0_GPIO_Port->BSRR = AD9954_P0_Pin)
#define AD9954_PS0_RESET (AD9954_P0_GPIO_Port->BSRR = (uint32_t)AD9954_P0_Pin << 16)
#define AD9954_PS1_SET (AD9954_P0_GPIO_Port->BSRR = AD9954_P0_Pin)
#define AD9954_PS1_RESET (AD9954_P0_GPIO_Port->BSRR = (uint32_t)AD9954_P0_Pin << 16)
#define AD9954_UPDATE_SET (AD9954_UPDATE_GPIO_Port->BSRR = AD9954_UPDATE_Pin)
#define AD9954_UPDATE_RESET (AD9954_UPDATE_GPIO_Port->BSRR = (uint32_t)AD9954_UPDATE_Pin << 16)
#else
#define AD9954_PWR PGout(1)    // PE5
#define AD9954_RES PGout(0)    // PE5
#define AD9954_IOSY PFout(15)  // PE5
#define AD9954_SDO PFin(14)
#define AD9954_CS PFout(13)    // PE5
#define AD9954_SCLK PFout(12)  // PE5
#define AD9954_SDIO PFout(11)  // PE5
#define AD9954_OSK PBout(2)    // PE5
#define PS0 PBout(1)           // PB5
#define PS1 PBout(0)           // PE5
#define IOUPDATE PCout(5)      // PE5
#endif /* RTTHREAD_VERSION */

#define CFR1 0X00
#define CFR2 0X01
#define ASF 0X02
#define ARR 0X03
#define FTW0 0X04
#define POW0 0X05
#define FTW1 0X06
#define NLSCW 0X07
#define PLSCW 0X08
#define RSCW0 0X07
#define RSCW1 0X08
#define RSCW2 0X09
#define RSCW3 0X0A
#define RAM 0X0B

#define No_Dwell 0X80

typedef enum { DownScan = 0, UpScan, DoubleScan } ScanMode;

void GPIO_AD9954_Init(void);
int AD9954_Init(void);
void AD9954_Set_Freq(double f);  // single tone
void Single_Tone(double Freq);
void Linear_Sweep(double MinFreq, double MaxFreq, double UpStepFreq,
                  uint8_t UpStepTime, double DownStepFreq, uint8_t DownStepTime,
                  ScanMode Mode);
void PS0_WriteBit(uint8_t BitVal);
void SetASF(uint16_t data);
void DirectSwitchFSK(double f1, double f2, double f3, double f4);
void DirectSwitchPSK(float f, float phase1, float phase2, float phase3,
                     float phase4);
void AM_Init(void);
void Write_ASF(uint16_t factor);  // 2ASK
uint32_t Read_Vau1(void);
uint32_t Read_Vau2(void);
void Line_AR_Sweep(float f1, float f2);  // linear sweep mode
void Update_Sync(void);
void ad9954_change_speed(double input_freq);
void ad9954_change_freq_ram_config(uint32_t f0, uint32_t fp);
#endif