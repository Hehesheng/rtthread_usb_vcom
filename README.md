# STM32F407 Template 说明

基于冬令营训练时使用的代码改动

加了SRAM, ad9959, ad9954, ads1256半成品驱动

开启了usb的虚拟串口功能,原版BSP库usb驱动有一点小问题,没有drv_usb.c,于是我移植了别的BSP的drv_usbd.c,能用(迫真)

加了button库,CmBacktrace库

这个工程上传作为一个模板,希望有人能和我交流一下