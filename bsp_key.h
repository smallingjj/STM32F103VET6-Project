#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f10x.h"

#define KEY1_GPIO_PIN               GPIO_Pin_0
#define KEY1_GPIO_PORT              GPIOA
#define KEY1_GPIO_CLK						    RCC_APB2Periph_GPIOA


#define KEY_ON      1
#define KEY_OFF     0
#define key_check    3
#define key_confirm  4
#define key_release  5
//---------------------------------------------------------------------

#define KEY2_GPIO_PIN          GPIO_Pin_13
#define KEY2_GPIO_PORT				 GPIOC 
#define KEY2_GPIO_CLK					 RCC_APB2Periph_GPIOC

uint8_t scan(void);
void Key1_Init(void);
//---------------------------------------------------------------------

void KEY_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);
uint8_t Keyb_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);
void KEY1_Scan(void);
void KEY2_Scan(void);
void Touch(void);
#endif
