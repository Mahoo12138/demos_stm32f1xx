 
	
#include "stm32f10x.h"
#include "bsp_StepMotor.h"
#include "bsp_SysTick.h"

u8 phasecw[8] ={0x08,0x0c,0x04,0x06,0x02,0x03,0x01,0x09};// ��ʱ��
u8 phaseccw[8]={0x09,0x01,0x03,0x02,0x06,0x04,0x0c,0x08};// ˳ʱ��

void MotoRcw(void)  //��ת
{  
		int k=0;
    for(k=0;k<8;k++)
    {      
        Set_Step_Motor(phasecw[k]);
        SysTick_Delay_Ms(3);
    }
}

void MotoRccw(void)  //��ת
{  
    int k=0;
    for(k=0;k<8;k++)
    {      
        Set_Step_Motor(phaseccw[k]);
        SysTick_Delay_Ms(3);
    }
}

void MotorStop(void) //ֹͣ
{  
    GPIO_ResetBits(STEP_MOTOR_PIN_PORT,
	  STEP_MOTOR_PIN_A|STEP_MOTOR_PIN_B|STEP_MOTOR_PIN_C|STEP_MOTOR_PIN_D );
}

//���Ƶ����ת���Ƿ�תĳ���Ƕ�
//direction����1Ϊ��ת��0Ϊ��ת
//angle�Ƕȣ���Ϊ0-360����ʵ������
void Motor_Ctrl_Direction_Angle(int direction, int angle)
{
	u16 j;
	if(direction == 1)
	{
		for(j=0;j<64*angle/45;j++) 
		{
			MotoRccw();//��ת
		}
		 MotorStop();//ֹͣ
  }
	else
	{
		for(j=0;j<64*angle/45;j++) 
		{
			MotoRcw();//��ת
		}
		 MotorStop();//ֹͣ
	}
}
int main(void)
{	
	
	/* ��ʱ����ʼ�� */
	Step_Motor_GPIO_Init();
	
	/* ����SysTick Ϊ10us�ж�һ�� */
	SysTick_Init();
	
 
	while(1)
	{
			//Motor_Ctrl_Direction_Angle(1,360);//��ת180��
			//SysTick_Delay_Ms(5000);
			Motor_Ctrl_Direction_Angle(0,90);//��ת90��
	}

}

/*********************************************END OF FILE**********************/
