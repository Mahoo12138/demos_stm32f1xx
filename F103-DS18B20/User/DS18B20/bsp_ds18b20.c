#include "bsp_ds18b20.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"


/**
  * @Name       DS18B20_GPIO_Config
  * @brief      �������߶˿�Ϊͨ���������
  * @param      None
  * @retval     None
  * @attention  None
  * @author     Mahoo12138
  * @Date       2020-10-01
  */
static void DS18B20_GPIO_Config(void) {
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;


    /*����DS18B20_DQ_GPIO_PORT������ʱ��*/
    DS18B20_DQ_SCK_APBxClock_FUN (DS18B20_DQ_GPIO_CLK, ENABLE);

    /*ѡ��Ҫ���Ƶ�DS18B20_DQ_GPIO_PORT����*/
    GPIO_InitStructure.GPIO_Pin = DS18B20_DQ_GPIO_PIN;

    /*��������ģʽΪ     ͨ���������   */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    /*������������Ϊ50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*���ÿ⺯������ʼ��DS18B20_DQ_GPIO_PORT*/
    GPIO_Init (DS18B20_DQ_GPIO_PORT, &GPIO_InitStructure);

}


/**
  * @Name       DS18B20_Mode_IPU
  * @brief      ʹDS18B20-DS���ű�Ϊ����ģʽ
  * @param      None
  * @retval     None
  * @attention
  * @author     Mahoo12138
  * @Date       2020-10-04
  */
static void DS18B20_Mode_IPU(void) {
	
    GPIO_InitTypeDef GPIO_InitStructure;

    /*ѡ��Ҫ���Ƶ�DS18B20_DQ_GPIO_PORT����*/
    GPIO_InitStructure.GPIO_Pin = DS18B20_DQ_GPIO_PIN;

    /*��������ģʽΪ��������ģʽ*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;

    /*���ÿ⺯������ʼ��DS18B20_DQ_GPIO_PORT*/
    GPIO_Init(DS18B20_DQ_GPIO_PORT, &GPIO_InitStructure);
}


/**
  * @Name       DS18B20_Mode_Out_PP
  * @brief      ʹDS18B20-DS���ű�Ϊ���ģʽ
  * @param      None
  * @retval     None
  * @attention  None
  * @author     Mahoo12138
  * @Date       2020-10-04
  */
static void DS18B20_Mode_Out_PP(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    /*ѡ��Ҫ���Ƶ�DS18B20_DQ_GPIO_PORT����*/
    GPIO_InitStructure.GPIO_Pin = DS18B20_DQ_GPIO_PIN;

    /*��������ģʽΪͨ���������*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    /*������������Ϊ50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*���ÿ⺯������ʼ��DS18B20_DQ_GPIO_PORT*/
    GPIO_Init(DS18B20_DQ_GPIO_PORT, &GPIO_InitStructure);
}


/**
  * @Name       DS18B20_Rst
  * @brief      ���͸�λ����
  * @param      None
  * @retval     None
  * @attention
  * @author     Mahoo12138
  * @Date       2020-10-05
  */
static void DS18B20_Rst(void) {

    /* ��������Ϊ������� */
    DS18B20_Mode_Out_PP();

    DS18B20_DQ_Low;
    /* �������ٲ���480us�ĵ͵�ƽ��λ�ź� */
    SysTick_Delay_Us(720);

    /* �����ڲ�����λ�źź��轫�������� */
    DS18B20_DQ_High;

    /*�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������*/
    SysTick_Delay_Us(15);
}


/**
  * @Name       DS18B20_Check
  * @brief      ��ⷵ�صĴ�������
  * @param      None
  * @retval     1/0
  * @attention
  * @author     Mahoo12138
  * @Date       2020-10-05
  */
static uint8_t DS18B20_Check(void) {

    uint8_t pulse_time = 0;

    /* ��������Ϊ�������� */
    DS18B20_Mode_IPU();

    /* �ȴ���������ĵ�������������Ϊһ��60~240us�ĵ͵�ƽ�ź�
     * �����������û����������ʱ�����ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
     */
    while(DS18B20_DQ_Read() && pulse_time < 100)
    {
        pulse_time++;
        SysTick_Delay_Us(1);
    }
    /* ����150us�󣬴������嶼��û�е���*/
    if(pulse_time >=150)
        return 1;
    else
        pulse_time = 0;

    /* �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us */
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
  * @brief      �� DS18B20 ��ȡһλ
  * @param      None
  * @retval     None
  * @attention  
  * @author     Mahoo12138 
  * @Date       2020-10-06
  */
static uint8_t DS18B20_ReadBit(void){
	uint8_t data;
	
	/* ��0�Ͷ�1��ʱ������Ҫ����60us */	
	DS18B20_Mode_Out_PP();
	/* ��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� */
	DS18B20_DQ_Low;
	
	SysTick_Delay_Us(10);
	
	/* ���ó����룬�ͷ����ߣ����ⲿ�������轫�������� */
	DS18B20_Mode_IPU();
	
	if( DS18B20_DQ_Read() == SET)
		data = 1;
	else
		data = 0;
	
	/* �����ʱ������ο�ʱ��ͼ */
	SysTick_Delay_Us(45);
	
	return data;
}


/**
  * @Name       DS18B20_ReadByte
  * @brief      �� DS18B20 ��ȡһ���ֽ�
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
  * @param      data: [д�� DS18B20 ���ֽ�] 
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
		/* д0��д1��ʱ������Ҫ����60us */
		if (testb)
		{			
			DS18B20_DQ_Low;
			/* 1us < �����ʱ < 15us */
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
			/* 1us < Trec(�ָ�ʱ��) < �����*/
			SysTick_Delay_Us(2);
		}
	}
}
/**
  * @Name       DS18B20_SkipRom
  * @brief      ��ʼ�� DS18B20 ������ Rom ��ѯ
  * @param      None
  * @retval     None
  * @attention  
  * @author     Mahoo12138 
  * @Date       2020-10-06
  */
static void DS18B20_SkipRom (void){
	DS18B20_Rst();	   
	
	DS18B20_Check();	 
	
	DS18B20_WriteByte(0XCC);		/* ���� ROM */
	
}



/**
  * @Name       DS18B20_SkipRom
  * @brief      ִ��ƥ�� DS18B20 ROM ָ��
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
	
	DS18B20_WriteByte(0X55);		/* ƥ�� ROM */
	
}


/**
  * @Name       DS18B20_GetTemp_SkipRom
  * @brief      ����ƥ�� ROM ��ȡ�¶�ֵ
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
	DS18B20_WriteByte(0X44);				/* ��ʼת�� */
	
	
	DS18B20_SkipRom();
  DS18B20_WriteByte(0XBE);				/* ���¶�ֵ */
	
	tplsb = DS18B20_ReadByte();		 
	tpmsb = DS18B20_ReadByte(); 
	
	s_tem = tpmsb << 8;

	s_tem = s_tem | tplsb;
	
	if(s_tem < 0)		/* ���¶� */
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = s_tem * 0.0625;
	
	return f_tem; 	
}


/**
  * @Name       DS18B20_ReadId
  * @brief      ��ȡ������
  * @param      ds18b20_id: [uint8_t *] 
  * @retval     None
  * @attention  None
  * @author     Mahoo12138
  * @Date       2020-10-23
  */
void DS18B20_ReadId (uint8_t * ds18b20_id){
	
	uint8_t uc;
	
	DS18B20_WriteByte(0x33);       //��ȡ���к�
	
	for (uc = 0; uc < 8; uc ++)
	  ds18b20_id[uc] = DS18B20_ReadByte();
	
}

/**
  * @Name       DS18B20_GetTemp_MatchRom
  * @brief      ƥ�� ROM ��ȡ�¶�ֵ
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
	
	
	DS18B20_MatchRom();            //ƥ��ROM
	
  for(i=0;i<8;i++)
		DS18B20_WriteByte ( ds18b20_id [ i ] );	
	
	DS18B20_WriteByte(0X44);				/* ��ʼת�� */

	
	DS18B20_MatchRom();            //ƥ��ROM
	
	for(i=0;i<8;i++)
		DS18B20_WriteByte (ds18b20_id [i]);	
	
	DS18B20_WriteByte(0XBE);				/* ���¶�ֵ */
	
	tplsb = DS18B20_ReadByte();		 
	tpmsb = DS18B20_ReadByte(); 
	
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )		/* ���¶� */
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = s_tem * 0.0625;
	
	return f_tem; 	
	
}

/**
  * @Name       DS18B20_Init
  * @brief      DS18B20 ��ʼ��
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
