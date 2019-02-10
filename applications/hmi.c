#include <board.h>
#include <rtdevice.h>
#include <rtthread.h>
#include "drv_gpio.h"
#define LOG_TAG "hmi"  //该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL \
    LOG_LVL_DBG  //该模块对应的日志输出级别。不定义时，默认：调试级别
#include <stdlib.h>
#include <ulog.h>  //必须在 LOG_TAG 与 LOG_LVL 下面

#define HMI_THREAD_STACK_SIZE (2048)
#define HMI_THREAD_PRIORITY (20)

static rt_sem_t hmi_sem = RT_NULL;
static rt_device_t hmi_port = RT_NULL;

#define HMI_CMD_LEN (30)
#define CHANGE_FO (1)
#define CHANGE_FP (2)
/* hmi指令解释 */
static void hmi_cmd_analysis(const char *cmd) {
    char cache_buff[HMI_CMD_LEN] = {0};
    uint32_t res = 0, temp_f0 = 0, temp_fp = 0;
    const char cmd_list[][5] = {{"f0"}, {"fp"}, {0}};
    const char hmi_cmd_end[] = {"\xff\xff\xff"};

    extern uint32_t fsk_f0_freq, fsk_fp_freq;
    extern void ad9954_change_freq_ram_config(uint32_t f0, uint32_t fp);

    log_d("hmi rec cmd: %s", cmd);
    for (int i = 0;; i++) {
        if (cmd_list[i][0] == 0) break;
        if (rt_strncmp(cmd_list[i], cmd, rt_strlen(cmd_list[i])) == 0) {
            res = i + 1;
            break;
        }
    }
    /* 指令运行 */
    switch (res) {
        case CHANGE_FO:
            temp_f0 = atoi(cmd + rt_strlen(cmd_list[res - 1]) + 1);
            if (temp_f0 > 10000000 && temp_f0 < 12000000) {
                fsk_f0_freq = temp_f0;
                ad9954_change_freq_ram_config(fsk_f0_freq, fsk_fp_freq);

                sprintf(cache_buff, "f0.txt=\"%.3fKHz\"",
                        (double)fsk_f0_freq / 1000);
                rt_device_write(hmi_port, 0, cache_buff, rt_strlen(cache_buff));
                rt_device_write(hmi_port, 0, hmi_cmd_end,
                                rt_strlen(hmi_cmd_end));
            }
            break;

        case CHANGE_FP:
            temp_fp = atoi(cmd + rt_strlen(cmd_list[res - 1]) + 1);
            if (temp_fp >= 1000 && temp_fp <= 10000000) {
                fsk_fp_freq = temp_fp;
                ad9954_change_freq_ram_config(fsk_f0_freq, fsk_fp_freq);

                sprintf(cache_buff, "fp.txt=\"%.3fKHz\"",
                        (double)fsk_fp_freq / 1000);
                rt_device_write(hmi_port, 0, cache_buff, rt_strlen(cache_buff));
                rt_device_write(hmi_port, 0, hmi_cmd_end,
                                rt_strlen(hmi_cmd_end));
            }
            break;

        default:
            log_w("wrong cmd");
            break;
    }
}

void hmi_thread(void *parameter) {
    char cmd_buff[HMI_CMD_LEN] = {0};
    uint32_t i = 0;

    hmi_port = rt_device_find("uart3");

    while (1) {
        rt_sem_take(hmi_sem, RT_WAITING_FOREVER);
        rt_device_read(hmi_port, 0, cmd_buff, 1);
        /* 接收到开始符 */
        if (cmd_buff[0] == 0xFE) {
            while (1) {
                if (rt_sem_take(hmi_sem, 100) != RT_ETIMEOUT)
                    rt_device_read(hmi_port, 0, cmd_buff + i, 1);
                else {
                    log_w("get end cmd timeout!");
                    i = 0;
                    break;
                }
                /* 接收到结束符 */
                if (cmd_buff[i] == 0xFF) {
                    cmd_buff[i] = 0; /* 给最后一位写0 */
                    break;
                }
                i++;
            }
            if (i) { /* 接收到了有效字符解析并执行指令 */
                hmi_cmd_analysis(cmd_buff);
            }
            /* 清空指令缓存 */
            for (i = 0; i < HMI_CMD_LEN; i++) {
                cmd_buff[i] = 0;
            }
            i = 0;
        }
    }
}

static rt_err_t uart3_input(rt_device_t dev, rt_size_t size) {
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(hmi_sem);

    return RT_EOK;
}

static int hmi_create(void) {
    rt_thread_t tid = RT_NULL;
    rt_device_t serial = RT_NULL;
    // char str[] = {"uart3 test\r\n"};

    /* 打开串口3 */
    serial = rt_device_find("uart3");
    if (serial != RT_NULL) {
        /* 以读写及中断接收方式打开串口设备 */
        rt_device_open(serial, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
        // rt_device_write(serial, 0, str, rt_strlen(str));  // test
        /* 初始化信号量通信 */
        hmi_sem = rt_sem_create("shmi", 0, RT_IPC_FLAG_PRIO);
        /* 设置钩子函数 */
        rt_device_set_rx_indicate(serial, uart3_input);
    } else
        log_e("uart3 open fail");

    /* 创建线程 */
    tid = rt_thread_create("thmi", hmi_thread, RT_NULL, HMI_THREAD_STACK_SIZE,
                           HMI_THREAD_PRIORITY, 20);
    if (tid != RT_NULL) {
        rt_thread_startup(tid);
    } else
        log_e("hmi create fail");

    return 0;
}
INIT_APP_EXPORT(hmi_create);
