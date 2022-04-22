 
	
#include "stm32f10x.h"
#include "bsp_StepMotor.h"
#include "bsp_SysTick.h"

u8 phasecw[8] ={0x08,0x0c,0x04,0x06,0x02,0x03,0x01,0x09};// 逆时针
u8 phaseccw[8]={0x09,0x01,0x03,0x02,0x06,0x04,0x0c,0x08};// 顺时针

void MotoRcw(void)  //反转
{  
		int k=0;
    for(k=0;k<8;k++)
    {      
        Set_Step_Motor(phasecw[k]);
        SysTick_Delay_Ms(3);
    }
}

void MotoRccw(void)  //正转
{  
    int k=0;
    for(k=0;k<8;k++)
    {      
        Set_Step_Motor(phaseccw[k]);
        SysTick_Delay_Ms(3);
    }
}

void MotorStop(void) //停止
{  
    GPIO_ResetBits(STEP_MOTOR_PIN_PORT,
	  STEP_MOTOR_PIN_A|STEP_MOTOR_PIN_B|STEP_MOTOR_PIN_C|STEP_MOTOR_PIN_D );
}

//控制电机正转还是反转某个角度
//direction方向，1为正转，0为反转
//angle角度，可为0-360具有实际意义
void Motor_Ctrl_Direction_Angle(int direction, int angle)
{
	u16 j;
	if(direction == 1)
	{
		for(j=0;j<64*angle/45;j++) 
		{
			MotoRccw();//正转
		}
		 MotorStop();//停止
  }
	else
	{
		for(j=0;j<64*angle/45;j++) 
		{
			MotoRcw();//反转
		}
		 MotorStop();//停止
	}
}
int main(void)
{	
	
	/* 定时器初始化 */
	Step_Motor_GPIO_Init();
	
	/* 配置SysTick 为10us中断一次 */
	SysTick_Init();
	
 
	while(1)
	{
			//Motor_Ctrl_Direction_Angle(1,360);//正转180度
			//SysTick_Delay_Ms(5000);
			Motor_Ctrl_Direction_Angle(0,90);//反转90度
	}

}

/*********************************************END OF FILE**********************/
