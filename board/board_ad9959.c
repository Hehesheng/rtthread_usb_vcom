#include "board_ad9959.h"
#include "stdlib.h"

static uint8_t CSR_DATA0[1] = {0x10};  // 开 CH0
static uint8_t CSR_DATA1[1] = {0x20};  // 开 CH1
static uint8_t CSR_DATA2[1] = {0x40};  // 开 CH2
static uint8_t CSR_DATA3[1] = {0x80};  // 开 CH3

static uint8_t FR1_DATA[3] = {0xD0, 0x00, 0x00};
RT_UNUSED static uint8_t FR2_DATA[2] = {0x00, 0x00};  // default Value = 0x0000
RT_UNUSED static uint8_t CFR_DATA[3] = {0x00, 0x03,
                                        0x02};  // default Value = 0x000302

// default Value = 0x0000   @ = POW/2^14*360
static uint8_t CPOW0_DATA[2] = {0x00, 0x00};

RT_UNUSED static uint8_t LSRR_DATA[2] = {0x00, 0x00};  // default Value = 0x----

RT_UNUSED static uint8_t RDW_DATA[4] = {0x00, 0x00, 0x00,
                                        0x00};  // default Value = 0x--------

RT_UNUSED static uint8_t FDW_DATA[4] = {0x00, 0x00, 0x00,
                                        0x00};  // default Value = 0x--------

static uint32_t SinFre[4] = {10000000, 10100000, 10200000, 10300000};
static uint32_t SinAmp[4] = {1023, 1023, 1023, 1023};
static uint32_t SinPhr[4] = {0, 0, 0, 0};

static void InitReset(void);
static void IO_Updata(void);
static void delay_9959(uint32_t length);
static void InitIO_9959(void);

/**
 * @brief  初始化io口和reset设备
 * @funna  Init_AD9959
 * @param  None
 * @retval None
 */
int Init_AD9959(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOF_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOF,
                      AD9959_RST_Pin | AD9959_UPDATE_Pin | AD9959_CS_Pin |
                          AD9959_SCK_Pin | AD9959_SDIO_Pin,
                      GPIO_PIN_RESET);

    /*Configure GPIO pins : AD9959_RST_Pin AD9959_UPDATE_Pin AD9959_CS_Pin
       AD9959_SCK_Pin AD9959_SDIO_Pin */
    GPIO_InitStruct.Pin = AD9959_RST_Pin | AD9959_UPDATE_Pin | AD9959_CS_Pin |
                          AD9959_SCK_Pin | AD9959_SDIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    InitIO_9959();
    InitReset();

    WriteData_AD9959(FR1_ADD, 3, FR1_DATA, 1);  //写功能寄存器1

    // WriteData_AD9959(FR2_ADD,2,FR2_DATA,0);

    // WriteData_AD9959(CFR_ADD,3,CFR_DATA,1);

    // WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
    // //WriteData_AD9959(ACR_ADD,3,ACR_DATA,0);
    // WriteData_AD9959(LSRR_ADD,2,LSRR_DATA,0);
    // WriteData_AD9959(RDW_ADD,2,RDW_DATA,0);

    // WriteData_AD9959(FDW_ADD,4,FDW_DATA,1);
    //写入初始频率
    Write_frequence(3, SinFre[3]);
    Write_frequence(0, SinFre[0]);
    // Write_frequence(1, SinFre[1]);
    // Write_frequence(2, SinFre[2]);

    ////  Write_frequence(3,50);
    ////  Write_frequence(0,50);
    ////  Write_frequence(1,50);
    ////  Write_frequence(2,50);

    Write_Phase(3, SinPhr[3]);
    Write_Phase(0, SinPhr[0]);
    // Write_Phase(1, SinPhr[1]);
    // Write_Phase(2, SinPhr[2]);

    Write_Amplitude(3, SinAmp[3]);
    Write_Amplitude(0, SinAmp[0]);
    // Write_Amplitude(1, SinAmp[1]);
    // Write_Amplitude(2, SinAmp[2]);

    return 0;
}
INIT_DEVICE_EXPORT(Init_AD9959);
MSH_CMD_EXPORT_ALIAS(Init_AD9959, 9959_init, ad9959 init);

/**
 * @brief  9959专用延时
 * @funna  delay_9959
 * @param  时间
 * @retval None
 */
static void delay_9959(uint32_t length) {
    length = length * 1;
    while (length--)
        ;
}

/**
 * @brief  IO口状态初始化
 * @funna  InitIO_9959
 * @param  None
 * @retval None
 */
static void InitIO_9959(void) {
    AD9959_PWR_RESET;
    CS_SET;
    SCLK_RESET;
    UPDATE_RESET;
    PS0_RESET;
    PS1_RESET;
    PS2_RESET;
    PS3_RESET;
    SDIO0_RESET;
    SDIO1_RESET;
    SDIO2_RESET;
    SDIO3_RESET;
}

/**
 * @brief  复位9959
 * @funna  InitReset
 * @param  None
 * @retval None
 */
static void InitReset(void) {
    RESET_RESET;
    delay_9959(1);
    RESET_SET;
    delay_9959(30);
    RESET_RESET;
}

/**
 * @brief  更新数据
 * @funna  IO_Updata
 * @param  None
 * @retval None
 */
static void IO_Updata(void) {
    UPDATE_RESET;
    // delay_9959(1);
    UPDATE_SET;
    // delay_9959(1);
    UPDATE_RESET;
}

/**
 * @brief  写入数据
 * @funna  WriteData_9959
 * @param  地址(RegisterAddress), 数据长度(NumberofRegisters),
 * 数据(*RegisterData), 更新(temp)
 * @retval None
 */
void WriteData_AD9959(uint8_t RegisterAddress, uint8_t NumberofRegisters,
                      uint8_t *RegisterData, uint8_t temp) {
    uint8_t ControlValue = 0;
    uint8_t ValueToWrite = 0;
    uint8_t RegisterIndex = 0;
    uint8_t i = 0;

    ControlValue = RegisterAddress;
    //写入地址
    SCLK_RESET;
    CS_RESET;
    for (i = 0; i < 8; i++) {
        SCLK_RESET;
        if (0x80 == (ControlValue & 0x80))
            SDIO0_SET;
        else
            SDIO0_RESET;
        SCLK_SET;
        ControlValue <<= 1;
        // delay_9959(2);
    }
    SCLK_RESET;
    //写入数据
    for (RegisterIndex = 0; RegisterIndex < NumberofRegisters;
         RegisterIndex++) {
        ValueToWrite = RegisterData[RegisterIndex];
        for (i = 0; i < 8; i++) {
            SCLK_RESET;
            if (0x80 == (ValueToWrite & 0x80))
                SDIO0_SET;
            else
                SDIO0_RESET;
            SCLK_SET;
            ValueToWrite <<= 1;
            // delay_9959(2);
        }
        SCLK_RESET;
    }
    if (temp == 1) IO_Updata();
    CS_SET;
}

/**
 * @brief  写入频率
 * @funna  Write_frequence
 * @param  通道(channel), 频率(freq)
 * @retval None
 */
void Write_frequence(uint8_t Channel, uint32_t Freq) {
    uint8_t CFTW0_DATA[4] = {0x00, 0x00, 0x00, 0x00};  //中间变量
    uint32_t Temp;
    Temp =
        (uint32_t)Freq *
        8.589934592;  //将输入频率因子分为四个字节  4.294967296=(2^32)/500000000
    CFTW0_DATA[3] = (uint8_t)Temp;
    CFTW0_DATA[2] = (uint8_t)(Temp >> 8);
    CFTW0_DATA[1] = (uint8_t)(Temp >> 16);
    CFTW0_DATA[0] = (uint8_t)(Temp >> 24);
    if (Channel == 0) {
        WriteData_AD9959(CSR_ADD, 1, CSR_DATA0, 0);  //控制寄存器写入CH0通道
        WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA,
                         1);  // CTW0 address 0x04.输出CH0设定频率
    } else if (Channel == 1) {
        WriteData_AD9959(CSR_ADD, 1, CSR_DATA1, 0);  //控制寄存器写入CH1通道
        WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA,
                         1);  // CTW0 address 0x04.输出CH1设定频率
    } else if (Channel == 2) {
        WriteData_AD9959(CSR_ADD, 1, CSR_DATA2, 0);  //控制寄存器写入CH2通道
        WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA,
                         1);  // CTW0 address 0x04.输出CH2设定频率
    } else if (Channel == 3) {
        WriteData_AD9959(CSR_ADD, 1, CSR_DATA3, 0);  //控制寄存器写入CH3通道
        WriteData_AD9959(CFTW0_ADD, 4, CFTW0_DATA,
                         1);  // CTW0 address 0x04.输出CH3设定频率
    }
}

/**
 * @brief  设置幅度
 * @funna  Write_Amplitude
 * @param  通道(channel), 幅度(ampli)
 * @retval None
 */
void Write_Amplitude(uint8_t Channel, uint16_t Ampli) {
    uint16_t A_temp;  //=0x23ff;
    uint8_t ACR_DATA[3] = {0x00, 0x00,
                           0x00};  // default Value = 0x--0000 Rest = 18.91/Iout

    A_temp = Ampli | 0x1000;
    ACR_DATA[2] = (uint8_t)A_temp;         //低位数据
    ACR_DATA[1] = (uint8_t)(A_temp >> 8);  //高位数据
    if (Channel == 0) {
        WriteData_AD9959(CSR_ADD, 1, CSR_DATA0, 0);
        WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
    } else if (Channel == 1) {
        WriteData_AD9959(CSR_ADD, 1, CSR_DATA1, 0);
        WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
    } else if (Channel == 2) {
        WriteData_AD9959(CSR_ADD, 1, CSR_DATA2, 0);
        WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
    } else if (Channel == 3) {
        WriteData_AD9959(CSR_ADD, 1, CSR_DATA3, 0);
        WriteData_AD9959(ACR_ADD, 3, ACR_DATA, 1);
    }
}

/**
 * @brief  写入相位
 * @funna  Write_Phase
 * @param  通道(channel), 相位(Phase)
 * @retval None
 */
void Write_Phase(uint8_t Channel, uint16_t Phase) {
    uint16_t P_temp = 0;
    P_temp = (uint16_t)((float)Phase / 360 * 16384);
    CPOW0_DATA[1] = (uint8_t)P_temp;
    CPOW0_DATA[0] = (uint8_t)(P_temp >> 8);
    if (Channel == 0) {
        WriteData_AD9959(CSR_ADD, 1, CSR_DATA0, 0);
        WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 1);
    } else if (Channel == 1) {
        WriteData_AD9959(CSR_ADD, 1, CSR_DATA1, 0);
        WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 1);
    } else if (Channel == 2) {
        WriteData_AD9959(CSR_ADD, 1, CSR_DATA2, 0);
        WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 1);
    } else if (Channel == 3) {
        WriteData_AD9959(CSR_ADD, 1, CSR_DATA3, 0);
        WriteData_AD9959(CPOW0_ADD, 2, CPOW0_DATA, 1);
    }
}

void ad99_speed(void) {
    uint32_t i = 0;

    log_d("begin");
    for (i = 1; i < 1000000; i++) {
        Write_frequence(0, i);
    }
    log_d("done");
    // Write_frequence(0, 1);
}
MSH_CMD_EXPORT_ALIAS(ad99_speed, 9959_speed, speed test);

#include <rtdevice.h>

void adc_speed(void) {
    rt_adc_device_t adc = RT_NULL;

    adc = (rt_adc_device_t)rt_device_find("adc1");
    rt_adc_enable(adc, 4);

    log_d("begin");
    for (uint32_t i = 0; i < 1000000; i++) {
        rt_adc_read(adc, 4);
    }
    log_d("done");
}
MSH_CMD_EXPORT(adc_speed, adc speed test);

/* 控制指令: 9959_c <freq|amp|phase> <ch> <data> */
void ad9959_control_cmd(int argc, char **argv) {
    if (argc >= 4) {
        if (!rt_strcmp("freq", argv[1])) {
            Write_frequence(atoi(argv[2]), atoi(argv[3]));
        } else if (!rt_strcmp("amp", argv[1])) {
            Write_Amplitude(atoi(argv[2]), atoi(argv[3]));
        } else if (!rt_strcmp("phase", argv[1])) {
            Write_Phase(atoi(argv[2]), atoi(argv[3]));
        }
    } else {
        rt_kprintf(
            "less argv\ncmd like : 9959_c <freq|amp|phase> <ch> <data>\n");
    }
}
MSH_CMD_EXPORT_ALIAS(ad9959_control_cmd, 9959_c, 9959_c <freq|amp|phase> <ch> <data>);
