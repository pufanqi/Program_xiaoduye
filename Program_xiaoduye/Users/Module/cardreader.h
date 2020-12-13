#ifndef __CARDREADER_H__
#define __CARDREADER_H__

#include "stdio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdlib.h>

#define DEBUG_USARTx USART3

//USART3
#define CARDREADER_USART      USART3
#define CARDREADER_GPIO_CLK   RCC_APB2Periph_GPIOB
#define CARDREADER_TX_PORT    GPIOB
#define CARDREADER_TX_PIN     GPIO_Pin_10;
#define CARDREADER_RX_PORT    GPIOB
#define CARDREADER_RX_PIN     GPIO_Pin_11;

typedef struct ID_data
{
	uint8_t isreading;
	uint8_t flag;	//0:未开始  1：接收了起始符  2：接收了长度  3：接收了卡号 4：接收了校验
	uint8_t type;
	uint8_t length;
	uint8_t ID[5];
	uint8_t check;
	uint8_t ID_type;  //0：无效卡 1：管理员卡 2：用户卡
}ID_data_t;

extern ID_data_t id_data;
void Card_Init(void);
void Card_SaveID(uint8_t length,uint8_t *ID);
uint8_t Card_GetType(void);
void Card_Order(void);

#endif
