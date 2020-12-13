#ifndef __RELAY_H__
#define __RELAY_H__

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

//原液电磁阀1
#define RELAY1_CLK  RCC_APB2Periph_GPIOB
#define RELAY1_PORT GPIOB
#define RELAY1_PIN  GPIO_Pin_0
//原液电磁阀2
#define RELAY2_CLK  RCC_APB2Periph_GPIOB
#define RELAY2_PORT GPIOB
#define RELAY2_PIN  GPIO_Pin_1
//清洗电磁阀
#define RELAY3_CLK  RCC_APB2Periph_GPIOB
#define RELAY3_PORT GPIOB
#define RELAY3_PIN  GPIO_Pin_3
//CO2电磁阀1
#define RELAY4_CLK  RCC_APB2Periph_GPIOB
#define RELAY4_PORT GPIOB
#define RELAY4_PIN  GPIO_Pin_6
//CO2电磁阀2
#define RELAY5_CLK  RCC_APB2Periph_GPIOB
#define RELAY5_PORT GPIOB
#define RELAY5_PIN  GPIO_Pin_7
//纯净水电磁阀
#define RELAY6_CLK  RCC_APB2Periph_GPIOB
#define RELAY6_PORT GPIOB
//#define RELAY6_PIN  GPIO_Pin_2
#define RELAY6_PIN  GPIO_Pin_6
//原液总电磁阀
#define RELAY7_CLK  RCC_APB2Periph_GPIOB
#define RELAY7_PORT GPIOB
#define RELAY7_PIN  GPIO_Pin_2

typedef enum Realy_
{
	relay1 = 0,
	relay2,
	relay3,
	relay4,
	relay5,
	relay6,
	relay7,
}Relay_e;

typedef enum RelayState_
{
	RELAY_OFF = 0,   //关断
	RELAY_ON = 1    //开通
}RelayState_e;

void Relay_Init(void);
void Relay_Set(Relay_e relay_x,RelayState_e state);
void Relay_OpenWater(void);
void Relay_CloseWater(void);
void ChangeCO2(uint8_t id);
void ChangeStoste(uint8_t temp);
void Relay_CloseALL(void);

#endif
