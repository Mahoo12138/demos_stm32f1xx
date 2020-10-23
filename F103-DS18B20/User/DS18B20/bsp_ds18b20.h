/**
  ************************************* Copyright ******************************
  *                 (C) Copyright 2020,Mahoo12138,China, DLU.
  *                            All Rights Reserved
  *
  *                     By   大连大学智能控制工作室
  *                     https://github.com/mahoo12138
  *
  * FileName   : bsp_ds18b20.h
  * Version    : v1.0
  * Author     : Mahoo12138
  * Date       : 2020-10-01
  * Description: DS18B20 驱动头文件
  *
  ******************************************************************************
 */



#ifndef __BSP_DS18B20_H_
#define __BSP_DS18B20_H_

#include "stm32f10x.h"

/************************** DS18B20 连接引脚定义********************************/
#define      DS18B20_DQ_SCK_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define      DS18B20_DQ_GPIO_CLK                       RCC_APB2Periph_GPIOA
#define      DS18B20_DQ_GPIO_PORT                      GPIOA
#define      DS18B20_DQ_GPIO_PIN                       GPIO_Pin_5

/************************** DS18B20 函数宏定义********************************/
#define      DS18B20_DQ_Low                            GPIO_ResetBits(DS18B20_DQ_GPIO_PORT, DS18B20_DQ_GPIO_PIN )
#define      DS18B20_DQ_High	                         GPIO_SetBits(DS18B20_DQ_GPIO_PORT, DS18B20_DQ_GPIO_PIN )
#define      DS18B20_DQ_Read()	                       GPIO_ReadInputDataBit(DS18B20_DQ_GPIO_PORT, DS18B20_DQ_GPIO_PIN )



/************************** DS18B20 函数声明 ********************************/
uint8_t		DS18B20_Init(void);
void			DS18B20_ReadId(uint8_t * ds18b20_id);
float     DS18B20_GetTemp_SkipRom(void);
float     DS18B20_GetTemp_MatchRom(uint8_t * ds18b20_id);

#endif







