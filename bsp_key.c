#include "bsp_key.h"
#include "WYX_OLED.h"
#include "bell.h"
#include "bsp_led.h"
void KEY_GPIO_Config(void)
{
	GPIO_InitTypeDef    GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;											//KEY2
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
//-----------------------------------------------------	
	RCC_APB2PeriphClockCmd(KEY2_GPIO_CLK,ENABLE); 
	GPIO_InitStruct.GPIO_Pin = KEY2_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruct);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;				//KEY1
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	

	
}



 extern int mode;
 extern char select_line;
 u8 Key_State_2 = key_check;
 u8 Key_State_1 = key_check;
 u8 TimeCont_1 = 0;
 u8 TimeCont_2 = 0;


uint8_t scan()
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1 )
		{
			while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1 );
			return 1 ;
		}else return 0;
	}

	
	
extern char temperature_thresholed;
extern char humidity_threshold;
extern char gas_threshold;
extern char Warm_state;
extern char warm_lock;	
char lock_bell = 0;				   //专门锁关于自己按了警报的报警信息
void KEY1_Scan(void)
{
		 
    static u8 lock = 0;
    switch (Key_State_1)
    {
    case key_check:
        if (GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN)==KEY_ON)
        {
            Key_State_1 = key_confirm;
        }
        TimeCont_1  = 0;
        lock = 0;
        break;
    case key_confirm:
        if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN )==KEY_ON)
        {
            if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN )==KEY_ON)
            {
               lock = 1;
               TimeCont_1 ++;
            }
        }else               //若不按了，就要开始检查是长按还是短按！
        {
            if(lock)
            {
                if(TimeCont_1  > 10)
                {
										if(lock_bell == 0)
										{
                    bell_on();                    //长按了
										lock_bell = 1;							
										}
                }else
                {
									if(lock_bell == 1)
									{

										bell_off();		
										lock_bell = 0;
									}
									if(warm_lock == 2)
									{
										warm_lock = 3;
										bell_off();								   //短按了，这个关铃声如果关不了那就是没发完信息。    
									}
										
                    if(mode == 2)
                    {
                        switch(select_line)
												{
													case 1:
													temperature_thresholed += 5;	
													break;
													case 2:
													humidity_threshold += 5;	
													break;
													case 3:
													gas_threshold +=10;	
													break;	
												}
                    }              
                    TimeCont_1  = 0;              
                }
                Key_State_1 = key_release;
            }
            else{
                Key_State_1 = key_check;
            }
        }
        break;
				
        case key_release:
        if( GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN )==KEY_OFF)
        {
           Key_State_1 = key_check;
            OLED_Clear();

        }
        break;

        default:
        break;
    }
}









void KEY2_Scan(void)
{

    static u8 lock = 0;
    switch (Key_State_2)
    {
    case key_check:
        if (GPIO_ReadInputDataBit(KEY2_GPIO_PORT,KEY2_GPIO_PIN )==KEY_ON)
        {
            Key_State_2 = key_confirm;
        }
        TimeCont_2 = 0;
        lock = 0;
        break;

    case key_confirm:
        if(GPIO_ReadInputDataBit(KEY2_GPIO_PORT,KEY2_GPIO_PIN )==KEY_ON)
        {
            if(GPIO_ReadInputDataBit(KEY2_GPIO_PORT,KEY2_GPIO_PIN )==KEY_ON)
            {
                lock = 1;
                TimeCont_2++;
            }
        }else               //若不按了，就要开始检查是长按还是短按！
        {
            if(lock)
            {
                if(TimeCont_2 > 10)
                {
                    mode++;                        //长按了
                    TimeCont_2 = 0;
                }else
                { 
                    if(mode == 2)
                    {
                        select_line++;
                    }              
                     TimeCont_2 = 0;              //短按了
                }
                Key_State_2 = key_release;
            }
            else{
                Key_State_2 = key_check;
            }
        }
        break;
				
        case key_release:
        if( GPIO_ReadInputDataBit(KEY2_GPIO_PORT,KEY2_GPIO_PIN )==KEY_OFF)
        {
            Key_State_2 = key_check;
            OLED_Clear();

        }
        break;

        default:
        break;
    }
}



uint8_t Key_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin) 
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)== KEY_ON)
	{
		//松手检测
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)== KEY_ON);
		return KEY_ON;
	}
	else return KEY_OFF;
}






