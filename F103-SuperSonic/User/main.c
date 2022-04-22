#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include "utils_tim_timer.h"
#include "bsp_SuperSonic.h"



int main(void){
	
	/* ��ʱ����ʼ�� */
	/* ����SysTick Ϊ10us�ж�һ�� */
	SysTick_Init();

	/* ���ڳ�ʼ�� */
	USART_Config();

	/* ��ʱ����ʼ�� */
	TIM_TIMER_Init();
	
	/* ���������ų�ʼ�� */
	HCSR04_GPIO_Init();
	
	float distanse = 0;
	
	
  while(1){		
		
		SysTick_Delay_Ms( 1000 );

		//printf("������1��%d\n",TIM_GetCounter(TIM_TIMER));
		
		Open_Tim_Timer();
		//HCSR04_GetLength();
		distanse = Get_Distance();
		
		Close_Tim_Timer();
    //distanse = Get_Distance();
		
		//printf("������2��%d\n",TIM_GetCounter(TIM_TIMER));
		
		unsigned int time = Get_Tim_Time();
		


		distanse = (float)time * 0.017;      // cm
		printf("����ʱ��: %d �ҵ�΢��\r\n\n", time);
		printf("����Ϊ : %.3f���ʱ��\r\n\n", distanse);
		
		//SysTick_Delay_Ms(1500);
  }

}

/*********************************************END OF FILE**********************/
