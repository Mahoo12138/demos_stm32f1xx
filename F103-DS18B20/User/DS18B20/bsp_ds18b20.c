#include "bsp_ds18b20.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"


/**
  * @Name       DS18B20_GPIO_Config
  * @brief      设置总线端口为通用推挽输出
  * @param      None
  * @retval     None
  * @attention  None
  * @author     Mahoo12138
  * @Date       2020-10-01
  */
static void DS18B20_GPIO_Config(void) {
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;


    /*开启DS18B20_DQ_GPIO_PORT的外设时钟*/
    DS18B20_DQ_SCK_APBxClock_FUN (DS18B20_DQ_GPIO_CLK, ENABLE);

    /*选择要控制的DS18B20_DQ_GPIO_PORT引脚*/
    GPIO_InitStructure.GPIO_Pin = DS18B20_DQ_GPIO_PIN;

    /*设置引脚模式为     通用推挽输出   */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    /*设置引脚速率为50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*调用库函数，初始化DS18B20_DQ_GPIO_PORT*/
    GPIO_Init (DS18B20_DQ_GPIO_PORT, &GPIO_InitStructure);

}


/**
  * @Name       DS18B20_Mode_IPU
  * @brief      使DS18B20-DS引脚变为输入模式
  * @param      None
  * @retval     None
  * @attention
  * @author     Mahoo12138
  * @Date       2020-10-04
  */
static void DS18B20_Mode_IPU(void) {
	
    GPIO_InitTypeDef GPIO_InitStructure;

    /*选择要控制的DS18B20_DQ_GPIO_PORT引脚*/
    GPIO_InitStructure.GPIO_Pin = DS18B20_DQ_GPIO_PIN;

    /*设置引脚模式为上拉输入模式*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;

    /*调用库函数，初始化DS18B20_DQ_GPIO_PORT*/
    GPIO_Init(DS18B20_DQ_GPIO_PORT, &GPIO_InitStructure);
}


/**
  * @Name       DS18B20_Mode_Out_PP
  * @brief      使DS18B20-DS引脚变为输出模式
  * @param      None
  * @retval     None
  * @attention  None
  * @author     Mahoo12138
  * @Date       2020-10-04
  */
static void DS18B20_Mode_Out_PP(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    /*选择要控制的DS18B20_DQ_GPIO_PORT引脚*/
    GPIO_InitStructure.GPIO_Pin = DS18B20_DQ_GPIO_PIN;

    /*设置引脚模式为通用推挽输出*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    /*设置引脚速率为50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*调用库函数，初始化DS18B20_DQ_GPIO_PORT*/
    GPIO_Init(DS18B20_DQ_GPIO_PORT, &GPIO_InitStructure);
}


/**
  * @Name       DS18B20_Rst
  * @brief      发送复位脉冲
  * @param      None
  * @retval     None
  * @attention
  * @author     Mahoo12138
  * @Date       2020-10-05
  */
static void DS18B20_Rst(void) {

    /* 主机设置为推挽输出 */
    DS18B20_Mode_Out_PP();

    DS18B20_DQ_Low;
    /* 主机至少产生480us的低电平复位信号 */
    SysTick_Delay_Us(720);

    /* 主机在产生复位信号后，需将总线拉高 */
    DS18B20_DQ_High;

    /*从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲*/
    SysTick_Delay_Us(15);
}


/**
  * @Name       DS18B20_Check
  * @brief      检测返回的存在脉冲
  * @param      None
  * @retval     1/0
  * @attention
  * @author     Mahoo12138
  * @Date       2020-10-05
  */
static uint8_t DS18B20_Check(void) {

    uint8_t pulse_time = 0;

    /* 主机设置为上拉输入 */
    DS18B20_Mode_IPU();

    /* 等待存在脉冲的到来，存在脉冲为一个60~240us的低电平信号
     * 如果存在脉冲没有来则做超时处理，从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲
     */
    while(DS18B20_DQ_Read() && pulse_time < 100)
    {
        pulse_time++;
        SysTick_Delay_Us(1);
    }
    /* 经过150us后，存在脉冲都还没有到来*/
    if(pulse_time >=150)
        return 1;
    else
        pulse_time = 0;

    /* 存在脉冲到来，且存在的时间不能超过240us */
    while(!DS18B20_DQ_Read() && pulse_time<240)
    {
        pulse_time++;
        SysTick_Delay_Us(1);
    }
    if(pulse_time >=240)
        return 1;
    else
        return 0;
}


/**
  * @Name       DS18B20_ReadBit
  * @brief      从 DS18B20 读取一位
  * @param      None
  * @retval     None
  * @attention  
  * @author     Mahoo12138 
  * @Date       2020-10-06
  */
static uint8_t DS18B20_ReadBit(void){
	uint8_t data;
	
	/* 读0和读1的时间至少要大于60us */	
	DS18B20_Mode_Out_PP();
	/* 读时间的起始：必须由主机产生 >1us <15us 的低电平信号 */
	DS18B20_DQ_Low;
	
	SysTick_Delay_Us(10);
	
	/* 设置成输入，释放总线，由外部上拉电阻将总线拉高 */
	DS18B20_Mode_IPU();
	
	if( DS18B20_DQ_Read() == SET)
		data = 1;
	else
		data = 0;
	
	/* 这个延时参数请参考时序图 */
	SysTick_Delay_Us(45);
	
	return data;
}


/**
  * @Name       DS18B20_ReadByte
  * @brief      从 DS18B20 读取一个字节
  * @param      None
  * @retval     None
  * @attention  
  * @author     Mahoo12138 
  * @Date       2020-10-06
  */
static uint8_t DS18B20_ReadByte(void){
	uint8_t i, j, data = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_ReadBit();		
		data = (data) | (j<<i);
	}
	
	return data;
}


/**
  * @Name       DS18B20_WriteByte
  * @brief      
  * @param      data: [写入 DS18B20 的字节] 
  * @retval     None
  * @attention  
  * @author     Mahoo12138 
  * @Date       2020-10-06
  */
static void DS18B20_WriteByte(uint8_t data){
	uint8_t i, testb;
	DS18B20_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = data&0x01;
		data = data>>1;		
		/* 写0和写1的时间至少要大于60us */
		if (testb)
		{			
			DS18B20_DQ_Low;
			/* 1us < 这个延时 < 15us */
			SysTick_Delay_Us(8);
			
			DS18B20_DQ_High;
			SysTick_Delay_Us(58);
		}		
		else
		{			
			DS18B20_DQ_Low;
			
			/* 60us < Tx 0 < 120us */
			SysTick_Delay_Us(70);
			
			DS18B20_DQ_High;			
			/* 1us < Trec(恢复时间) < 无穷大*/
			SysTick_Delay_Us(2);
		}
	}
}
/**
  * @Name       DS18B20_SkipRom
  * @brief      初始化 DS18B20 并跳过 Rom 查询
  * @param      None
  * @retval     None
  * @attention  
  * @author     Mahoo12138 
  * @Date       2020-10-06
  */
static void DS18B20_SkipRom (void){
	DS18B20_Rst();	   
	
	DS18B20_Check();	 
	
	DS18B20_WriteByte(0XCC);		/* 跳过 ROM */
	
}



/**
  * @Name       DS18B20_SkipRom
  * @brief      执行匹配 DS18B20 ROM 指令
  * @param      None
  * @retval     None
  * @attention  
  * @author     Mahoo12138 
  * @Date       2020-10-23
  */
static void DS18B20_MatchRom ( void )
{
	DS18B20_Rst();	   
	
	DS18B20_Check();	 
	
	DS18B20_WriteByte(0X55);		/* 匹配 ROM */
	
}


/**
  * @Name       DS18B20_GetTemp_SkipRom
  * @brief      跳过匹配 ROM 读取温度值
  * @param      None
  * @retval     f_tem: [float]
  * @attention  None
  * @author     Mahoo12138
  * @Date       2020-10-23
  */
float DS18B20_GetTemp_SkipRom (void){
	
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
	DS18B20_SkipRom();
	DS18B20_WriteByte(0X44);				/* 开始转换 */
	
	
	DS18B20_SkipRom();
  DS18B20_WriteByte(0XBE);				/* 读温度值 */
	
	tplsb = DS18B20_ReadByte();		 
	tpmsb = DS18B20_ReadByte(); 
	
	s_tem = tpmsb << 8;

	s_tem = s_tem | tplsb;
	
	if(s_tem < 0)		/* 负温度 */
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = s_tem * 0.0625;
	
	return f_tem; 	
}


/**
  * @Name       DS18B20_ReadId
  * @brief      读取序列码
  * @param      ds18b20_id: [uint8_t *] 
  * @retval     None
  * @attention  None
  * @author     Mahoo12138
  * @Date       2020-10-23
  */
void DS18B20_ReadId (uint8_t * ds18b20_id){
	
	uint8_t uc;
	
	DS18B20_WriteByte(0x33);       //读取序列号
	
	for (uc = 0; uc < 8; uc ++)
	  ds18b20_id[uc] = DS18B20_ReadByte();
	
}

/**
  * @Name       DS18B20_GetTemp_MatchRom
  * @brief      匹配 ROM 读取温度值
  * @param      ds18b20_id: [uint8_t *] 
  * @retval     None
  * @attention  None
  * @author     Mahoo12138
  * @Date       2020-10-23
  */
float DS18B20_GetTemp_MatchRom ( uint8_t * ds18b20_id )
{
	uint8_t tpmsb, tplsb, i;
	short s_tem;
	float f_tem;
	
	
	DS18B20_MatchRom();            //匹配ROM
	
  for(i=0;i<8;i++)
		DS18B20_WriteByte ( ds18b20_id [ i ] );	
	
	DS18B20_WriteByte(0X44);				/* 开始转换 */

	
	DS18B20_MatchRom();            //匹配ROM
	
	for(i=0;i<8;i++)
		DS18B20_WriteByte (ds18b20_id [i]);	
	
	DS18B20_WriteByte(0XBE);				/* 读温度值 */
	
	tplsb = DS18B20_ReadByte();		 
	tpmsb = DS18B20_ReadByte(); 
	
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )		/* 负温度 */
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = s_tem * 0.0625;
	
	return f_tem; 	
	
}

/**
  * @Name       DS18B20_Init
  * @brief      DS18B20 初始化
  * @param      None
  * @retval     1/0
  * @attention  None
  * @author     Mahoo12138
  * @Date       2020-10-23
  */
uint8_t DS18B20_Init(void)
{
	DS18B20_GPIO_Config();
	
	DS18B20_DQ_High;
	
	DS18B20_Rst();
	
	return DS18B20_Check();
	
}
