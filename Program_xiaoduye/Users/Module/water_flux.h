#ifndef _WATER_FLUX_H_
#define _WATER_FLUX_H_

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

//GPIO
#define WATER_CLK  RCC_APB2Periph_GPIOC
#define WATER_PORT GPIOC
#define WATER_PIN  GPIO_Pin_7

//TIM
#define RCC_WATER_FUN RCC_APB2PeriphClockCmd
#define WATER_TIM_CLK RCC_APB2Periph_TIM8
#define WATER_TIM TIM8
#define WATER_TIM_IRQ TIM8_CC_IRQn
#define WATER_TIM_IRQHANDLER TIM8_CC_IRQHandler

//·½²¨ÆµÂÊ
extern uint16_t freq;
extern uint32_t pulse_num ;

void Water_Init(void);
uint16_t Water_GetFreq(void);

#endif
