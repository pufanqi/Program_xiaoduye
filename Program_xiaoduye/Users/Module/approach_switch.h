#ifndef __APPROACH_H__
#define __APPROACH_H__

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"

#define APPROACH_CLK  RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO
#define APPROACH_PORT GPIOC

#define APPROACH_PIN1  GPIO_Pin_8
#define APPROACH_PORT_SOURCE1 GPIO_PortSourceGPIOC
#define APPROACH_PIN_SOURCE1 GPIO_PinSource8
#define APPROACH_LINE1 EXTI_Line8
#define APPROACH_IRQ1 EXTI9_5_IRQn

#define APPROACH_PIN2  GPIO_Pin_9
#define APPROACH_PORT_SOURCE2 GPIO_PortSourceGPIOC
#define APPROACH_PIN_SOURCE2 GPIO_PinSource9
#define APPROACH_LINE2 EXTI_Line9
#define APPROACH_IRQ2 EXTI9_5_IRQn

void Approach_Init(void);
uint8_t Approach_State(void);
#endif
