#include "bsp_SuperSonic.h"
#include "utils_tim_timer.h"
#include "bsp_SysTick.h"
#include "stm32f10x.h"

u32 time;
float distance;

void HCSR04_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin =HCSR04_TRIG;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//普通推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);

    GPIO_InitStructure.GPIO_Pin =   HCSR04_ECHO;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(HCSR04_PORT,HCSR04_ECHO);

}


void HCSR04_GetLength(void){
	// unsigned char i = 0;

	GPIO_SetBits(HCSR04_PORT,HCSR04_TRIG);

	//SysTick_Delay_Us(20);
	Delay_us(20);
	// 输出超过 10us 的高电平
	GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);

	while(GPIO_ReadInputDataBit(HCSR04_PORT,HCSR04_ECHO)==RESET);

	TIM_Cmd(TIM_TIMER, ENABLE);

	// i++;

	while(GPIO_ReadInputDataBit(HCSR04_PORT,HCSR04_ECHO)==SET);
	TIM_Cmd(TIM_TIMER, DISABLE);


}

float Get_Distance(void)
{
	time=0;
    GPIO_SetBits(HCSR04_PORT,GPIO_Pin_4);
    Delay_us(20);
    GPIO_ResetBits(HCSR04_PORT,GPIO_Pin_4);
    while(GPIO_ReadInputDataBit(HCSR04_PORT,HCSR04_ECHO)!=1)
    {};
		
    while(GPIO_ReadInputDataBit(HCSR04_PORT,HCSR04_ECHO)==1)
    {
        Delay_us(1);
        time++;

    }
    distance=(float)time * 0.017;
		//time=0;
		return time;

}

