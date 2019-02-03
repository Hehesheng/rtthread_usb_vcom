from machine import Pin

led = Pin(("LED1", 7), Pin.OUT_PP)
key = Pin(("KEY", 68), Pin.IN, Pin.PULL_UP)  # 将第125号 Pin 设备设置为上拉输入模式
while True:
    if key.value():
        led.value(0)
    else:
        led.value(1)
