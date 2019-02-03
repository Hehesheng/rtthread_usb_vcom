#include "board_ad9954.h"

//系统频率fosc（外部晶振频率），系统频率=fosc*M
#define fosc (40)                   //晶振频率
#define PLL_MULTIPLIER (10)         // PLL倍频数（4--20）
#define fs (fosc * PLL_MULTIPLIER)  //系统时钟频率
// double fH_Num=11.2204;
double fH_Num = 10.73741824;
// double fH_Num=11.3671588397205;//
// double fH_Num = 11.3025455157895;
// double fH_Num=17.17986918;

// static void delay_ms(uint32_t t) { rt_thread_delay(t / 10); }

static void delay_us(uint32_t t) {
    int32_t i;

    for (i = t * 40; i > 0; i--) {
    }
}

static void __nop(void) { __ASM volatile("nop"); }

void GPIO_AD9954_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    /*Configure GPIO pins : AD9954_RESET_Pin AD9954_SCLK_Pin AD9954_SDIO_Pin
       AD9954_P0_Pin AD9954_UPDATE_Pin */
    GPIO_InitStruct.Pin = AD9954_RESET_Pin | AD9954_SCLK_Pin | AD9954_SDIO_Pin |
                          AD9954_P0_Pin | AD9954_UPDATE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    AD9954_IOSY = 0;
    AD9954_OSK_RESET;
    AD9954_PWR = 0;
}

/*********************************************************************************************************
** 函数名称 ：void AD9954_RESET(void)
** 函数功能 ：复位AD9954
** 函数说明 ：不复位也可以
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************/
void AD9954_RESET(void) {
    AD9954_CS = 0;
    AD9954_RES_RESET;
    AD9954_RES_SET;
    delay_us(100);
    AD9954_RES_RESET;
    AD9954_CS = 0;
    AD9954_SCLK_RESET;
    AD9954_PS0_RESET;
    AD9954_PS1_RESET;
    AD9954_UPDATE_RESET;
    AD9954_CS = 1;
}

/*********************************************************************************************************
** 函数名称 ：void AD9954_Send_Byte(uint8_t dat)
** 函数功能 ：往AD9954发送一个字节的内容
** 函数说明 ：AD9954的传输速度最大为25M，所以不加延时也可以
** 入口参数 ：待发送字节
** 出口参数 ：无
*********************************************************************************************************/
void AD9954_Send_Byte(uint8_t dat) {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        AD9954_SCLK_RESET;
        if (dat & 0x80) {
            AD9954_SDIO_SET;
        } else {
            AD9954_SDIO_RESET;
        }
        __nop();
        __nop();
        __nop();
        // delay_us(10);

        AD9954_SCLK_SET;
        __nop();
        __nop();
        __nop();
        // delay_us(10);
        dat <<= 1;
    }
    __nop();
    __nop();
    __nop();
    delay_us(10);
    AD9954_SCLK_RESET;
}

uint8_t AD9954_Read_Byte(void) {
    uint8_t i, dat = 0;

    for (i = 0; i < 8; i++) {
        AD9954_SCLK_RESET;
        dat |= AD9954_SDO;
        AD9954_SCLK_SET;
        dat <<= 1;
    }

    return dat;
}

/*********************************************************************************************************
** 函数名称 ：void IO_Update(void)
** 函数功能 ：产生一个更新信号，更新AD9954内部寄存器，
** 函数说明 ：可以不加任何延时
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************/
void IO_Update(void) {
    AD9954_UPDATE_RESET;
    //	delay_us(100);
    AD9954_UPDATE_SET;
    delay_us(200);
    AD9954_UPDATE_RESET;
}

void Update_Sync(void) {
    AD9954_UPDATE_RESET;
    //	delay_us(100);
    AD9954_UPDATE_SET;
    delay_us(200);
    AD9954_UPDATE_RESET;
}

//读寄存器
uint32_t Read_Vau1(void) {
    uint32_t ret;
    //    char i;
    AD9954_CS = 1;
    AD9954_Send_Byte(CFR1);

    ret = AD9954_Read_Byte();
    ret <<= 8;
    ret += AD9954_Read_Byte();
    ret <<= 8;
    ret += AD9954_Read_Byte();
    ret <<= 8;
    ret += AD9954_Read_Byte();
    AD9954_CS = 0;

    return ret;
}

uint32_t Read_Vau2(void) {
    uint32_t ret;
    //    char i;
    AD9954_CS = 1;
    AD9954_Send_Byte(CFR2);
    ret = AD9954_Read_Byte();
    ret <<= 8;
    ret += AD9954_Read_Byte();
    ret <<= 8;
    ret += AD9954_Read_Byte();
    AD9954_CS = 0;
    return ret;
}

/*********************************************************************************************************
** 函数名称 ：void AD9954_Init(void))
** 函数功能 ：初始化AD9954的管脚和最简单的内部寄存器的配置，
** 函数说明
*：默认关掉比较器的电源，因板上的晶振为100MHz，最大采用了4倍频，为400M
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************/

int AD9954_Init(void) {
    GPIO_AD9954_Init();
    AD9954_RESET();
    // delay_ms(300);

    AD9954_CS = 0;
    // // single tone
    // AD9954_Send_Byte(CFR1);  //地址0写操作
    // AD9954_Send_Byte(0x02);  //
    // AD9954_Send_Byte(0x10);
    // AD9954_Send_Byte(0x00);
    // AD9954_Send_Byte(0x00);  //比较器power down
    //                          //	AD9954_Send_Byte(0x00);//比较器使能
    // IO_Update();

    AD9954_Send_Byte(CFR2);  //地址1写操作
    AD9954_Send_Byte(0x00);  //
    AD9954_Send_Byte(0x00);
#if fs > 400
#error "系统频率超过芯片最大值"
#elif fs >= 250
    AD9954_Send_Byte(PLL_MULTIPLIER << 3 | 0x04 | 0X03);
#else
    AD9954_Send_Byte(PLL_MULTIPLIER << 3);
#endif
    IO_Update();
    //	AD9954_Send_Byte(0x24);//4倍频，打开VCO控制高位，系统时钟倍频后为400M

    Write_ASF(0x0FFF);
    //输出幅度设置函数，输入参数为0-3FFF，对应输出幅度0-480mv
    AD9954_Set_Freq(10650000);
    AD9954_CS = 1;

    return 0;
}
MSH_CMD_EXPORT_ALIAS(AD9954_Init, 9954_init, init ad9954);
INIT_DEVICE_EXPORT(AD9954_Init);

uint32_t Get_FTW(double Real_fH) { return (uint32_t)(fH_Num * Real_fH); }

/*********************************************************************************************************
** 函数名称 ：void AD9954_Set_Freq(float f)
** 函数功能 ：设置AD9954当前的频率输出，采用的是单一频率输出
** 函数说明
*：因为采用的浮点数进行计算，转换过程中会出现误差，通过调整可以精确到0.1Hz以内
** 入口参数 ：欲设置的频率值
** 出口参数 ：无
*********************************************************************************************************/
void AD9954_Set_Freq(double f)  // single tone
{
    uint32_t date;
    AD9954_CS = 0;

    date = Get_FTW(f);                        // date=(f/fclk)x2^32=10.7374xf
    AD9954_Send_Byte(FTW0);                   // FTW0地址
    AD9954_Send_Byte((uint8_t)(date >> 24));  //频率控制字
    AD9954_Send_Byte((uint8_t)(date >> 16));
    AD9954_Send_Byte((uint8_t)(date >> 8));
    AD9954_Send_Byte((uint8_t)date);
    IO_Update();
    AD9954_CS = 1;
    //	fH_Num+=0.001;
}

void DirectSwitchPSK(float f, float phase1, float phase2, float phase3,
                     float phase4) {
    uint16_t date;
    AD9954_CS = 0;
    AD9954_UPDATE_RESET;
    delay_us(1);
    AD9954_Send_Byte(0x00);  //地址0写操作
    AD9954_Send_Byte(0x00);  //打开RAM控制位驱动FTW
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x40);
    //
    AD9954_Send_Byte(0x01);  //地址1写操作
    AD9954_Send_Byte(0x00);  //
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0xA4);  // 4倍频，打开VCO控制高位，系统时钟倍频后为400M
    AD9954_Set_Freq(f);  //载波频率
    AD9954_PS0_RESET;
    AD9954_PS1_RESET;
    AD9954_Send_Byte(0x07);
    AD9954_Send_Byte(0x01);  // ramp rate=0x0010
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);  // final address:0x000
    AD9954_Send_Byte(0x00);  // start address:0x000;
    AD9954_Send_Byte(0x00);  // RAM0工作于模式0,不停留位没有激活
    IO_Update();
    date = 45.51 * phase1;
    date = date << 2;
    AD9954_Send_Byte(0x0b);
    AD9954_Send_Byte((uint8_t)(date >> 8));  //频率控制字
    AD9954_Send_Byte((uint8_t)date);
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);

    AD9954_PS0_SET;  // ram1
    AD9954_PS1_RESET;

    AD9954_Send_Byte(0x08);
    AD9954_Send_Byte(0x01);  // ramp rate=0x0010
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x01);  // final address:0x0001
    AD9954_Send_Byte(0x04);  // start address:0x0001
    AD9954_Send_Byte(0x00);  // RAM1工作于模式0,不停留位没有激活
    IO_Update();
    date = 45.51 * phase2;
    date = date << 2;
    AD9954_Send_Byte(0x0b);
    AD9954_Send_Byte((uint8_t)(date >> 8));  //频率控制字
    AD9954_Send_Byte((uint8_t)date);
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);

    AD9954_PS0_RESET;  // ram2
    AD9954_PS1_SET;

    AD9954_Send_Byte(0x09);
    AD9954_Send_Byte(0x01);  // ramp rate=0x0010
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x02);  // final address:0x0002
    AD9954_Send_Byte(0x08);  // start address:0x0002
    AD9954_Send_Byte(0x00);
    IO_Update();
    date = 45.51 * phase3;
    date = date << 2;
    AD9954_Send_Byte(0x0b);
    AD9954_Send_Byte((uint8_t)(date >> 8));  //频率控制字
    AD9954_Send_Byte((uint8_t)date);
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);
    AD9954_PS0_SET;  // ram3
    AD9954_PS1_SET;
    AD9954_Send_Byte(0x0a);
    AD9954_Send_Byte(0x01);  // ramp rate=0x0010
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x03);  // final address:0x0003
    AD9954_Send_Byte(0x0c);  // start address:0x0003
    AD9954_Send_Byte(0x00);
    IO_Update();
    date = 45.51 * phase4;
    date = date << 2;
    AD9954_Send_Byte(0x0b);
    AD9954_Send_Byte((uint8_t)(date >> 8));  //频率控制字
    AD9954_Send_Byte((uint8_t)date);
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);

    AD9954_Send_Byte(0x00);  //地址0写操作
    AD9954_Send_Byte(0xa0);  //打开RAM控制位驱动FTW
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x40);
    AD9954_CS = 1;
    IO_Update();
}

/*********************************************************************************************************
** 函数名称 ：void Generate_PSK(float f,uint16_t *phase)
** 函数功能 ：PSK调制信号输出
** 函数说明 ：通过RAM端来控制信号的相位，从而实现PSK信号的调制
**            这里使用一个RAM段的循环模式，PSK信号为8为数据位。
**            RAM的更新速度由ram
*rate定时寄存器控制，寄存器为16位，实现对系统提供的100M时钟1~65535分频，写0是无效的
**			  当跟输出PSK载波的频率是ram
*rate跟新频率的整数倍时，才能保证每次的相位改变一致
** 入口参数 ：float f   ：PSK载波的频率
** 			  uint16_t *phase:
*写入相位累加器的相位值，180实现对相位翻转，0不改变相位，PSK信号为8位，需为8位的数组
** 出口参数 ：无
*********************************************************************************************************/
void Generate_PSK(float f, uint16_t *phase) {
    uint8_t i;
    uint16_t date;
    AD9954_CS = 0;
    AD9954_UPDATE_RESET;

    AD9954_Send_Byte(0x00);  //地址0写操作
    AD9954_Send_Byte(0x00);  //打开RAM控制位驱动FTW
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x40);
    //
    AD9954_Send_Byte(0x01);  //地址1写操作
    AD9954_Send_Byte(0x00);  //
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0xA4);  // 4倍频，打开VCO控制高位，系统时钟倍频后为400M
    AD9954_Set_Freq(f);  //载波频率

    AD9954_Send_Byte(0x04);  // FTW0地址
    AD9954_Send_Byte(0x00);  //频率控制字
    AD9954_Send_Byte(0x10);
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);
    IO_Update();

    AD9954_PS0_RESET;
    AD9954_PS1_RESET;
    AD9954_Send_Byte(0x07);
    AD9954_Send_Byte(0x00);  // ramp rate=0x0400
    AD9954_Send_Byte(0x04);
    AD9954_Send_Byte(0x07);  // final address:0x007
    AD9954_Send_Byte(0x00);  // start address:0x000;
    AD9954_Send_Byte(0x80);  // RAM0工作于模式4,不停留位没有激活
    IO_Update();

    AD9954_Send_Byte(0x0b);
    for (i = 0; i < 8; i++) {
        date = 45.51 * phase[i];
        date = date << 2;
        AD9954_Send_Byte((uint8_t)(date >> 8));  //频率控制字
        AD9954_Send_Byte((uint8_t)date);
        AD9954_Send_Byte(0x00);
        AD9954_Send_Byte(0x00);
    }
    AD9954_Send_Byte(0x00);  //地址0写操作
    AD9954_Send_Byte(0xc0);  //打开RAM控制位驱动FTW
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x40);
    AD9954_CS = 1;
    IO_Update();
}

/*********************************************************************************************************
** 函数名称 ：void Generate_FM(uint32_t *fre)
** 函数功能 ：FM调制信号输出
** 函数说明 ：通过RAM端来控制信号的频率，从而实现FM信号的调制
**            这里使用一个RAM段的循环模式，PSK信号为8为数据位。
**            RAM的更新速度由ram
*rate定时寄存器控制，寄存器为16位，实现对系统提供的100M时钟1~65535分频，写0是无效的
**			  ram
*rate的跟新速度为调制信号的频率，这里如1KHz，写入ram rate的值为0X0C35,
** 入口参数 ：uint32_t
**fre：FM信号频率值，这里采用的是32点频率采样，调用此函数之前需先设置好频率表的值，其按正弦规律改变
**            for(i=0;i<32;i++)
**				{
**					fre[i]=Fc+Fshift*sin(wt);
**				}
** 出口参数 ：无
*********************************************************************************************************/
void Generate_FM(uint32_t *fre) {
    uint8_t i;
    uint32_t date;
    AD9954_CS = 0;
    AD9954_UPDATE_RESET;

    AD9954_Send_Byte(0x00);  //地址0写操作
    AD9954_Send_Byte(0x00);  //打开RAM控制位驱动FTW
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x40);

    AD9954_Send_Byte(0x01);  //地址1写操作
    AD9954_Send_Byte(0x00);  //
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x24);  // 4倍频，打开VCO控制高位，系统时钟倍频后为400M

    AD9954_PS0_RESET;
    AD9954_PS1_RESET;
    AD9954_Send_Byte(0x07);
    AD9954_Send_Byte(0x35);  // ramp rate=32kHz
    AD9954_Send_Byte(0x0C);
    AD9954_Send_Byte(0x1F);  // final address:0x000
    AD9954_Send_Byte(0x00);  // start address:0x000;
    AD9954_Send_Byte(0x80);  // RAM0工作于模式0,不停留位没有激活
    IO_Update();

    AD9954_Send_Byte(0x0b);
    for (i = 0; i < 32; i++) {
        date = 10.73741 * fre[i];
        AD9954_Send_Byte((uint8_t)(date >> 24));  //频率控制字
        AD9954_Send_Byte((uint8_t)(date >> 16));
        AD9954_Send_Byte((uint8_t)(date >> 8));
        AD9954_Send_Byte((uint8_t)date);
    }
    AD9954_Send_Byte(0x00);  //地址0写操作
    AD9954_Send_Byte(0x80);  //打开RAM控制位驱动FTW
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x40);
    AD9954_CS = 1;
    IO_Update();
}

#include <math.h>
#define AD9954_RAM_SIZE (1024)
#define PI_DATA (3.1415926)
#define GET_SIN_NUM(i) ((i)*2 * PI_DATA / (AD9954_RAM_SIZE * 1.0))
#define RAMP_SPEED_NUM(f) (fs * 1000000.0 / 4.0 / (f) / AD9954_RAM_SIZE)

uint32_t fsk_f0_freq = 10650000, fsk_fp_freq = 50000;

void ad9954_change_freq_ram_config(uint32_t f0, uint32_t fp) {
    uint32_t temp = 0;
    double freq = 0;

    AD9954_PS0_RESET;
    AD9954_PS1_RESET;
    AD9954_SCLK_RESET;

    AD9954_Send_Byte(0x00);  //地址0写操作
    AD9954_Send_Byte(0x00);  //关闭RAM控制位驱动FTW
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);
    IO_Update();

    // AD9954_Init();

    delay_us(100);
    // AD9954_Send_Byte(0x07);
    // AD9954_Send_Byte(0xFF);
    // AD9954_Send_Byte(0xFF);
    // AD9954_Send_Byte(0xFF);  // final address:0x3FF
    // AD9954_Send_Byte(0x03);  // start address:0x000;
    // AD9954_Send_Byte(0x80);  // RAM0工作于模式0,不停留位没有激活
    // IO_Update();

    AD9954_Send_Byte(0x0b);
    for (size_t i = 0; i < AD9954_RAM_SIZE; i++) {
        freq = f0 + fp * sin(GET_SIN_NUM(i));
        temp = Get_FTW(freq);
        AD9954_Send_Byte((uint8_t)(temp >> 24));  //频率控制字
        AD9954_Send_Byte((uint8_t)(temp >> 16));
        AD9954_Send_Byte((uint8_t)(temp >> 8));
        AD9954_Send_Byte((uint8_t)temp);
    }
    // IO_Update();

    AD9954_Send_Byte(0x00);  //地址0写操作
    AD9954_Send_Byte(0x80);  //打开RAM控制位驱动FTW
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);
    IO_Update();
    // Write_ASF(0x3FFF);
}

static int write_sine_freq_data_to_ram(void) {
    ad9954_change_freq_ram_config(fsk_f0_freq, fsk_fp_freq);

    return 0;
}
INIT_APP_EXPORT(write_sine_freq_data_to_ram);
MSH_CMD_EXPORT_ALIAS(write_sine_freq_data_to_ram, 9954_test, fsk begin);

static void change_ramp_speed(int argc, char **argv) {
    uint16_t temp = 0;
    uint32_t input_freq;

    AD9954_PS0_RESET;
    AD9954_PS1_RESET;
    if (argc > 1) {
        input_freq = atoi(argv[1]);
        if (input_freq > 97000 || input_freq < 2) {
            temp = 0xFFFF;
        } else {
            temp = (uint16_t)RAMP_SPEED_NUM(input_freq);
        }
        AD9954_Send_Byte(0x07);
        AD9954_Send_Byte((uint8_t)temp);
        AD9954_Send_Byte((uint8_t)(temp >> 8));
        AD9954_Send_Byte(0xFF);  // final address:0x3FF
        AD9954_Send_Byte(0x03);  // start address:0x000;
        AD9954_Send_Byte(0x80);  // RAM0工作于模式0,不停留位没有激活
    } else {
        AD9954_Send_Byte(0x07);
        AD9954_Send_Byte(0xFF);
        AD9954_Send_Byte(0xFF);
        AD9954_Send_Byte(0xFF);  // final address:0x3FF
        AD9954_Send_Byte(0x03);  // start address:0x000;
        AD9954_Send_Byte(0x80);  // RAM0工作于模式0,不停留位没有激活
    }
    IO_Update();
}
MSH_CMD_EXPORT_ALIAS(change_ramp_speed, 9954_change, none);

void ad9954_change_speed(double input_freq) {
    uint16_t temp = 0;

    if (input_freq > 97000 || input_freq < 1.5) {
        temp = 0xFFFF;
    } else {
        temp = (uint16_t)RAMP_SPEED_NUM(input_freq);
    }
    AD9954_Send_Byte(0x07);
    AD9954_Send_Byte((uint8_t)temp);
    AD9954_Send_Byte((uint8_t)(temp >> 8));
    AD9954_Send_Byte(0xFF);  // final address:0x3FF
    AD9954_Send_Byte(0x03);  // start address:0x000;
    AD9954_Send_Byte(0x80);  // RAM0工作于模式0,不停留位没有激活
    IO_Update();
}

/*********************************************************************************************************
** 函数名称 ：void ASK_Init(void)
** 函数功能 ：ASK调制信号输出初始化
** 函数说明 ：ASK调制，需要先设置好载波频率，然后改变DAC比例因子 scale
*factor其为14位，通过设置为最大和0两种值实现ASK信号的调制 *
*调用Write_ASF(uint16_t factor)来改变幅值
** 入口参数 ：无
** 出口参数 ：无
******************************************************
****************************************************/
void ASK_Init(void) {
    AD9954_CS = 0;
    AD9954_Send_Byte(0x00);  //地址0写操作
    AD9954_Send_Byte(0x02);  //手动打开OSK打开
    AD9954_Send_Byte(0x00);

    AD9954_Send_Byte(0x20);  //当受到UPDATE信号，自动清楚相位累加器
                             //// 	AD9954_Send_Byte(0x00);//不改变任何

    AD9954_Send_Byte(0x40);

    //控制相位偏移，因默认输出是余弦波，所以需控制相位累加器的累加相位为270
    AD9954_Send_Byte(0X05);
    AD9954_Send_Byte(0X30);
    AD9954_Send_Byte(0X00);

    AD9954_CS = 1;
    IO_Update();
}
/*********************************************************************************************************
** 函数名称 ：void AM_Init(void)
** 函数功能 ：AM调制信号输出初始化
** 函数说明 ：AM调制，需要先设置好载波频率，然后改变DAC比例因子 scale
*factor其为14位,AM的幅度值按正弦规律变换，可以实现AM
**            最高可以采用32点采样
**			  调用Write_ASF(uint16_t factor)来改变幅值
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************/
void AM_Init(void) {
    AD9954_CS = 0;
    AD9954_Send_Byte(0x00);  //地址0写操作
    AD9954_Send_Byte(0x02);  //手动打开OSK打开
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);  //不改变任何
    AD9954_Send_Byte(0x40);

    AD9954_CS = 1;
    IO_Update();
}
/*********************************************************************************************************
** 函数名称 ：void Write_ASF(uint16_t factor)
** 函数功能 ：改变scale factor数值，改变DAC输出幅度
** 函数说明 ：写入最大为0X3FFF,最小为0
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************/
void Write_ASF(uint16_t factor)  // 2ASK
{
    AD9954_CS = 0;
    /*	AD9954_Send_Byte(0x00);//地址0写操作
    AD9954_Send_Byte(0x02);//手动打开OSK打开
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x20);//当受到UPDATE信号，自动清楚相位累加器
    //
    //	AD9954_Send_Byte(0x00);//不改变任何
    AD9954_Send_Byte(0x40);
    //控制相位偏移
    AD9954_Send_Byte(0X05);
    AD9954_Send_Byte(0X30);
    AD9954_Send_Byte(0X00);	  */

    AD9954_Send_Byte(0x02);  //幅度
    AD9954_Send_Byte(factor >> 8);
    AD9954_Send_Byte(factor);
    AD9954_CS = 1;
    IO_Update();
}
/******************************************************************
 *  名    称：Linear_Sweep
 *  功    能：AD9954扫频模式输出
 *  参    数：MinFreq：扫频下限频率，MaxFreq：扫频上限频率
 *            UpStepFreq：向上扫频步进，UpStepTime：向上扫频的跳频时间
 *            DownStepFreq：向下扫频步进，DownStepTime：向下扫频的跳频时间
 *            Mode：扫频模式
 *  返 回 值：无 Linear_Sweep(100, 100000000, 100,  250,   100,250, 1);
 *  说    明：与频率相关的参数的单位都是 MHz
 *            Mode=DoubleScan(双边扫频)时，PS0=1，即为向上扫频,但配置了
 *            向下扫频的寄存器,可以直接通过PS0_WriteBit()来改变扫频方向
 ******************************************************************/
void Linear_Sweep(double MinFreq, double MaxFreq, double UpStepFreq,
                  uint8_t UpStepTime, double DownStepFreq, uint8_t DownStepTime,
                  ScanMode Mode)  // 0向下扫频模式，1向上扫频模式，2双边扫频

{
    uint32_t FTW_Vau;
    AD9954_PS0_RESET;
    AD9954_PS1_RESET;
    AD9954_CS = 0;
    AD9954_Send_Byte(CFR1);
    AD9954_Send_Byte(0x02);
    AD9954_Send_Byte(0x20);  // Linear_Sweep Enable
    AD9954_Send_Byte(0);
    if (Mode & No_Dwell)
        AD9954_Send_Byte(0x44);  // Comp Power-Down & Linear Sweep No Dwell
    else
        AD9954_Send_Byte(0x40);  // Comp Power-Down

    //写入FTW0----最小频率
    FTW_Vau = Get_FTW(MinFreq);
    ;
    AD9954_Send_Byte(FTW0);
    AD9954_Send_Byte(FTW_Vau >> 24);
    AD9954_Send_Byte(FTW_Vau >> 16);
    AD9954_Send_Byte(FTW_Vau >> 8);
    AD9954_Send_Byte(FTW_Vau);

    //写入FTW1----最大频率
    FTW_Vau = Get_FTW(MaxFreq);
    AD9954_Send_Byte(FTW1);
    AD9954_Send_Byte(FTW_Vau >> 24);
    AD9954_Send_Byte(FTW_Vau >> 16);
    AD9954_Send_Byte(FTW_Vau >> 8);
    AD9954_Send_Byte(FTW_Vau);

    Mode &= 0x7F;

    if (Mode != UpScan) {
        //写入NLSCW----下降频率步进和跳频时间
        FTW_Vau = Get_FTW(DownStepFreq);
        AD9954_Send_Byte(NLSCW);
        AD9954_Send_Byte(DownStepTime);  //跳频时间（DownStepTime个？？周期）
        AD9954_Send_Byte(FTW_Vau >> 24);  //频率步进
        AD9954_Send_Byte(FTW_Vau >> 16);
        AD9954_Send_Byte(FTW_Vau >> 8);
        AD9954_Send_Byte(FTW_Vau);
        AD9954_PS0_RESET;  //下降扫频
    }
    if (Mode != DownScan) {
        //写入PLSCW----上升频率步进和跳频时间
        FTW_Vau = Get_FTW(UpStepFreq);
        AD9954_Send_Byte(PLSCW);
        AD9954_Send_Byte(UpStepTime);  //跳频时间（0XFF个？？周期）
        AD9954_Send_Byte(FTW_Vau >> 24);  //频率步进
        AD9954_Send_Byte(FTW_Vau >> 16);
        AD9954_Send_Byte(FTW_Vau >> 8);
        AD9954_Send_Byte(FTW_Vau);
        AD9954_PS0_SET;  //上升扫频
    }
    AD9954_CS = 1;
    IO_Update();
}

/******************************************************************
 *  名    称：PS0_WriteBit
 *  功    能：设置PS0的状态
 *  参    数：BitVal：PS0状态
 *  返 回 值：无
 *  说    明：PS0可以控制AD9954扫频方向
 ******************************************************************/
void PS0_WriteBit(uint8_t BitVal) {
    if (BitVal)
        AD9954_PS0_SET;
    else
        AD9954_PS0_RESET;
}

/*********************************************************************************************************
** 函数名称 ：void DirectSwitchFSK(float f1,float f2,float f3,float f4)
** 函数功能 ：四相FSK信号输出
** 函数说明
*：在四个RAM区各设置了一个频率值，通过改变PS0和PS1的电平选择对应的RAM端输出相应的频率值来实现FSK，也可以实现二项的FSK；
** 通过设置定时器中断控制PS0,PS1管脚的电平就可以将二进制的编码转化为FSK信号输出
** 入口参数 ：float f1:频率1
** 	      float f2:频率2
** 	      float f3:频率3
** 	      float f4:频率4
** 隐含控制   			PS0: 0   1	0	 1
** 管脚参数： 			PS1: 0   0	1	 1
** 对应控制 RAM段：		 0   1	2	 3
** 出口参数 ：无
*********************************************************************************************************/
void DirectSwitchFSK(double f1, double f2, double f3, double f4) {
    uint32_t FTW_Vau;
    AD9954_CS = 0;

    AD9954_Send_Byte(0x00);  //地址0写操作
    AD9954_Send_Byte(0x00);  //打开RAM控制位驱动FTW
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x40);

    AD9954_Send_Byte(0x01);  //地址1写操作
    AD9954_Send_Byte(0x00);  //
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0xA4);  // 8倍频，打开VCO控制高位，系统时钟倍频后为400M
    AD9954_PS0_RESET;
    AD9954_PS1_RESET;
    AD9954_Send_Byte(0x07);
    AD9954_Send_Byte(0x01);  // ramp rate=0x0010
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);  // final address:0x000
    AD9954_Send_Byte(0x00);  // start address:0x000;
    AD9954_Send_Byte(0x00);  // RAM0工作于模式0,不停留位没有激活
    IO_Update();
    FTW_Vau = Get_FTW(f1);
    AD9954_Send_Byte(0x0b);
    AD9954_Send_Byte(FTW_Vau >> 24);  //频率步进
    AD9954_Send_Byte(FTW_Vau >> 16);
    AD9954_Send_Byte(FTW_Vau >> 8);
    AD9954_Send_Byte(FTW_Vau);

    AD9954_PS0_SET;  // ram1
    AD9954_PS1_RESET;

    AD9954_Send_Byte(0x08);
    AD9954_Send_Byte(0x01);  // ramp rate=0x0010
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x01);  // final address:0x0001
    AD9954_Send_Byte(0x04);  // start address:0x0001
    AD9954_Send_Byte(0x00);  // RAM1工作于模式0,不停留位没有激活
    IO_Update();
    FTW_Vau = Get_FTW(f2);
    AD9954_Send_Byte(0x0b);
    AD9954_Send_Byte(FTW_Vau >> 24);  //频率步进
    AD9954_Send_Byte(FTW_Vau >> 16);
    AD9954_Send_Byte(FTW_Vau >> 8);
    AD9954_Send_Byte(FTW_Vau);

    AD9954_PS0_RESET;  // ram2
    AD9954_PS1_SET;

    AD9954_Send_Byte(0x09);
    AD9954_Send_Byte(0x01);  // ramp rate=0x0010
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x02);  // final address:0x0002
    AD9954_Send_Byte(0x08);  // start address:0x0002
    AD9954_Send_Byte(0x00);
    IO_Update();
    FTW_Vau = Get_FTW(f3);
    AD9954_Send_Byte(0x0b);
    AD9954_Send_Byte(FTW_Vau >> 24);  //频率步进
    AD9954_Send_Byte(FTW_Vau >> 16);
    AD9954_Send_Byte(FTW_Vau >> 8);
    AD9954_Send_Byte(FTW_Vau);
    AD9954_PS0_SET;  // ram3
    AD9954_PS1_SET;
    AD9954_Send_Byte(0x0a);
    AD9954_Send_Byte(0x01);  // ramp rate=0x0010
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x03);  // final address:0x0003
    AD9954_Send_Byte(0x0c);  // start address:0x0003
    AD9954_Send_Byte(0x00);
    IO_Update();
    FTW_Vau = Get_FTW(f4);
    AD9954_Send_Byte(0x0b);
    AD9954_Send_Byte(FTW_Vau >> 24);  //频率步进
    AD9954_Send_Byte(FTW_Vau >> 16);
    AD9954_Send_Byte(FTW_Vau >> 8);
    AD9954_Send_Byte(FTW_Vau);

    AD9954_Send_Byte(0x00);  //地址0写操作
    AD9954_Send_Byte(0x80);  //打开RAM控制位驱动FTW
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x40);
    AD9954_CS = 1;
    IO_Update();
}
/*********************************************************************************************************
** 函数名称 ：void Line_AR_Sweep(float f1,float f2)
** 函数功能 ：线性扫面输出模式
** 函数说明 ：使频率按预置的模式线性扫描上去，详细参见官方PDF
** 入口参数 ：float f1:起始频率
** 			  float f2:终止频率
** 出口参数 ：无
*********************************************************************************************************/
void Line_AR_Sweep(float f1, float f2)  // linear sweep mode
{
    uint32_t date;
    AD9954_SCLK_RESET;
    AD9954_RES_RESET;
    AD9954_UPDATE_RESET;
    AD9954_PS0_RESET;
    AD9954_PS1_RESET;
    AD9954_CS = 0;

    // linear sweep mode
    AD9954_Send_Byte(0x00);  //地址0写操作
    AD9954_Send_Byte(0x00);  //
    AD9954_Send_Byte(0x20);
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x40);

    AD9954_Send_Byte(0x01);  //地址1写操作
    AD9954_Send_Byte(0x00);  //
    AD9954_Send_Byte(0x00);
    //	AD9954_Send_Byte(0x24);//4倍频，打开VCO控制高位，系统时钟倍频后为400M
    AD9954_Send_Byte(PLL_MULTIPLIER << 3 | 0x04);
    date = 10.7374 * f1;
    AD9954_Send_Byte(0x04);                   // FTW0地址
    AD9954_Send_Byte((uint8_t)(date >> 24));  //频率控制字
    AD9954_Send_Byte((uint8_t)(date >> 16));
    AD9954_Send_Byte((uint8_t)(date >> 8));
    AD9954_Send_Byte((uint8_t)date);
    date = 10.7374 * f2;
    AD9954_Send_Byte(0x06);                   // FTW1地址
    AD9954_Send_Byte((uint8_t)(date >> 24));  //频率控制字
    AD9954_Send_Byte((uint8_t)(date >> 16));
    AD9954_Send_Byte((uint8_t)(date >> 8));
    AD9954_Send_Byte((uint8_t)date);
    AD9954_Send_Byte(0x07);  // NLSCW
    AD9954_Send_Byte(0x01);
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x1b);
    AD9954_Send_Byte(0xf4);
    AD9954_Send_Byte(0x08);  // RLSCW
    AD9954_Send_Byte(0x01);
    AD9954_Send_Byte(0x00);
    AD9954_Send_Byte(0x01);
    AD9954_Send_Byte(0xa3);
    AD9954_Send_Byte(0x6f);

    AD9954_CS = 1;
    IO_Update();
}

void ad9954_set_freq(int argc, char **argv) {
    if (argc > 1) {
        AD9954_Set_Freq(atoi(argv[1]));
    }
}
MSH_CMD_EXPORT_ALIAS(ad9954_set_freq, 9954_set, set ad9954 output freq);