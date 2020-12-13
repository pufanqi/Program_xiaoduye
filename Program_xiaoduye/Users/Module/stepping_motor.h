#ifndef _STEPPING_MOTOR_H_
#define _STEPPING_MOTOR_H_

#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"

//PU
#define PU_CLK  RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO
#define PU_PORT GPIOB
#define PU_PIN  GPIO_Pin_5
#define RCC_PU_FUN RCC_APB1PeriphClockCmd
#define PU_TIM_CLK RCC_APB1Periph_TIM3
#define PU_TIM  TIM3

//DR
#define DR_CLK  RCC_APB2Periph_GPIOC
#define DR_PORT GPIOC
#define DR_PIN  GPIO_Pin_1

//MF
#define MF_CLK  RCC_APB2Periph_GPIOC
#define MF_PORT GPIOC
#define MF_PIN  GPIO_Pin_2

//定义方向
typedef enum direction
{
	DIRE_PUSH = 0,  //使滑块远离步进电机
	DIRE_PULL = 1   //使滑块靠近步进电机
}direction_t;

//定义制动
typedef enum MF_state_
{
	MF_ENABLE = 0,
	MF_DISABLE,
}MF_State_e;

void Motor_Init(void);
void Motor_Enable(direction_t dire);
void Motor_Disable(void);
void Motor_Setperiod(uint16_t speed);
void Motor_SetDirection(direction_t dire);

#endif
