/***************************************************************************************/
//原液剩余监测驱动程序
//型号：
/***************************************************************************************/
#include "stoste.h"

uint8_t stoste1_state = 1;  //初始为有原液
uint8_t stoste2_state = 1;

void Stoste_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

 /*--------------------------stoste1---------------------*/
	//GPIO初始化
	RCC_APB2PeriphClockCmd(STOSTE_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = STOSTE1_PIN|STOSTE2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(STOSTE_PORT,&GPIO_InitStructure);
	
	//EXTI配置
	GPIO_EXTILineConfig(STOSTE1_PORT_SOURCE,STOSTE1_PIN_SOURCE);
	EXTI_InitStructure.EXTI_Line = STOSTE1_EXTI_LINE;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //下降沿触发
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(STOSTE2_PORT_SOURCE,STOSTE2_PIN_SOURCE);
	EXTI_InitStructure.EXTI_Line = STOSTE2_EXTI_LINE;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;   //下降沿触发
	EXTI_Init(&EXTI_InitStructure);
	
	//NVIC初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = STOSTE1_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = STOSTE2_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

//获取原液监测器数据
stoste_state_t Stoste_GetStatus(uint8_t id)
{
	if(id ==0)
	{
		if(GPIO_ReadInputDataBit(STOSTE_PORT,STOSTE1_PIN) || stoste1_state == 0)
		{
			return EMPTY;
		}
		else
		{
			return NOTEMPTY;
		}
	}
	else
	{
		if(!(GPIO_ReadInputDataBit(STOSTE_PORT,STOSTE2_PIN)) || stoste2_state == 0)
		{
			return EMPTY;
		}
		else
		{
			return NOTEMPTY;
		}
	}
}
