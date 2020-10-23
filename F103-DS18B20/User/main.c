#include "stm32f10x.h"
#include "bsp_pwm.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include "bsp_ds18b20.h"


int main(void){
	
	uint8_t uc, ucDs18b20Id[8];
	
	/* ��ʱ����ʼ�� */
	/* ����SysTick Ϊ10us�ж�һ�� */
	SysTick_Init();
		
	/* ���ڳ�ʼ�� */
	USART_Config();
	
	while( DS18B20_Init()){
		printf("\r\n No Ds18b20 exit \r\n");
	}
	printf("\r\n Ds18b20 exit \r\n");
	
	DS18B20_ReadId(ucDs18b20Id);           // ��ȡ DS18B20 �����к�
	
	printf("\r\n DS18B20�����к��ǣ� 0x");
	for (uc = 0; uc < 8; uc++){             // ��ӡ DS18B20 �����к�
		printf ("%x", ucDs18b20Id[uc]);
	}
	
	printf ( "\r\n 1�¶ȣ� %.1f\r\n", DS18B20_GetTemp_MatchRom(ucDs18b20Id));		// ��ӡͨ�� DS18B20 ���кŻ�ȡ���¶�ֵ

  while(1){
		
		printf ( "\r\n 2�¶ȣ� %.1f\r\n", DS18B20_GetTemp_SkipRom() );		// ��ӡͨ�� DS18B20 ���кŻ�ȡ���¶�ֵ
		
		SysTick_Delay_Ms(1000);
			
  }

}





/*********************************************END OF FILE**********************/
