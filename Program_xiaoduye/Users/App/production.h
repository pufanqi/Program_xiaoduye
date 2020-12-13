#ifndef __PRODUCTION_H__
#define __PRODUCTION_H__

#include "stm32f10x.h"
#include "module.h"

#define CO2_LIMIT 600

//�����������
typedef struct production
{
	direction_t Motor_DR;  //����������з���
	MF_State_e Motor_MF;   //��������ƶ��Ƿ�ʹ��
	uint8_t Stoste_now;    //��ǰʹ�õ�ԭҺ��
	stoste_state_t stoste_state;
	uint16_t CO2_Flux;     //CO2����
	uint8_t CO2_now;       //��ǰʹ�õ�CO2��
	uint16_t Water_Flux;   //����ˮ����
	uint8_t Approach_state; //��λ����״̬��0����������  1�������������������  2������Զ�벽�������
}production_t;

//����ʣ������
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
