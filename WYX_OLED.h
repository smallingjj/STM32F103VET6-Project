#ifndef __WYX_OLED_H
#define __WYX_OLED_H
#include<stdint.h>
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowChinese(uint8_t Line,uint8_t Column,uint8_t num);
void OLED_BMP(int i);
void OLED_SHOW(void);
void OLED_SHOW1(void);
void Mode_Change(void);
void Device_Type_set(void);
void Type_Show1(void);
void Select_change(void);
void line_one_show(void);
void line_two_show(void);
void line_three_show(void);
void lone_change(void);
void OLED_ShowChinese14(uint8_t Line,uint8_t Column,uint8_t num);
#endif


