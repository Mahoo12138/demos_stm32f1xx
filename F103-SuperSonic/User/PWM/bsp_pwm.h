#ifndef  _BSP_PWM_H_
#define  _BSP_PWM_H_

#include "stm32f10x.h"
// 这里使用 TIM3
#define            GENERAL_TIM                   TIM3
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM3
#define            GENERAL_TIM_Period            199
#define            GENERAL_TIM_Prescaler         7199
// TIM3 输出比较通道
#define            GENERAL_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH1_PORT          GPIOA
#define            GENERAL_TIM_CH1_PIN           GPIO_Pin_6

void GENERAL_TIM_Init(void);

#endif

