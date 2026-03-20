#ifndef __BELL_H
#define __BELL_H

#define BELL_GPIO_PIN                GPIO_Pin_0
#define BELL_Green_GPIO_PORT         GPIOC
#define BELL_GPIO_CLK							   RCC_APB2Periph_GPIOC

#define BELL_ON      1
#define BELL_OFF     0

void bell(void);
void bell_on(void);
void bell_off(void);
void Warming_Scan(void);
void Warm_unlock(void);
void sentmessage(void);
void Motor_Init(void);
#define BELL_G_TOGGLE       {BELL_Green_GPIO_PORT->ODR ^= BELL_GPIO_PIN  ;}

#endif
