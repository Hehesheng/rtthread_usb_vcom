#ifndef RT_CONFIG_H__
#define RT_CONFIG_H__

/* Automatically generated file; DO NOT EDIT. */
/* RT-Thread Configuration */

/* RT-Thread Kernel */

#define RT_NAME_MAX 8
/* RT_USING_SMP is not set */
#define RT_ALIGN_SIZE 4
/* RT_THREAD_PRIORITY_8 is not set */
#define RT_THREAD_PRIORITY_32
/* RT_THREAD_PRIORITY_256 is not set */
#define RT_THREAD_PRIORITY_MAX 32
#define RT_TICK_PER_SECOND 100
#define RT_USING_OVERFLOW_CHECK
#define RT_USING_HOOK
#define RT_USING_IDLE_HOOK
#define RT_IDEL_HOOK_LIST_SIZE 4
#define IDLE_THREAD_STACK_SIZE 1024
/* RT_USING_TIMER_SOFT is not set */
/* RT_DEBUG is not set */

/* Inter-Thread communication */

#define RT_USING_SEMAPHORE
#define RT_USING_MUTEX
#define RT_USING_EVENT
#define RT_USING_MAILBOX
#define RT_USING_MESSAGEQUEUE
/* RT_USING_SIGNALS is not set */

/* Memory Management */

#define RT_USING_MEMPOOL
#define RT_USING_MEMHEAP
/* RT_USING_NOHEAP is not set */
#define RT_USING_SMALL_MEM
/* RT_USING_SLAB is not set */
/* RT_USING_MEMHEAP_AS_HEAP is not set */
/* RT_USING_MEMTRACE is not set */
#define RT_USING_HEAP

/* Kernel Device Object */

#define RT_USING_DEVICE
/* RT_USING_DEVICE_OPS is not set */
/* RT_USING_INTERRUPT_INFO is not set */
#define RT_USING_CONSOLE
#define RT_CONSOLEBUF_SIZE 128
#define RT_CONSOLE_DEVICE_NAME "uart1"
#define RT_VER_NUM 0x40000
#define ARCH_ARM
#define ARCH_ARM_CORTEX_M
#define ARCH_ARM_CORTEX_M4
/* ARCH_CPU_STACK_GROWS_UPWARD is not set */

/* RT-Thread Components */

#define RT_USING_COMPONENTS_INIT
#define RT_USING_USER_MAIN
#define RT_MAIN_THREAD_STACK_SIZE 2048
#define RT_MAIN_THREAD_PRIORITY 20

/* C++ features */

/* RT_USING_CPLUSPLUS is not set */

/* Command shell */

#define RT_USING_FINSH
#define FINSH_THREAD_NAME "tshell"
#define FINSH_USING_HISTORY
#define FINSH_HISTORY_LINES 5
#define FINSH_USING_SYMTAB
#define FINSH_USING_DESCRIPTION
/* FINSH_ECHO_DISABLE_DEFAULT is not set */
#define FINSH_THREAD_PRIORITY 20
#define FINSH_THREAD_STACK_SIZE 4096
#define FINSH_CMD_SIZE 80
/* FINSH_USING_AUTH is not set */
#define FINSH_USING_MSH
#define FINSH_USING_MSH_DEFAULT
#define FINSH_USING_MSH_ONLY
#define FINSH_ARG_MAX 10

/* Device virtual file system */

/* RT_USING_DFS is not set */

/* Device Drivers */

#define RT_USING_DEVICE_IPC
#define RT_PIPE_BUFSZ 512
#define RT_USING_SERIAL
#define RT_SERIAL_USING_DMA
/* RT_USING_CAN is not set */
/* RT_USING_HWTIMER is not set */
/* RT_USING_CPUTIME is not set */
/* RT_USING_I2C is not set */
#define RT_USING_PIN
/* RT_USING_ADC is not set */
/* RT_USING_PWM is not set */
/* RT_USING_MTD_NOR is not set */
/* RT_USING_MTD_NAND is not set */
/* RT_USING_MTD is not set */
/* RT_USING_PM is not set */
/* RT_USING_RTC is not set */
/* RT_USING_SDIO is not set */
/* RT_USING_SPI is not set */
/* RT_USING_WDT is not set */
/* RT_USING_AUDIO is not set */

/* Using WiFi */

/* RT_USING_WIFI is not set */

/* Using USB */

/* RT_USING_USB_HOST is not set */
#define RT_USING_USB_DEVICE
#define RT_USBD_THREAD_STACK_SZ 4096
#define USB_VENDOR_ID 0x0FFE
#define USB_PRODUCT_ID 0x0001
/* RT_USB_DEVICE_COMPOSITE is not set */
/* _RT_USB_DEVICE_NONE is not set */
#define _RT_USB_DEVICE_CDC
/* _RT_USB_DEVICE_MSTORAGE is not set */
/* _RT_USB_DEVICE_HID is not set */
/* _RT_USB_DEVICE_WINUSB is not set */
#define RT_USB_DEVICE_CDC
#define RT_VCOM_TASK_STK_SIZE 2048
/* RT_VCOM_TX_USE_DMA is not set */
#define RT_VCOM_SERNO "32021919830108"
#define RT_VCOM_SER_LEN 14
#define RT_VCOM_TX_TIMEOUT 1000

/* POSIX layer and C standard library */

#define RT_USING_LIBC
/* RT_USING_PTHREADS is not set */

/* Network */

/* Socket abstraction layer */

/* RT_USING_SAL is not set */

/* light weight TCP/IP stack */

/* RT_USING_LWIP is not set */

/* Modbus master and slave stack */

/* RT_USING_MODBUS is not set */

/* AT commands */

/* RT_USING_AT is not set */

/* VBUS(Virtual Software BUS) */

/* RT_USING_VBUS is not set */

/* Utilities */

/* RT_USING_LOGTRACE is not set */
/* RT_USING_RYM is not set */
#define RT_USING_ULOG
/* ULOG_OUTPUT_LVL_A is not set */
/* ULOG_OUTPUT_LVL_E is not set */
/* ULOG_OUTPUT_LVL_W is not set */
/* ULOG_OUTPUT_LVL_I is not set */
#define ULOG_OUTPUT_LVL_D
#define ULOG_OUTPUT_LVL 7
#define ULOG_USING_ISR_LOG
#define ULOG_ASSERT_ENABLE
#define ULOG_LINE_BUF_SIZE 128
#define ULOG_USING_ASYNC_OUTPUT
#define ULOG_ASYNC_OUTPUT_BUF_SIZE 4096
#define ULOG_ASYNC_OUTPUT_BY_THREAD
#define ULOG_ASYNC_OUTPUT_THREAD_STACK 1024
#define ULOG_ASYNC_OUTPUT_THREAD_PRIORITY 30

/* log format */

#define ULOG_OUTPUT_FLOAT
#define ULOG_USING_COLOR
#define ULOG_OUTPUT_TIME
/* ULOG_TIME_USING_TIMESTAMP is not set */
#define ULOG_OUTPUT_LEVEL
#define ULOG_OUTPUT_TAG
/* ULOG_OUTPUT_THREAD_NAME is not set */
#define ULOG_BACKEND_USING_CONSOLE
/* ULOG_USING_FILTER is not set */
/* ULOG_USING_SYSLOG is not set */
#define ULOG_SW_VERSION_NUM 0x00101
/* RT_USING_UTEST is not set */

/* ARM CMSIS */

/* RT_USING_CMSIS_OS is not set */
/* RT_USING_RTT_CMSIS is not set */
/* RT_USING_LWP is not set */

/* RT-Thread online packages */

/* IoT - internet of things */

/* PKG_USING_PAHOMQTT is not set */
/* PKG_USING_WEBCLIENT is not set */
/* PKG_USING_WEBNET is not set */
/* PKG_USING_MONGOOSE is not set */
/* PKG_USING_WEBTERMINAL is not set */
/* PKG_USING_CJSON is not set */
/* PKG_USING_JSMN is not set */
/* PKG_USING_LJSON is not set */
/* PKG_USING_EZXML is not set */
/* PKG_USING_NANOPB is not set */

/* Wi-Fi */

/* Marvell WiFi */

/* PKG_USING_WLANMARVELL is not set */

/* Wiced WiFi */

/* PKG_USING_WLAN_WICED is not set */
/* PKG_USING_RW007 is not set */
/* PKG_USING_COAP is not set */
/* PKG_USING_NOPOLL is not set */
/* PKG_USING_NETUTILS is not set */
/* PKG_USING_AT_DEVICE is not set */
/* PKG_USING_WIZNET is not set */

/* IoT Cloud */

/* PKG_USING_ONENET is not set */
/* PKG_USING_GAGENT_CLOUD is not set */
/* PKG_USING_ALI_IOTKIT is not set */
/* PKG_USING_AZURE is not set */
/* PKG_USING_TENCENT_IOTKIT is not set */
/* PKG_USING_NIMBLE is not set */

/* security packages */

/* PKG_USING_MBEDTLS is not set */
/* PKG_USING_libsodium is not set */
/* PKG_USING_TINYCRYPT is not set */

/* language packages */

/* PKG_USING_LUA is not set */
/* PKG_USING_JERRYSCRIPT is not set */
/* PKG_USING_MICROPYTHON is not set */

/* multimedia packages */

/* PKG_USING_OPENMV is not set */
/* PKG_USING_MUPDF is not set */

/* tools packages */

#define PKG_USING_CMBACKTRACE
/* PKG_CMBACKTRACE_PLATFORM_M0_M0PLUS is not set */
/* PKG_CMBACKTRACE_PLATFORM_M3 is not set */
#define PKG_CMBACKTRACE_PLATFORM_M4
/* PKG_CMBACKTRACE_PLATFORM_M7 is not set */
/* PKG_CMBACKTRACE_PLATFORM_NOT_SELECTED is not set */
#define PKG_CMBACKTRACE_DUMP_STACK
#define PKG_CMBACKTRACE_PRINT_ENGLISH
/* PKG_CMBACKTRACE_PRINT_CHINESE is not set */
/* PKG_USING_CMBACKTRACE_V10202 is not set */
/* PKG_USING_CMBACKTRACE_V10200 is not set */
#define PKG_USING_CMBACKTRACE_LATEST_VERSION
/* PKG_USING_EASYFLASH is not set */
/* PKG_USING_EASYLOGGER is not set */
/* PKG_USING_SYSTEMVIEW is not set */
/* PKG_USING_RDB is not set */
/* PKG_USING_QRCODE is not set */
/* PKG_USING_ULOG_EASYFLASH is not set */
/* PKG_USING_ADBD is not set */

/* system packages */

/* PKG_USING_GUIENGINE is not set */
/* PKG_USING_PERSIMMON is not set */
/* PKG_USING_CAIRO is not set */
/* PKG_USING_PIXMAN is not set */
/* PKG_USING_LWEXT4 is not set */
/* PKG_USING_PARTITION is not set */
/* PKG_USING_FAL is not set */
/* PKG_USING_SQLITE is not set */
/* PKG_USING_RTI is not set */
/* PKG_USING_LITTLEVGL2RTT is not set */
/* PKG_USING_CMSIS is not set */
/* PKG_USING_DFS_YAFFS is not set */
/* PKG_USING_LITTLEFS is not set */

/* peripheral libraries and drivers */

/* PKG_USING_REALTEK_AMEBA is not set */
/* PKG_USING_SHT2X is not set */
/* PKG_USING_AHT10 is not set */
/* PKG_USING_AP3216C is not set */
/* PKG_USING_STM32_SDIO is not set */
/* PKG_USING_ICM20608 is not set */
/* PKG_USING_U8G2 is not set */
#define PKG_USING_BUTTON
#define SINGLE_AND_DOUBLE_TRIGGER
/* CONTINUOS_TRIGGER is not set */
#define LONG_FREE_TRIGGER
#define BUTTON_DEBOUNCE_TIME 1
#define BUTTON_CONTINUOS_CYCLE 1
#define BUTTON_LONG_CYCLE 1
#define BUTTON_DOUBLE_TIME 10
#define BUTTON_LONG_TIME 30
#define PKG_USING_BUTTON_V101
/* PKG_USING_BUTTON_V100 is not set */
/* PKG_USING_BUTTON_LATEST_VERSION is not set */
/* PKG_USING_MPU6XXX is not set */
/* PKG_USING_PCF8574 is not set */
/* PKG_USING_KENDRYTE_SDK is not set */

/* miscellaneous packages */

/* PKG_USING_LIBCSV is not set */
/* PKG_USING_OPTPARSE is not set */
/* PKG_USING_FASTLZ is not set */
/* PKG_USING_MINILZO is not set */
/* PKG_USING_QUICKLZ is not set */
/* PKG_USING_MULTIBUTTON is not set */
/* PKG_USING_CANFESTIVAL is not set */
/* PKG_USING_ZLIB is not set */
/* PKG_USING_DSTR is not set */
/* PKG_USING_TINYFRAME is not set */
/* PKG_USING_KENDRYTE_DEMO is not set */

/* samples: kernel and components samples */

/* PKG_USING_KERNEL_SAMPLES is not set */
/* PKG_USING_FILESYSTEM_SAMPLES is not set */
/* PKG_USING_NETWORK_SAMPLES is not set */
/* PKG_USING_PERIPHERAL_SAMPLES is not set */
/* PKG_USING_HELLO is not set */
/* PKG_USING_VI is not set */
#define SOC_FAMILY_STM32
#define SOC_SERIES_STM32F4

/* Hardware Drivers Config */

#define SOC_STM32F407ZG

/* Onboard Peripheral Drivers */

/* BSP_USING_USB_TO_USART is not set */
/* BSP_USING_COM2 is not set */
#define BSP_USING_COM3
/* BSP_USING_SPI_FLASH is not set */
/* BSP_USING_EEPROM is not set */
/* BSP_USING_ETH is not set */
/* BSP_USING_MPU6050 is not set */
/* BSP_USING_SDCARD is not set */

/* On-chip Peripheral Drivers */

#define BSP_USING_GPIO
#define BSP_USING_UART1
#define BSP_USING_UART3
/* BSP_USING_TIM is not set */
/* BSP_USING_PWM is not set */
/* BSP_USING_ON_CHIP_FLASH is not set */
/* BSP_USING_SPI1 is not set */
/* BSP_USING_SPI2 is not set */
/* BSP_SPI_USING_DMA is not set */
/* BSP_USING_ADC is not set */
/* BSP_USING_I2C1 is not set */
/* BSP_USING_ONCHIP_RTC is not set */
/* BSP_USING_WDT is not set */
/* BSP_USING_SDIO is not set */
/* BSP_USING_EXT_FMC_IO is not set */

/* Board extended module Drivers */

#define BOARD_USING_AD9959

#endif
