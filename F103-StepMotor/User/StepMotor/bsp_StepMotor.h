#ifndef  _STEP_MOTOR_H_
#define  _STEP_MOTOR_H_

#include "stm32f10x.h"

// 步进电机驱动端口
#define            STEP_MOTOR_GPIO_CLK            RCC_APB2Periph_GPIOA
#define            STEP_MOTOR_PIN_PORT            GPIOA
#define            STEP_MOTOR_PIN_A               GPIO_Pin_4
#define            STEP_MOTOR_PIN_B               GPIO_Pin_5
#define            STEP_MOTOR_PIN_C               GPIO_Pin_6
#define            STEP_MOTOR_PIN_D               GPIO_Pin_7

void Step_Motor_GPIO_Init(void);
void Set_Step_Motor(unsigned char InputData);

#endif

