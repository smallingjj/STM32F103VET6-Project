#include "bell.h"
#include "stm32f10x.h"
#include "./uart/bsp_uart.h"
#include "./systick/bsp_SysTick.h"
#include "./gsm_gprs/bsp_gsm_usart.h"
#include "./gsm_gprs/bsp_gsm_gprs.h"
#include "stm32f10x.h"
extern char temperature_thresholed;
extern char  humidity_threshold;
extern char  gas_threshold;

extern char show_tmp;
extern char show_humi;
extern uint16_t smokeDensity;
extern char mode;
extern char Warm_state;
char warm_lock = 0;

char warm_part = 0;						//表示触发警报的类型


void Motor_Init()
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE); 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;  
	
	GPIO_Init(GPIOE, &GPIO_InitStruct);

}




void Warming_Scan()
{
	if(warm_lock == 0)
	{
		if(mode == 1)
		{
			if((show_tmp > temperature_thresholed) && (show_tmp < 100))
			{
				bell_on();
				Warm_state = 0;								//指示灯变红！！
				warm_lock = 1;
				warm_part = 1;				
				GPIO_SetBits(GPIOE,GPIO_Pin_15);			//风扇会打开
			}else if((show_humi > humidity_threshold) && (show_humi < 100))
			{
				bell_on();
				Warm_state = 0;
				warm_lock = 1;
				warm_part = 2;
				GPIO_SetBits(GPIOA,GPIO_Pin_0);
				GPIO_SetBits(GPIOE,GPIO_Pin_15);
			}
			else if((smokeDensity/41 > gas_threshold)  && (smokeDensity/41 < 100))
			{
				bell_on();
				Warm_state = 0;
				warm_lock = 1;								//warmlock= 1时进行gsm发信，信息发完后立刻将其变为2，在1的时候只有gsm可以对其进行更改！！
				warm_part = 3;
				GPIO_SetBits(GPIOE,GPIO_Pin_15);
			}
		}
	}	
}

void sentmessage()
{
	if(warm_lock == 1)
	{
		GSM_Sent();
		warm_lock = 2;
		warm_part = 0;
	}
}



void Warm_unlock()
{
	if(warm_lock == 3)
	{
		if(show_tmp < temperature_thresholed && show_humi < humidity_threshold && smokeDensity/41 < gas_threshold)
		{
			warm_lock = 0;
			GPIO_ResetBits(GPIOE,GPIO_Pin_15);
		}
	}else if(warm_lock == 2)
	{
		if(show_tmp < temperature_thresholed && show_humi < humidity_threshold && smokeDensity/41 < gas_threshold)
		{warm_lock = 0;
			bell_off();
		}
	}
}




void bell(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(BELL_GPIO_CLK,ENABLE); 	
	GPIO_InitStruct.GPIO_Pin = BELL_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(BELL_Green_GPIO_PORT, &GPIO_InitStruct);//这句很重要
}


void bell_on()
{
	BELL_Green_GPIO_PORT->ODR = BELL_ON ;
}

void bell_off()
{
	BELL_Green_GPIO_PORT->ODR = BELL_OFF ;
}
