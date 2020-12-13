#ifndef _CO2_FLUX_H_
#define _CO2_FLUX_H_

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"

#define CO2_CLK   RCC_APB2Periph_GPIOA
//CO2采样通道
#define CO2_PORT GPIOA
#define CO2_PIN  GPIO_Pin_7

extern uint16_t CO2_data;

void CO2Flux_Init(void);
uint16_t GetCO2Val(void);

#endif
