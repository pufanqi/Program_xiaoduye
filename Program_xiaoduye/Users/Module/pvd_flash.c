#include "pvd_flash.h"

void PVD_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	PWR_PVDLevelConfig(PWR_PVDLevel_2V8); //这里电压合不合适？？
	PWR_PVDCmd(ENABLE); 
	EXTI_DeInit();
	EXTI_StructInit(&EXTI_InitStructure);  
	EXTI_InitStructure.EXTI_Line = EXTI_Line16; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
	EXTI_Init(&EXTI_InitStructure); 

		
}
void  PVD_IRQHandler(void)  
{ 

	if(PWR_GetFlagStatus(PWR_FLAG_PVDO))
	{
		//SET DATE TO FLASH
		//FLASH_WriteByte(USER_ADDR , date);
	}
	EXTI_ClearITPendingBit(EXTI_Line16); 
}



