#ifndef PVD_FLASH_H
#define PVD_FLASH_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_flash.h"
#include "production.h"

#define Flash_Page_Size    ((uint16_t)0x800)

#define WRITE_START_ADDR   ((uint32_t)0x08008000)
#define WRITE_END_ADDR     ((uint32_t)0x08008800)
//���ڸ��ݴ洢���ݴ�С���Ը��ģ�����ס����ҳ��Ŀǰֻ������һҳ��������Ҫͬʱ�Ѵ洢��Ŀ��ҳ������
void PVD_Init(void);
void Flash_Save(void);
uint8_t Init_Read_Flash(void);

#endif
