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

static uint16_t FLASH_ReadOptionByteData(uint32_t address)
{
  return *(__IO uint8_t*)address; //读一个字节后返回 
}

uint8_t Init_Read_Flash(void)
{
  uint32_t Address = WRITE_START_ADDR;
  
  if(FLASH_ReadOptionByteData(Address)!=1)
  {
    return 0;//未断电过
  }
  else
  {
    Address++;
  }
  product.Stoste_now=FLASH_ReadOptionByteData(Address);Address++;
  product.CO2_now=FLASH_ReadOptionByteData(Address);Address++;
  material_last.stoste1=FLASH_ReadOptionByteData(Address);Address++;
  material_last.stoste2=FLASH_ReadOptionByteData(Address);Address++;
  material_last.CO2_1=FLASH_ReadOptionByteData(Address);Address++;
  material_last.CO2_2=FLASH_ReadOptionByteData(Address);Address++;
  
  return 1;
  
}

void Flash_Save(void)
{
  uint32_t Address = WRITE_START_ADDR;
  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR|FLASH_FLAG_EOP);//清空所有标志位
  FLASH_ErasePage(WRITE_START_ADDR);//只擦除了一页
  //将数据写入
  FLASH_ProgramOptionByteData(Address, 1);Address++;//第一位作为断电标志位,表示已断电
  FLASH_ProgramOptionByteData(Address, product.Stoste_now);Address++;
  FLASH_ProgramOptionByteData(Address, product.CO2_now);Address++;
  FLASH_ProgramOptionByteData(Address, material_last.stoste1);Address++;
  FLASH_ProgramOptionByteData(Address, material_last.stoste2);Address++;
  FLASH_ProgramOptionByteData(Address, material_last.CO2_1);Address++;
  FLASH_ProgramOptionByteData(Address, material_last.CO2_2);Address++;
  
  FLASH_Lock();
  
}

void  PVD_IRQHandler(void)  
{ 

	if(PWR_GetFlagStatus(PWR_FLAG_PVDO))
	{
		//SET DATE TO FLASH
		Flash_Save();
	}
	EXTI_ClearITPendingBit(EXTI_Line16); 
}



