#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_key.h"
#include "bell.h"
#include "dsp_adc.h"
#include "WYX_OLED.h"
#include "./dht11/bsp_dht11.h"
#include "./usart/bsp_usart.h"
#include "./gsm_gprs/bsp_gsm_usart.h"
#include "./systick/bsp_SysTick.h"
#include "./gsm_gprs/bsp_gsm_gprs.h"
#include "./key/bsp_key.h" 
#include "bsp_sdfs_app.h"
#include <string.h>

extern int led_state ;

void Delay(uint32_t count)
{
	for(;count!=0;count--);
}




void OLED(void)
{
    Mode_Change();
    Select_change();
    line_one_show();
    line_two_show();
    line_three_show();
}



int main(void)
{	

	Motor_Init();			//电机驱动初始化
  GSM_Init();
	LED_GPIO_Config();
	DHT11_Data_TypeDef DHT11_Data;

//	DEBUG_UART_Config();
 	DHT11_Init ();
	KEY_GPIO_Config();
	bell();										//先把警告给关掉好调试！！
	Init_Smoke();

	OLED_Init();
	OLED_SHOW1();
  printf("\r\n***欢迎来到居家老人传感系统！！***\r\n");
	
		while(1)
	{		

			led_change();
		  KEY1_Scan();
			KEY2_Scan();
			OLED();
			super_smoke();
		  Warming_Scan();
			sentmessage();
			Warm_unlock();
   			dht11_analyse(& DHT11_Data);
			if( DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS)
			{
//   				printf("\r\n湿度为%d.%d ％ ，温度为 %d.%d℃ \r\n",\
//  				DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
			}			
			else
			{
//				printf("Read DHT11 ERROR!\r\n");
			}		
			
		}

}
