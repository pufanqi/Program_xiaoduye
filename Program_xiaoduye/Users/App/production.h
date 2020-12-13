#ifndef __PRODUCTION_H__
#define __PRODUCTION_H__

#include "stm32f10x.h"
#include "module.h"

#define CO2_LIMIT 600

//生产相关数据
typedef struct production
{
	direction_t Motor_DR;  //步进电机运行方向
	MF_State_e Motor_MF;   //步进电机制动是否使能
	uint8_t Stoste_now;    //当前使用的原液罐
	stoste_state_t stoste_state;
	uint16_t CO2_Flux;     //CO2流量
	uint8_t CO2_now;       //当前使用的CO2罐
	uint16_t Water_Flux;   //纯净水流量
	uint8_t Approach_state; //限位开关状态：0：都不触发  1：触发靠近步进电机的  2：触发远离步进电机的
}production_t;

//材料剩余数据
typedef struct material_last_
{
	uint8_t stoste1;
	uint8_t stoste2;
  uint8_t CO2_1;
	uint8_t CO2_2;
	uint8_t stoste_max;
	uint8_t CO2_max;
}material_last_t;

extern production_t product;
extern material_last_t material_last;
extern uint16_t response_flag;

void Production_Init(void);
void Production_SensorData(void);
void Production_Start(void);
void Production_Stop(void);
void Production_Wait(void);
void Material_Updata(void);
void MotorControl(uint16_t flux);
void Production_Adjust(void);
#endif
