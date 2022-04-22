#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include "utils_tim_timer.h"
#include "bsp_SuperSonic.h"



int main(void){
	
	/* 定时器初始化 */
	/* 配置SysTick 为10us中断一次 */
	SysTick_Init();

	/* 串口初始化 */
	USART_Config();

	/* 计时器初始化 */
	TIM_TIMER_Init();
	
	/* 超声波引脚初始化 */
	HCSR04_GPIO_Init();
	
	float distanse = 0;
	
	
  while(1){		
		
		SysTick_Delay_Ms( 1000 );

		//printf("计数器1：%d\n",TIM_GetCounter(TIM_TIMER));
		
		Open_Tim_Timer();
		//HCSR04_GetLength();
		distanse = Get_Distance();
		
		Close_Tim_Timer();
    //distanse = Get_Distance();
		
		//printf("计数器2：%d\n",TIM_GetCounter(TIM_TIMER));
		
		unsigned int time = Get_Tim_Time();
		


		distanse = (float)time * 0.017;      // cm
		printf("发送时间: %d 我的微秒\r\n\n", time);
		printf("距离为 : %.3f你的时间\r\n\n", distanse);
		
		//SysTick_Delay_Ms(1500);
  }

}

/*********************************************END OF FILE**********************/
