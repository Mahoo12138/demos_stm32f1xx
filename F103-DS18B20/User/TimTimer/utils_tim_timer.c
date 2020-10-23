/**
  ************************************* Copyright ****************************** 
  *
  *                 (C) Copyright 2020,Mahoo12138,China, DLU.
  *                            All Rights Reserved
  *                              
  *                     By 大连大学信息工程学院智能控制工作室
  *                     https://github.com/Mahoo12138
  *    
  * FileName   : utils_tim_timer.c   
  * Version    : v1.0		
  * Author     : Mahoo12138			
  * Date       : 2020-10-01         
  * Description: 计时器工具源代码，配置头文件使用，注意需要将中断函数写入到 stm32fxxx.it 内
  *              并引入中断次数标记值 timerCounts ，当然还有头文件	
  * 
  ******************************************************************************
 */

#include "utils_tim_timer.h"
#include "bsp_SysTick.h"
#include "stm32f10x.h"

/* 进入 中断次数 标记值*/
unsigned int timerCounts = 0;


/**
  * @Name       TIM_TIMER_Config
  * @brief      计时器 TIMx 的初始化
  * @param      None
  * @retval     None
  * @attention  
  * @author     Mahoo12138 
  * @Date       2020-09-30
  */
static void TIM_TIMER_Config(void){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    // 开启定时器时钟，即内部时钟 CK_INT=72M
    TIM_TIMER_APBxClkFUN(TIM_TIMER_CLK, ENABLE);

    // 自动重装载寄存器周的值(计数值), 50ms 进入一次中断
    TIM_TimeBaseStructure.TIM_Period = 50000;

    // 累计 TIM_Period 个频率后产生一个更新或者中断
    // 时钟预分频数为 71，则驱动计数器的时钟 CK_CNT = CK_INT / (71+1)= 1M
    TIM_TimeBaseStructure.TIM_Prescaler= 71;

    // 其余三个结构体变量，基本定时器没有，不用管

    // 初始化定时器
    TIM_TimeBaseInit(TIM_TIMER, &TIM_TimeBaseStructure);

    // 清除计数器中断标志位
    TIM_ClearFlag(TIM_TIMER, TIM_FLAG_Update);

    // 开启计数器中断
    TIM_ITConfig(TIM_TIMER,TIM_IT_Update,ENABLE);

    // 暂时关闭计数器
    TIM_Cmd(TIM_TIMER, DISABLE);
}  


/**
  * @Name       TIM_TIMER_NVIC_Config
  * @brief      计时器 TIMx 的中断优先级配置
  * @param      None
  * @retval     None
  * @attention  
  * @author     Mahoo12138 
  * @Date       2020-09-30
  */
 static void TIM_TIMER_NVIC_Config(void){
	 
    NVIC_InitTypeDef NVIC_InitStructure;
    // 设置中断组为 0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	 
    // 设置中断来源为基本定时器 
    NVIC_InitStructure.NVIC_IRQChannel = TIM_TIMER_IRQ ;
	 
    // 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    // 设置抢占优先级为 3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}



/**
  * @Name       Open_Tim_Timer
  * @brief      打开计时器
  * @param      None
  * @retval     None
  * @attention  
  * @author     Mahoo12138 
  * @Date       2020-10-01
  */
void Open_Tim_Timer(void){
	TIM_SetCounter(TIM_TIMER,0);//清除计数
	timerCounts = 0;
	TIM_Cmd(TIM_TIMER, ENABLE);  //使能TIMx外设
}


/**
  * @Name       Close_Tim_Timer
  * @brief      关闭计时器
  * @param      None
  * @retval     None
  * @attention  
  * @author     Mahoo12138 
  * @Date       2020-10-01
  */
void Close_Tim_Timer(void){
	TIM_Cmd(TIM_TIMER, DISABLE); 
}




/**
  * @Name       Get_Tim_Time
  * @brief      获取计时器时间
  * @param      None
  * @retval     unsigned int time: 计时器时间，单位 us
  * @attention  
  * @author     Mahoo12138 
  * @Date       2020-10-01
  */
unsigned int Get_Tim_Time(void){
	unsigned int time = 0;
	
	time = timerCounts * 50000;
	
  time += TIM_GetCounter(TIM_TIMER);
	
	TIM_SetCounter(TIM_TIMER,0);
	
	SysTick_Delay_Ms(2);
	
	return time;
}

void TIM_TIMER_Init(void){
	
	TIM_TIMER_Config();
	
	TIM_TIMER_NVIC_Config();
}

/*******************     中断服务函数    **********************************

	void TIM_TIMER_IRQHandler(){
		if(TIM_GetITStatus(TIM_TIMER, TIM_IT_Update) == SET){
					TIM_ClearITPendingBit(TIM_TIMER, TIM_IT_Update);
			
					timerCounts ++;
			}

	}

*/
 

