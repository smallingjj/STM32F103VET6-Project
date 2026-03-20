#include "bsp_led.h"
#include "stm32f10x.h"


void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(LED_G_GPIO_CLK,ENABLE); 
	GPIO_InitStruct.GPIO_Pin = LED_GREEN_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(LED_Green_GPIO_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 ;
	GPIO_Init(LED_Green_GPIO_PORT, &GPIO_InitStruct); 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 ;
	GPIO_Init(LED_Green_GPIO_PORT, &GPIO_InitStruct); 
}

char Warm_state = 2;				//这里设置成跟上面初始化一样才行！！

void LED_TURN_RED(void)				//
{
		GPIO_ResetBits(GPIOB, LED_RED_PIN);
    GPIO_SetBits(GPIOB, LED_GREEN_PIN);
    GPIO_SetBits(GPIOB, LED_BLUE_PIN); 

}

void LED_TURN_GREEN(void)
{
	  GPIO_ResetBits(GPIOB, LED_GREEN_PIN);
    GPIO_SetBits(GPIOB, LED_RED_PIN);
    GPIO_SetBits(GPIOB, LED_BLUE_PIN); 
}

void LED_TURN_BLUE(void)
{
	
GPIO_ResetBits(GPIOB, LED_BLUE_PIN);
    GPIO_SetBits(GPIOB, LED_RED_PIN);
   GPIO_SetBits(GPIOB, LED_GREEN_PIN); 
}

extern char warm_lock;
void led_change()
{
	warm_state_change();
	switch(Warm_state)
	{
		case 0:
		LED_TURN_RED();
		break;
		case 1:
		LED_TURN_BLUE();
		break;			
		case 2:
		LED_TURN_GREEN();
		break;		
	}

}

void warm_state_change()
{
	switch(warm_lock)
	{
		case 0:
		Warm_state = 2;
		break;
		case 1:
		Warm_state = 1;
		break;			
		case 3:
		Warm_state = 1;
		break;		
	}
}






