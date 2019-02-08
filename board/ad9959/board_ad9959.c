#include "board_ad9959.h"
#ifdef RT_USING_FINSH
#include <stdlib.h>
#endif /* RT_USING_FINSH */

__attribute__((weak)) void ad9959_pins_init(void) {
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
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}

static uint8_t CSR_DATA0[1] = {0x10};  // 开 CH0
static uint8_t CSR_DATA1[1] = {0x20};  // 开 CH1
static uint8_t CSR_DATA2[1] = {0x40};  // 开 CH2
static uint8_t CSR_DATA3[1] = {0x80};  // 开 CH3

static uint8_t FR1_DATA[3] = {0xD0, 0x00, 0x00};

__attribute__((unused)) static uint8_t FR2_DATA[2] = {
    0x00, 0x00};  // default Value = 0x0000

__attribute__((unused)) static uint8_t CFR_DATA[3] = {
    0x00, 0x03, 0x02};  // default Value = 0x000302

// default Value = 0x0000   @ = POW/2^14*360
static uint8_t CPOW0_DATA[2] = {0x00, 0x00};

__attribute__((unused)) static uint8_t LSRR_DATA[2] = {
    0x00, 0x00};  // default Value = 0x----

__attribute__((unused)) static uint8_t RDW_DATA[4] = {
    0x00, 0x00, 0x00, 0x00};  // default Value = 0x--------

__attribute__((unused)) static uint8_t FDW_DATA[4] = {
    0x00, 0x00, 0x00, 0x00};  // default Value = 0x--------

static uint32_t SinFre[4] = {10000000, 10100000, 10200000, 10300000};
static uint32_t SinAmp[4] = {1023, 1023, 1023, 1023};
static uint32_t SinPhr[4] = {0, 0, 0, 0};

static void _init_reset(void);
static void _io_updata(void);
static void _delay(uint32_t delayTime);
static void _pins_state_init(void);

/**
 * @brief  初始化io口和reset设备
 * @funna  ad9959_init(void)
 * @param  None
 * @retval None
 */
int ad9959_init(void) {
    ad9959_pins_init();
    _pins_state_init();
    _init_reset();

    ad9959_write_data(FR1_ADD, 3, FR1_DATA, 1);  //写功能寄存器1

    // ad9959_write_data(FR2_ADD,2,FR2_DATA,0);

    // ad9959_write_data(CFR_ADD,3,CFR_DATA,1);

    // ad9959_write_data(CPOW0_ADD,2,CPOW0_DATA,0);
    // //ad9959_write_data(ACR_ADD,3,ACR_DATA,0);
    // ad9959_write_data(LSRR_ADD,2,LSRR_DATA,0);
    // ad9959_write_data(RDW_ADD,2,RDW_DATA,0);

    // ad9959_write_data(FDW_ADD,4,FDW_DATA,1);
    //写入初始频率
    ad9959_write_freq(3, SinFre[3]);
    ad9959_write_freq(0, SinFre[0]);
    // ad9959_write_freq(1, SinFre[1]);
    // ad9959_write_freq(2, SinFre[2]);

    ////  ad9959_write_freq(3,50);
    ////  ad9959_write_freq(0,50);
    ////  ad9959_write_freq(1,50);
    ////  ad9959_write_freq(2,50);

    ad9959_write_phase(3, SinPhr[3]);
    ad9959_write_phase(0, SinPhr[0]);
    // ad9959_write_phase(1, SinPhr[1]);
    // ad9959_write_phase(2, SinPhr[2]);

    ad9959_write_amp(3, SinAmp[3]);
    ad9959_write_amp(0, SinAmp[0]);
    // ad9959_write_amp(1, SinAmp[1]);
    // ad9959_write_amp(2, SinAmp[2]);

    return 0;
}
#ifdef RT_USING_COMPONENTS_INIT
INIT_DEVICE_EXPORT(ad9959_init);
#endif /* RT_USING_COMPONENTS_INIT */

#ifdef RT_USING_FINSH
MSH_CMD_EXPORT_ALIAS(ad9959_init, 9959_init, ad9959 init);
#endif /* RT_USING_FINSH */

/**
 * @brief  9959专用延时
 * @funna  _delay
 * @param  时间
 * @retval None
 */
static void _delay(uint32_t delayTime) {
    delayTime = delayTime * 1;
    while (delayTime--)
        ;
}

/**
 * @brief  IO口状态初始化
 * @funna  _pins_state_init
 * @param  None
 * @retval None
 */
static void _pins_state_init(void) {
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
 * @funna  _init_reset
 * @param  None
 * @retval None
 */
static void _init_reset(void) {
    RESET_RESET;
    _delay(1);
    RESET_SET;
    _delay(30);
    RESET_RESET;
}

/**
 * @brief  更新数据
 * @funna  _io_updata
 * @param  None
 * @retval None
 */
static void _io_updata(void) {
    UPDATE_RESET;
    // _delay(1);
    UPDATE_SET;
    // _delay(1);
    UPDATE_RESET;
}

/**
 * @brief  写入数据
 * @funna  WriteData_9959
 * @param  地址(RegisterAddress), 数据长度(NumberofRegisters),
 * 数据(*RegisterData), 更新(temp)
 * @retval None
 */
void ad9959_write_data(uint8_t RegisterAddress, uint8_t NumberofRegisters,
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
        // _delay(2);
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
            // _delay(2);
        }
        SCLK_RESET;
    }
    if (temp == 1) _io_updata();
    CS_SET;
}

/**
 * @brief  写入频率
 * @funna  ad9959_write_freq
 * @param  通道(channel), 频率(freq)
 * @retval None
 */
void ad9959_write_freq(uint8_t Channel, uint32_t Freq) {
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
        ad9959_write_data(CSR_ADD, 1, CSR_DATA0, 0);  //控制寄存器写入CH0通道
        ad9959_write_data(CFTW0_ADD, 4, CFTW0_DATA,
                          1);  // CTW0 address 0x04.输出CH0设定频率
    } else if (Channel == 1) {
        ad9959_write_data(CSR_ADD, 1, CSR_DATA1, 0);  //控制寄存器写入CH1通道
        ad9959_write_data(CFTW0_ADD, 4, CFTW0_DATA,
                          1);  // CTW0 address 0x04.输出CH1设定频率
    } else if (Channel == 2) {
        ad9959_write_data(CSR_ADD, 1, CSR_DATA2, 0);  //控制寄存器写入CH2通道
        ad9959_write_data(CFTW0_ADD, 4, CFTW0_DATA,
                          1);  // CTW0 address 0x04.输出CH2设定频率
    } else if (Channel == 3) {
        ad9959_write_data(CSR_ADD, 1, CSR_DATA3, 0);  //控制寄存器写入CH3通道
        ad9959_write_data(CFTW0_ADD, 4, CFTW0_DATA,
                          1);  // CTW0 address 0x04.输出CH3设定频率
    }
}

/**
 * @brief  设置幅度
 * @funna  ad9959_write_amp
 * @param  通道(channel), 幅度(ampli)
 * @retval None
 */
void ad9959_write_amp(uint8_t Channel, uint16_t Ampli) {
    uint16_t A_temp;  //=0x23ff;
    uint8_t ACR_DATA[3] = {0x00, 0x00,
                           0x00};  // default Value = 0x--0000 Rest = 18.91/Iout

    A_temp = Ampli | 0x1000;
    ACR_DATA[2] = (uint8_t)A_temp;         //低位数据
    ACR_DATA[1] = (uint8_t)(A_temp >> 8);  //高位数据
    if (Channel == 0) {
        ad9959_write_data(CSR_ADD, 1, CSR_DATA0, 0);
        ad9959_write_data(ACR_ADD, 3, ACR_DATA, 1);
    } else if (Channel == 1) {
        ad9959_write_data(CSR_ADD, 1, CSR_DATA1, 0);
        ad9959_write_data(ACR_ADD, 3, ACR_DATA, 1);
    } else if (Channel == 2) {
        ad9959_write_data(CSR_ADD, 1, CSR_DATA2, 0);
        ad9959_write_data(ACR_ADD, 3, ACR_DATA, 1);
    } else if (Channel == 3) {
        ad9959_write_data(CSR_ADD, 1, CSR_DATA3, 0);
        ad9959_write_data(ACR_ADD, 3, ACR_DATA, 1);
    }
}

/**
 * @brief  写入相位
 * @funna  ad9959_write_phase
 * @param  通道(channel), 相位(Phase)
 * @retval None
 */
void ad9959_write_phase(uint8_t Channel, uint16_t Phase) {
    uint16_t P_temp = 0;
    P_temp = (uint16_t)((float)Phase / 360 * 16384);
    CPOW0_DATA[1] = (uint8_t)P_temp;
    CPOW0_DATA[0] = (uint8_t)(P_temp >> 8);
    if (Channel == 0) {
        ad9959_write_data(CSR_ADD, 1, CSR_DATA0, 0);
        ad9959_write_data(CPOW0_ADD, 2, CPOW0_DATA, 1);
    } else if (Channel == 1) {
        ad9959_write_data(CSR_ADD, 1, CSR_DATA1, 0);
        ad9959_write_data(CPOW0_ADD, 2, CPOW0_DATA, 1);
    } else if (Channel == 2) {
        ad9959_write_data(CSR_ADD, 1, CSR_DATA2, 0);
        ad9959_write_data(CPOW0_ADD, 2, CPOW0_DATA, 1);
    } else if (Channel == 3) {
        ad9959_write_data(CSR_ADD, 1, CSR_DATA3, 0);
        ad9959_write_data(CPOW0_ADD, 2, CPOW0_DATA, 1);
    }
}

#ifdef RT_USING_FINSH
/* 控制指令: 9959_c <freq|amp|phase> <ch> <data> */
void ad9959_control_cmd(int argc, char **argv) {
    if (argc >= 4) {
        if (!rt_strcmp("freq", argv[1])) {
            ad9959_write_freq(atoi(argv[2]), atoi(argv[3]));
        } else if (!rt_strcmp("amp", argv[1])) {
            ad9959_write_amp(atoi(argv[2]), atoi(argv[3]));
        } else if (!rt_strcmp("phase", argv[1])) {
            ad9959_write_phase(atoi(argv[2]), atoi(argv[3]));
        }
    } else {
        rt_kprintf(
            "less argv\ncmd like : 9959_c <freq|amp|phase> <ch> <data>\n");
    }
}
MSH_CMD_EXPORT_ALIAS(ad9959_control_cmd, 9959_c,
                     9959_c < freq | amp | phase > <ch><data>);
#endif /* RT_USING_FINSH */
