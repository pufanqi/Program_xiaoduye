/***************************************************************************************/
//接近开关驱动程序
//型号：RIKO公司SN04-N型 
/***************************************************************************************/
#include "approach_switch.h"

void Approach_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	//GPIO初始化
	RCC_APB2PeriphClockCmd(APPROACH_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = APPROACH_PIN1|APPROACH_PIN2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(APPROACH_PORT,&GPIO_InitStructure);
	
	//EXTI配置
	GPIO_EXTILineConfig(APPROACH_PORT_SOURCE1,APPROACH_PIN_SOURCE1);
	EXTI_InitStructure.EXTI_Line = APPROACH_LINE1;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(APPROACH_PORT_SOURCE2,APPROACH_PIN_SOURCE2);
	EXTI_InitStructure.EXTI_Line = APPROACH_LINE2;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	//NVIC初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = APPROACH_IRQ1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = APPROACH_IRQ2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
}

uint8_t Approach_State()
{
	uint8_t approach1=0,approach2=0;
	
	approach1 = GPIO_ReadInputDataBit(APPROACH_PORT,APPROACH_PIN1);
	approach2 = GPIO_ReadInputDataBit(APPROACH_PORT,APPROACH_PIN2);
	
	if(approach1 == Bit_RESET)
	{
		return 1;
	}
	else if(approach2 == Bit_RESET)
	{
		return 2;
	}
	else
	{
		return 0;
	}
}
