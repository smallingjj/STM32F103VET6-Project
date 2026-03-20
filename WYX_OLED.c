#include "WYX_OLED.h"
#include "stm32f10x.h"
#include "OLED_Font.h"
#include "./dht11/bsp_dht11.h"
#include "bsp_led.h"
unsigned int mode = 1;
char Device_Type = 1;
char select_line = 1;
/*引脚配置*/
#define OLED_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)(x))
#define OLED_W_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)(x))


char temperature_thresholed = 50;
char humidity_threshold = 90;
char gas_threshold = 60;

/*引脚初始化*/
void OLED_I2C_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2C开始
  * @param  无
  * @retval 无
  */
void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}

/**
  * @brief  I2C停止
  * @param  无
  * @retval 无
  */
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2C发送一个字节
  * @param  Byte 要发送的一个字节
  * @retval 无
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(Byte & (0x80 >> i));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);	//额外的一个时钟，不处理应答信号
	OLED_W_SCL(0);
}

/**
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x7A);		//从机地址
	OLED_I2C_SendByte(0x00);		//写命令
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x7A);		//从机地址
	OLED_I2C_SendByte(0x40);		//写数据
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//设置Y位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置低4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置高4位
}

/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 132; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}

/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}



void OLED_ShowChinese(uint8_t Line,uint8_t Column,uint8_t num)
{
	uint8_t i;
	OLED_SetCursor((Line-1)*2,(Column-1)*16);
	for(i=0;i<16;i++)
	{
		OLED_WriteData(Font_Chinese[2*num][i]);
	}
	OLED_SetCursor((Line-1)*2+1,(Column-1)*16);
	for(i=0;i<16;i++)
	{
		OLED_WriteData(Font_Chinese[2*num+1][i]);
	}
}

void OLED_ShowChinese14(uint8_t Line,uint8_t Column,uint8_t num)
{
	uint8_t i;
	OLED_SetCursor((Line-1)*2,(Column-1)*14);
	for(i=0;i<14;i++)
	{
		OLED_WriteData(Font_Chinese[2*num][i]);
	}
	OLED_SetCursor((Line-1)*2+1,(Column-1)*14);
	for(i=0;i<14;i++)
	{
		OLED_WriteData(Font_Chinese[2*num+1][i]);
	}
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char PI[])
{  
 unsigned int j=0;
 unsigned char x,y;
  
 // if(y1%8==0) y=y1/8;      
 // else y=y1/8+1;
 for(y=y0;y<y1;y++)
 {
  OLED_SetCursor(y,x0);  //第一个设置y,第二个设置x//设置光标位置左上角(0,0) 往下0-7 ， 往右0-127
    for(x=x0;x<x1;x++)
     {      
      OLED_WriteData(PI[j++]);      //写数据
     }
 }
} 




/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//端口初始化
	
	OLED_WriteCommand(0xAE);	//关闭显示
	
	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//设置显示偏移
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//设置显示开始行
	
	OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//设置对比度控制
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

	OLED_WriteCommand(0xA6);	//设置正常/倒转显示

	OLED_WriteCommand(0x8D);	//设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//开启显示
		
	OLED_Clear();				//OLED清屏
}


//-------------------------------------------------------------------------------------------------

void OLED_SHOW1()			//主界面
{
	OLED_ShowChinese(1,1,57);
	OLED_ShowChinese(1,2,58);
	OLED_ShowChinese(1,3,59);
	OLED_ShowChinese(1,4,60);
	OLED_ShowChinese(1,5,61);
	OLED_ShowChinese(1,6,62);
	OLED_ShowChinese(1,7,63);
	OLED_ShowChinese(1,8,64);


	OLED_ShowChinese(2,1,65);		//开关
	OLED_ShowChinese(2,2,66);
	OLED_ShowChinese(2,3,27);

	OLED_ShowChinese(3,1,67);
	OLED_ShowChinese(3,2,68);
	OLED_ShowChinese(3,3,27);

	OLED_ShowChinese(4,1,69);
	OLED_ShowChinese(4,2,70);
	OLED_ShowChinese(4,3,71);
	OLED_ShowChinese(4,4,72);
	OLED_ShowChinese(4,5,27);

}


void OLED_SHOW_SET()			//设置界面
{
	OLED_ShowChinese(1,3,19);
	OLED_ShowChinese(1,4,20);
	OLED_ShowChinese(1,5,21);			//设置模式
	OLED_ShowChinese(1,6,22);

	
//设备类型和设备序号跟上面一致

	OLED_ShowChinese(2,1,65);		//开关
	OLED_ShowChinese(2,2,66);
	OLED_ShowChinese(2,3,27);

	OLED_ShowChinese(3,1,67);
	OLED_ShowChinese(3,2,68);
	OLED_ShowChinese(3,3,27);

	OLED_ShowChinese(4,1,69);
	OLED_ShowChinese(4,2,70);
	OLED_ShowChinese(4,3,71);
	OLED_ShowChinese(4,4,72);
	OLED_ShowChinese(4,5,27);
}

void extra_show(void)
{
	OLED_ShowChinese(1,3,19);
	OLED_ShowChinese(1,4,20);
	OLED_ShowChinese(1,5,21);			//设置模式
	OLED_ShowChinese(1,6,22);

	OLED_ShowChinese(2,1,9);
	OLED_ShowChinese(2,2,10);
	OLED_ShowChinese(2,3,11);
	OLED_ShowChinese(2,4,12);

	OLED_ShowChinese(3,1,23);
	OLED_ShowChinese(3,2,24);
	OLED_ShowChinese(3,3,25);
	OLED_ShowChinese(3,4,26);

	OLED_ShowChinese(4,1,52);
	OLED_ShowChinese(4,2,53);
	OLED_ShowChinese(4,3,54);
	OLED_ShowChinese(4,4,55);
}





//-------------------------------------------------------------------
extern char show_tmp;
extern char show_humi;
char line_one = 1;
char line_three = 1;

DHT11_Data_TypeDef DHT11_Data;

void line_one_show()
{
	
	if(mode == 2)
	{
		OLED_ShowNum(2,13,temperature_thresholed,2);	//设定当前设定的阈值1
		OLED_ShowChinese(2,8,73);
	}else if(mode == 1)
	{
		OLED_ShowNum(2,13,show_tmp,2);//只显示当前数据就好
		OLED_ShowChinese(2,8,73);
		
	}
}

extern char TimeCont;
extern char Key_State;
int line_two = 10;
extern int count;
void line_two_show()
{
	if(mode == 2)
	{
		OLED_ShowNum(3,13,humidity_threshold,2);	//设定当前设定的阈值1
		OLED_ShowChinese(3,8,74);
	}else if(mode == 1)
	{
		OLED_ShowNum(3, 13,show_humi,2);		//显示当前值
		OLED_ShowChinese(3,8,74);
	}
}

extern uint16_t smokeDensity;				//smokeDensity的值范围在100-4000，这里将它除以100再显示成百分比
extern char Warm_state;
extern char warm_lock;	
extern char	lock_bell;
void line_three_show()
{
	if(mode == 2)
	{
		OLED_ShowNum(4,13,gas_threshold,2);	//设定当前设定的阈值1
		OLED_ShowChinese(4,8,74);//显示当前阈值3
	}
	else if (mode == 1)
	{
//		OLED_ShowNum(4,13,lock_bell,2);
	
//	    OLED_ShowNum(3,11,warm_lock,2);
  		OLED_ShowNum(4,13,smokeDensity/41,2);//设定当前设定的阈值1     因为最大是4095，除以41最大可以用99%表示
			OLED_ShowChinese(4,8,74);//显示当前数值
	}
	
}


/**
  * @brief  OLED显示浮点数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字
  * @param  Length 要显示数字的长度，范围：1~10
  * @param  Flength 要显示的小数点后几位
  * @retval 无
  */

void OLED_ShowFNum(uint8_t Line, uint8_t Column, float Number, uint8_t Length,uint8_t Flength)
{
    uint8_t i;
    uint8_t flag = 1;
    float Number1;
    uint32_t Number2;
    if (Number >= 0)
    {

        Number1 = Number;
    }
    else
    {
        OLED_ShowChar(Line, Column, '-');
        Number1 = -Number;
    }
    //将浮点数转换成整数然后显示
    Number2 = (int)(Number1 * OLED_Pow(10,Flength));

    for (i = Length; i > 0; i--)                            
    {
        if(i == (Length - Flength))
        {
            OLED_ShowChar(Line,Column + i + flag,'.');
            flag = 0;
            OLED_ShowChar(Line, Column + i + flag, Number2 / OLED_Pow(10, Length - i) % 10 + '0');
        }
        else
        {
            OLED_ShowChar(Line, Column + i + flag, Number2 / OLED_Pow(10, Length - i) % 10 + '0');
        }
        
    }    
        
}


char show = 1;
char line_four = 1;



//-------------------------------------------------------------------

//--------------------------不同模式进入不同界面----------------------------------------------------

void Mode_Change()
{

	switch (mode)
	{
	case 1:
		OLED_SHOW1();
		break;
	case 2:
	OLED_SHOW_SET();	
		break;	
	}
														if(mode == 3)						//此处放了mode 和 selectline的转换!!!
														{
															mode = 1;
														}else if(mode == 0)
														{
															mode = 3;
														}


														if(select_line == 4)				//这里还要改！
														{
															select_line = 1;
														}else if(select_line == 0)
														{
															select_line = 3;
														}
														
														if(temperature_thresholed == 100)
														{
															temperature_thresholed = 0;
														}
														
														if(humidity_threshold == 100)
														{
															humidity_threshold = 0;
														}
														
														if(gas_threshold == 100)
														{
															gas_threshold = 0;
														}
}

//------- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- 



//void Device_Type_set()
//{
//	switch (Device_Type)
//	{
//	case 1:
//		Type_Show1();													//本来是Type_Show1();
//		break;
//	case 2:
//		OLED_SHOW1();
//		break;
//	}
//}
//------------------->的切换---------------------------------

void Select_change()
{
	switch (mode)
	{
	case 1:
		break;
	case 2:
	switch (select_line)
	{
	case 1:

		OLED_ShowChinese(2,6,28);
		break;
	case 2:

		OLED_ShowChinese(3,6,28);
		break;
	case 3:

		OLED_ShowChinese(4,6,28);
		break;	
	}
		break;
	}
}






