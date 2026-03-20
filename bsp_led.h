#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f10x.h"

#define LED_GREEN_PIN          GPIO_Pin_0
#define LED_Green_GPIO_PORT         GPIOB
#define LED_G_GPIO_CLK							RCC_APB2Periph_GPIOB

#define LED_RED_PIN          	GPIO_Pin_5

#define LED_BLUE_PIN          	GPIO_Pin_1

void LED_GPIO_Config(void);


#define LED_G_TOGGLE       {LED_Green_GPIO_PORT  -> ODR ^=LED_Green_GPIO_PIN;}


void led_change(void);
void LED_TURN_BLUE(void);
void LED_TURN_GREEN(void);
void LED_TURN_RED(void);
void warm_state_change(void);
#endif
