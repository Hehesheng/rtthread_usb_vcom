#include <board.h>
#include <rtdevice.h>
#include <rtthread.h>
#include "drv_gpio.h"
#define LOG_TAG "btn"  //该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL \
    LOG_LVL_DBG  //该模块对应的日志输出级别。不定义时，默认：调试级别
#include <button.h>
#include <math.h>
#include <stdlib.h>
#include <stm32f4xx.h>
#include <ulog.h>  //必须在 LOG_TAG 与 LOG_LVL 下面
#include <shell.h>

#define KEY0_PIN GET_PIN(E, 4)
#define KEY1_PIN GET_PIN(E, 3)

rt_device_t vcom_dev = RT_NULL;
rt_device_t uart_dev = RT_NULL;

static uint8_t key0_read_level(void) { return rt_pin_read(KEY0_PIN); }

static void key0_press_callback(void *btn) { rt_kprintf("key0 pressed\n"); }

static void key0_double_press_callback(void *btn) {
    rt_kprintf("key0 double pressed\n");
}

static void key0_long_press_callback(void *btn) {
    rt_kprintf("key0 long pressed\n");
    rt_kprintf("console change to vcom\n");
    finsh_set_device("vcom");
    rt_console_set_device("vcom");
    rt_device_close(vcom_dev);
    rt_device_open(vcom_dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_STREAM |
                                 RT_DEVICE_FLAG_INT_RX);
    rt_kprintf("console change to vcom\n");
}

static uint8_t key1_read_level(void) { return rt_pin_read(KEY1_PIN); }

static void key1_press_callback(void *btn) { rt_kprintf("key1 pressed\n"); }

static void key1_double_press_callback(void *btn) {
    rt_kprintf("key1 double pressed\n");
}

static void key1_long_press_callback(void *btn) {
    rt_kprintf("key1 long pressed\n");
    rt_kprintf("console change to uart1\n");
    finsh_set_device(RT_CONSOLE_DEVICE_NAME);
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
    rt_device_close(uart_dev);
    rt_device_open(uart_dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_STREAM |
                                 RT_DEVICE_FLAG_INT_RX);
    rt_kprintf("console change to uart1\n");
}

void button_cycle_check_thread(void *parameter) {
    Button_t key0, key1;

    rt_pin_mode(KEY0_PIN, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(KEY1_PIN, PIN_MODE_INPUT_PULLUP);

    Button_Create("key0", &key0, key0_read_level, 0);
    Button_Create("key1", &key1, key1_read_level, 0);

    Button_Attach(&key0, BUTTON_DOWM, key0_press_callback);
    Button_Attach(&key0, BUTTON_DOUBLE, key0_double_press_callback);
    Button_Attach(&key0, BUTTON_LONG, key0_long_press_callback);

    Button_Attach(&key1, BUTTON_DOWM, key1_press_callback);
    Button_Attach(&key1, BUTTON_DOUBLE, key1_double_press_callback);
    Button_Attach(&key1, BUTTON_LONG, key1_long_press_callback);

    vcom_dev = rt_device_find("vcom");
    uart_dev = rt_device_find(RT_CONSOLE_DEVICE_NAME);

    while (1) {
        Button_Process();
        rt_thread_mdelay(50);
    }
}
