
#include "board_ads1256.h"
#include <stdlib.h>

// static void delayad(unsigned tt) {
//     unsigned char i, j;
//     for (j = tt; j > 0; j--)
//         for (i = 100; i > 0; i--)
//             ;
// }

static void delay_us(uint32_t t) {
    unsigned long i = 500;
    while (i--)
        ;
}

/* 应答函数,防死机 1:应答失败 */
uint32_t wait_drdy_call(void) {
    rt_tick_t timeout = 0, current_time = 0;

    current_time = rt_tick_get();
    while (DRDY) {
        timeout = rt_tick_get();
        if (timeout - current_time > 10) {
            log_w("ad1256 timeout");
            return 1;
        }
    }

    return 0;
}

/*******************************************
函数名称：ADS1256_Init
功    能：初始化SPI总线
参    数：无
返回值  ：无
********************************************/
int ADS1256_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(
        GPIOD,
        AD1256_RESET_Pin | AD1256_CS_Pin | AD1256_DIN_Pin | AD1256_SCLK_Pin,
        GPIO_PIN_RESET);

    /*Configure GPIO pins : AD1256_RESET_Pin AD1256_CS_Pin AD1256_DIN_Pin
     * AD1256_SCLK_Pin */
    GPIO_InitStruct.Pin =
        AD1256_RESET_Pin | AD1256_CS_Pin | AD1256_DIN_Pin | AD1256_SCLK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : AD1256_DRDY_Pin AD1256_DOUT_Pin */
    GPIO_InitStruct.Pin = AD1256_DRDY_Pin | AD1256_DOUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    CS_SET;
    delay_us(2);
    SCLK_RESET;
    delay_us(3);
    DIN_RESET;
    delay_us(3);
    RESET_RESET;
    delay_us(3);
    RESET_SET;
    delay_us(3);
    CS_RESET;
    delay_us(1);
    if (wait_drdy_call()) return 0;
    CS_SET;
    delay_us(1);
    CS_RESET;
    if (wait_drdy_call()) return 0;
    ADS1256_SendCommand(ADS1256_CMD_RESET);
    ADS1256_WriteToRegister(ADS1256_STATUS, 0x06);
    // ADS1256_WriteToRegister(ADS1256_MUX,0x08);
    ADS1256_WriteToRegister(ADS1256_ADCON, 0x00);
    ADS1256_WriteToRegister(ADS1256_DRATE, ADS1256_DRATE_30SPS);
    //	ADS1256_WriteToRegister(ADS1256_MUX,
    // ADS1256_MUXP_AIN0|ADS1256_MUXN_AINCOM);//设置通道
    ADS1256_SendCommand(ADS1256_CMD_SYNC);    //同步校准
    ADS1256_SendCommand(ADS1256_CMD_WAKEUP);  //唤醒
    CS_SET;
    SCLK_RESET;
    DIN_RESET;

    return 0;
}
MSH_CMD_EXPORT_ALIAS(ADS1256_Init, 1256_init, adc init);
INIT_DEVICE_EXPORT(ADS1256_Init);

/*******************************************
函数名称：ADS1256_WriteByte
功    能：向SPI总线写一个字节数据
参    数：date-一个字节数据
返回值  ：无
********************************************/
void ADS1256_WriteByte(uint8_t date) {
    uint8_t i;

    for (i = 0; i < 8; i++) {
        if (date & 0x80) {
            SCLK_SET;
            DIN_SET;
        } else {
            SCLK_SET;
            DIN_RESET;
        }
        date = date << 1;
        delay_us(2);
        SCLK_RESET;
        delay_us(2);
    }
}

/*******************************************
函数名称：ADS1256_ReadByte
功    能：从SPI总线读一个字节数据
参    数：无
返回值  ：所读的一个字节
********************************************/
uint8_t ADS1256_ReadByte() {
    uint8_t a = 0;
    uint8_t i, dat = 0;
    //    ADS1256_CS_CLR;
    for (i = 0; i < 8; i++) {
        dat <<= 1;
        SCLK_SET;
        delay_us(1);
        if (DOUT) a = 1;
        delay_us(1);
        SCLK_RESET;
        if (DOUT) a = 1;
        if (DOUT) a = 1;
        delay_us(2);
        if (a == 1) dat |= 1;
        a = 0;
    }
    //    ADS1256_CS_SET;
    return dat;
}

/*******************************************
函数名称：ADS1256_SendCommand
功    能：向ADS1256发送一条命令
参    数：Command——要发送的命令
返回值  ：无
********************************************/
void ADS1256_SendCommand(uint8_t Command) {
    //	CS_0;
    delay_us(1);
    if (wait_drdy_call()) return;  // DRDY为低的时候才可以写数据
    ADS1256_WriteByte(Command);
    //	CS_1;
    // delay_us(2);
}

/*******************************************
函数名称：ADS1256_WriteToRegister
功    能：写指定的寄存器的值
参    数：Address----寄存器地址
                                        Data	 ----要写入的数据
返回值  ：无
********************************************/
void ADS1256_WriteToRegister(uint8_t Address, uint8_t Data) {
    //	CS_0;
    if (wait_drdy_call()) return;       // DRDY为低的时候才可以写数据
    ADS1256_WriteByte(Address | 0x50);  //发送寄存器地址
    if (wait_drdy_call()) return;
    ADS1256_WriteByte(0x00);  //只写一个寄存器
    if (wait_drdy_call()) return;
    ADS1256_WriteByte(Data);  //发送数据
    //	CS_1;
}

/*******************************************
函数名称：ADS1256_GetData
功    能：获取指定通道的数据
参    数：Channal	----通道编码
返回值  ：uint32_t----所获得的数据
********************************************/
uint32_t ADS1256_GetData(uint32_t Channal) {
    uint8_t Index;
    uint32_t Data = 0;
    uint8_t Buffer[3];
    CS_RESET;
    if (wait_drdy_call()) return 0;
    CS_SET;
    rt_thread_mdelay(1);
    CS_RESET;
    if (wait_drdy_call()) return 0;
    CS_SET;
    rt_thread_mdelay(1);
    CS_RESET;
    if (wait_drdy_call()) return 0;
    ADS1256_SendCommand(ADS1256_CMD_RESET);
    ADS1256_SendCommand(ADS1256_CMD_SYNC);          //同步校准
    ADS1256_SendCommand(ADS1256_CMD_WAKEUP);        //唤醒
    ADS1256_WriteToRegister(ADS1256_MUX, Channal);  //设置通道

    //读取数据
    ADS1256_WriteByte(ADS1256_CMD_RDATA);
    delay_us(12);
    for (Index = 0; Index < 3; Index++) Buffer[Index] = ADS1256_ReadByte();
    CS_SET;
    // Buffer[0] = Buffer[0] & 0x01f;
    Data = (Buffer[0] << 16) + (Buffer[1] << 8) + Buffer[2];  //奇奇怪怪的写法
    return Data;
}

void adc_get(int argc, char **argv) {
    uint32_t res = 0;

    if (argc > 2) {
        res = ADS1256_GetData((atoi(argv[1]) << 4) | atoi(argv[2]));
    } else {
        res = ADS1256_GetData(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);
    }
    rt_kprintf("Res: 0X%X\n", res);
}
MSH_CMD_EXPORT(adc_get, adc_get<0 - 8><0 - 8> default : 0 8);

#define THREAD_PRIORITY 25
#define THREAD_STACK_SIZE 512
#define THREAD_TIMESLICE 5

/* 线程 1 的入口函数 */
static void thread1_entry(void *parameter) {
    int32_t res = 0;

    while (1) {
        res = ADS1256_GetData(ADS1256_MUXP_AIN3 | ADS1256_MUXN_AINCOM);
        /* 线程 1 采用低优先级运行，一直打印计数值 */
        rt_kprintf("RES: 0x%X\n", res);

        rt_kprintf("vol: %d uv\n",
                   (int)(res * 1.0 / 8388608.0 * 2 * 2499970.0));

        rt_thread_mdelay(1000);
    }
}

/* 线程示例 */
int thread_sample(void) {
    static rt_thread_t tid1 = RT_NULL;

    if (tid1 == RT_NULL) {
        /* 创建线程 1，名称是 thread1，入口是 thread1_entry*/
        tid1 = rt_thread_create("thread1", thread1_entry, RT_NULL,
                                THREAD_STACK_SIZE, THREAD_PRIORITY,
                                THREAD_TIMESLICE);

        /* 如果获得线程控制块，启动这个线程 */
        if (tid1 != RT_NULL) rt_thread_startup(tid1);
    } else {
        rt_thread_delete(tid1);
        tid1 = RT_NULL;
    }

    return 0;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(thread_sample, thread sample);