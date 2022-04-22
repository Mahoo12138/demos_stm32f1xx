/**
  ************************************* Copyright ******************************
  *                 (C) Copyright 2020,Mahoo12138,China, DLU.
  *                            All Rights Reserved
  *
  *                     By   大连大学智能控制工作室
  *                     https://github.com/mahoo12138
  *
  * FileName   : bsp_SuperSonic.h
  * Version    : v1.0
  * Author     : Mahoo12138
  * Date       : 2020-10-01
  * Description:
  *
  ******************************************************************************
 */



#ifndef __BSP__SUPER_SONIC_H_
#define __BSP__SUPER_SONIC_H_

#define HCSR04_PORT              GPIOA
#define HCSR04_PORTC_CLK_FUN     RCC_APB2PeriphClockCmd
#define HCSR04_CLK               RCC_APB2Periph_GPIOA
#define HCSR04_TRIG              GPIO_Pin_4
#define HCSR04_ECHO              GPIO_Pin_5

void HCSR04_GPIO_Init(void);
void HCSR04_GetLength(void);


float Get_Distance(void);
#endif





