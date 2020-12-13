#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "production.h"
#include "schedule.h"

//余额
typedef union money_last_
{
	uint32_t money;
	uint8_t money_B[4];
}money_last_u;

//生产的体积
typedef union volume_produce_
{
	uint16_t volume;
	uint8_t volume_B[2];
}volume_produce_u;

typedef union unit_price
{
	uint16_t unit_price;
	uint8_t unit_B[2];
}unit_price_u;

typedef enum client_state_
{
	CLIENT_INIT = 0,
	CLIENT_BEGIN = 1,
	CLIENT_STOP = 2,
	CHANGE_STOSTE1_BEGIN = 3,
	CHANGE_STOSTE1_FINISH = 4,
	CHANGE_STOSTE2_BEGIN= 5,
	CHANGE_STOSTE2_FINISH = 6,
	CHANGE_CO2_1_BEGIN = 7,
	CHANGE_CO2_1_FINISH = 8,
	CHANGE_CO2_2_BEGIN = 9,
	CHANGE_CO2_2_FINISH = 10
}client_state_e;

typedef struct client_
{
	money_last_u money_last;
	unit_price_u unit_price ;
	volume_produce_u volume_produce_last;
	volume_produce_u volume_produce_next;
	client_state_e state;
	uint8_t fault_1;
	uint8_t fault_2;
	uint8_t fault_3;
	uint8_t fault_4;
	uint8_t fault_5;
}client_s;

extern client_s client;
extern uint16_t connect_flag;
extern uint16_t limit_flag;
uint32_t Client_Settlemen(void);
void Client_Send_Origin(client_state_e state);
void Client_Send(void);
void Client_Order(void);
void Client_VolumeUpdata(void);
#endif
