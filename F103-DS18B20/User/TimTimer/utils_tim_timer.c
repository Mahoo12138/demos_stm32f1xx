/**
  ************************************* Copyright ****************************** 
  *
  *                 (C) Copyright 2020,Mahoo12138,China, DLU.
  *                            All Rights Reserved
  *                              
  *                     By ������ѧ��Ϣ����ѧԺ���ܿ��ƹ�����
  *                     https://github.com/Mahoo12138
  *    
  * FileName   : utils_tim_timer.c   
  * Version    : v1.0		
  * Author     : Mahoo12138			
  * Date       : 2020-10-01         
  * Description: ��ʱ������Դ���룬����ͷ�ļ�ʹ�ã�ע����Ҫ���жϺ���д�뵽 stm32fxxx.it ��
  *              �������жϴ������ֵ timerCounts ����Ȼ����ͷ�ļ�	
  * 
  ******************************************************************************
 */

#include "utils_tim_timer.h"
#include "bsp_SysTick.h"
#include "stm32f10x.h"

/* ���� �жϴ��� ���ֵ*/
unsigned int timerCounts = 0;


/**
  * @Name       TIM_TIMER_Config
  * @brief      ��ʱ�� TIMx �ĳ�ʼ��
  * @param      None
  * @retval     None
  * @attention  
  * @author     Mahoo12138 
  * @Date       2020-09-30
  */
static void TIM_TIMER_Config(void){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    // ������ʱ��ʱ�ӣ����ڲ�ʱ�� CK_INT=72M
    TIM_TIMER_APBxClkFUN(TIM_TIMER_CLK, ENABLE);

    // �Զ���װ�ؼĴ����ܵ�ֵ(����ֵ), 50ms ����һ���ж�
    TIM_TimeBaseStructure.TIM_Period = 50000;

    // �ۼ� TIM_Period ��Ƶ�ʺ����һ�����»����ж�
    // ʱ��Ԥ��Ƶ��Ϊ 71����������������ʱ�� CK_CNT = CK_INT / (71+1)= 1M
    TIM_TimeBaseStructure.TIM_Prescaler= 71;

    // ���������ṹ�������������ʱ��û�У����ù�

    // ��ʼ����ʱ��
    TIM_TimeBaseInit(TIM_TIMER, &TIM_TimeBaseStructure);

    // ����������жϱ�־λ
    TIM_ClearFlag(TIM_TIMER, TIM_FLAG_Update);

    // �����������ж�
    TIM_ITConfig(TIM_TIMER,TIM_IT_Update,ENABLE);

    // ��ʱ�رռ�����
    TIM_Cmd(TIM_TIMER, DISABLE);
}  


/**
  * @Name       TIM_TIMER_NVIC_Config
  * @brief      ��ʱ�� TIMx ���ж����ȼ�����
  * @param      None
  * @retval     None
  * @attention  
  * @author     Mahoo12138 
  * @Date       2020-09-30
  */
 static void TIM_TIMER_NVIC_Config(void){
	 
    NVIC_InitTypeDef NVIC_InitStructure;
    // �����ж���Ϊ 0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	 
    // �����ж���ԴΪ������ʱ�� 
    NVIC_InitStructure.NVIC_IRQChannel = TIM_TIMER_IRQ ;
	 
    // ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    // ������ռ���ȼ�Ϊ 3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}



/**
  * @Name       Open_Tim_Timer
  * @brief      �򿪼�ʱ��
  * @param      None
  * @retval     None
  * @attention  
  * @author     Mahoo12138 
  * @Date       2020-10-01
  */
void Open_Tim_Timer(void){
	TIM_SetCounter(TIM_TIMER,0);//�������
	timerCounts = 0;
	TIM_Cmd(TIM_TIMER, ENABLE);  //ʹ��TIMx����
}


/**
  * @Name       Close_Tim_Timer
  * @brief      �رռ�ʱ��
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
  * @brief      ��ȡ��ʱ��ʱ��
  * @param      None
  * @retval     unsigned int time: ��ʱ��ʱ�䣬��λ us
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

/*******************     �жϷ�����    **********************************

	void TIM_TIMER_IRQHandler(){
		if(TIM_GetITStatus(TIM_TIMER, TIM_IT_Update) == SET){
					TIM_ClearITPendingBit(TIM_TIMER, TIM_IT_Update);
			
					timerCounts ++;
			}

	}

*/
 

