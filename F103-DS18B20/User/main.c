#include "stm32f10x.h"
#include "bsp_pwm.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include "bsp_ds18b20.h"


int main(void){
	
	uint8_t uc, ucDs18b20Id[8];
	
	/* 定时器初始化 */
	/* 配置SysTick 为10us中断一次 */
	SysTick_Init();
		
	/* 串口初始化 */
	USART_Config();
	
	while( DS18B20_Init()){
		printf("\r\n No Ds18b20 exit \r\n");
	}
	printf("\r\n Ds18b20 exit \r\n");
	
	DS18B20_ReadId(ucDs18b20Id);           // 读取 DS18B20 的序列号
	
	printf("\r\n DS18B20的序列号是： 0x");
	for (uc = 0; uc < 8; uc++){             // 打印 DS18B20 的序列号
		printf ("%x", ucDs18b20Id[uc]);
	}
	
	printf ( "\r\n 1温度： %.1f\r\n", DS18B20_GetTemp_MatchRom(ucDs18b20Id));		// 打印通过 DS18B20 序列号获取的温度值

  while(1){
		
		printf ( "\r\n 2温度： %.1f\r\n", DS18B20_GetTemp_SkipRom() );		// 打印通过 DS18B20 序列号获取的温度值
		
		SysTick_Delay_Ms(1000);
			
  }

}





/*********************************************END OF FILE**********************/
