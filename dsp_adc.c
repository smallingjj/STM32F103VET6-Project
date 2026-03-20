#include "stm32f10x.h"
#include "dsp_adc.h"
#include "bell.h"
#include "./uart/bsp_uart.h"
#include "stm32f10x_adc.h"



void Delay1(uint32_t count)
{
	for(;count!=0;count--);
}

void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; // MQ-2传感器数据引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
 
void ADC_Configuration(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;			//不外部gpio触发？
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;					//向右对其
    ADC_InitStructure.ADC_NbrOfChannel = 1;											//要转换通道数目
    ADC_Init(ADC1, &ADC_InitStructure);//将成员全部写到寄存器，&取地址
    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));
}


uint16_t GetSmokeDensity(void)
{
    ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_55Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    return ADC_GetConversionValue(ADC1);
}

uint16_t smokeDensity;



//%d表示将一个数据整形输出！！！！一个%d下面对应一个数据！！

 
void Alarm(void)
{
    bell_on(); // 发出报警声音和亮起LED指示灯
}


void Init_Smoke(void)//初始化用的
{
	GPIO_Configuration();
  ADC_Configuration();
}


int smoke_count = 0;

void super_smoke(void)
{	
	smoke_count++; 
	if(smoke_count == 100)
	{
		smokeDensity = GetSmokeDensity();
		smoke_count = 0;
	}
   
}

 





