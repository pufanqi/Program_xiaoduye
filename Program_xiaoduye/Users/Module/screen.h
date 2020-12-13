#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "stdio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdlib.h>

//USART4
#define SCREEN_USART      UART4
#define SCREEN_GPIO_CLK   RCC_APB2Periph_GPIOA
#define SCREEN_TX_PORT    GPIOC
#define SCREEN_TX_PIN     GPIO_Pin_10;
#define SCREEN_RX_PORT    GPIOC
#define SCREEN_RX_PIN     GPIO_Pin_11;

typedef struct screen_TX_
{
	uint8_t front1;
	uint8_t front2;
	uint8_t state;
	uint8_t stoste1;
	uint8_t stoste2;
	uint8_t CO2_1;
	uint8_t CO2_2;
	uint8_t ID_Type;
	uint8_t ID[5];
	uint8_t money_H;
	uint8_t money_M;
	uint8_t money_L;
}screen_TX_s;

typedef struct screen_RX_
{
	uint8_t command;
	uint8_t data;
	uint8_t flag;
	uint8_t isRead;  //0£ºÎ´¶Á  1£ºÒÑ¶Á
}screen_RX_s;

void Screen_Init(void);
void Screen_Order(void);
void Screen_Getdata(screen_RX_s *msg);
void Screen_Senddata(screen_TX_s *msg);

#endif
