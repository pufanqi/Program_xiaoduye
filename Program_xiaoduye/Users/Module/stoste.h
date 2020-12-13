#ifndef __STOSTE_H__
#define __STOSTE_H__

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"

#define STOSTE_CLK  RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO
#define STOSTE_PORT GPIOC

//¼à²âÆ÷1
#define STOSTE1_PIN  GPIO_Pin_3
#define STOSTE1_PORT_SOURCE GPIO_PortSourceGPIOC
#define STOSTE1_PIN_SOURCE GPIO_PinSource3
#define STOSTE1_EXTI_LINE EXTI_Line3
#define STOSTE1_EXTI_IRQ EXTI3_IRQn
//¼à²âÆ÷2
#define STOSTE2_PIN  GPIO_Pin_4
#define STOSTE2_PORT_SOURCE GPIO_PortSourceGPIOC
#define STOSTE2_PIN_SOURCE GPIO_PinSource3
#define STOSTE2_EXTI_LINE EXTI_Line4
#define STOSTE2_EXTI_IRQ EXTI4_IRQn

extern uint8_t stoste1_state;  //0£ºÎÞÔ­Òº 1£ºÓÐÔ­Òº
extern uint8_t stoste2_state;

typedef enum stoste_state_
{
	EMPTY = 0,
	NOTEMPTY = 1,
}stoste_state_t;

void Stoste_Init(void);
stoste_state_t Stoste_GetStatus(uint8_t id);
#endif
