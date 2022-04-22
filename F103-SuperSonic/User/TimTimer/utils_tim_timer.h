/**
  ************************************* Copyright ******************************   
  *                 (C) Copyright 2020,Mahoo12138,China, DLU.
  *                            All Rights Reserved
  *                              
  *                     By   大连大学智能控制工作室
  *                     https://github.com/mahoo12138
  *      
  * FileName   : bsp_tim.h   
  * Version    : v1.0		
  * Author     : Mahoo12138			
  * Date       : 2020-09-30         
  * Description:    
  *
  ******************************************************************************
 */



 #ifndef __BSP_TIM_H_
#define __BSP_TIM_H_

#define		TIM_TIMER													TIM7												// 计时器功能使用的定时器
#define		TIM_TIMER_APBxClkFUN							RCC_APB1PeriphClockCmd			// 时钟开启函数
#define		TIM_TIMER_CLK								      RCC_APB1Periph_TIM7					// TIMx 时钟
#define 	TIM_TIMER_IRQ                     TIM7_IRQn                   // 中断源
#define 	TIM_TIMER_IRQHandler              TIM7_IRQHandler  						// 中断服务函数

void TIM_TIMER_Init(void);
void Close_Tim_Timer(void);
void Open_Tim_Timer(void);
unsigned int Get_Tim_Time(void);

#endif


 




 

