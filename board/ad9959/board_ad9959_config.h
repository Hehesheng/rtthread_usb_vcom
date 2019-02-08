#ifndef __AD9959_CONFIG_H__
#define __AD9959_CONFIG_H__

/* AD9959 IO Config */
#define AD9959_RST_Pin GPIO_PIN_6
#define AD9959_RST_GPIO_Port GPIOF
#define AD9959_UPDATE_Pin GPIO_PIN_7
#define AD9959_UPDATE_GPIO_Port GPIOF
#define AD9959_CS_Pin GPIO_PIN_8
#define AD9959_CS_GPIO_Port GPIOF
#define AD9959_SCK_Pin GPIO_PIN_9
#define AD9959_SCK_GPIO_Port GPIOF
#define AD9959_SDIO_Pin GPIO_PIN_10
#define AD9959_SDIO_GPIO_Port GPIOF

/* If using different gpio ports set to 1 */
#define AD9959_USING_DIFFERENT_PORT (0)

#if (AD9959_USING_DIFFERENT_PORT == 0)
#define AD9959_GPIO_Port GPIOF
#else
#define AD9959_CLOCK_ENABLE() \
    do {                      \
    } while (0)
#endif

#endif  // __AD9959_CONFIG_H
