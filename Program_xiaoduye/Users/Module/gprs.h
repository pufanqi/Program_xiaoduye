#ifndef __GPRS_H__
#define __GPRS_H__

#include "stdio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdlib.h>

//USART1
#define GPRS_USART      USART1
#define GPRS_GPIO_CLK   RCC_APB2Periph_GPIOA
#define GPRS_TX_PORT    GPIOA
#define GPRS_TX_PIN     GPIO_Pin_9;
#define GPRS_RX_PORT    GPIOA
#define GPRS_RX_PIN     GPIO_Pin_10;

//GPRS通信协议-发送
typedef struct gprs_msg_TX_
{
	uint8_t head;          //0:帧头
	uint8_t length;        //1：长度
	uint8_t sequence;      //2：数据包序列
	uint8_t msg_id;        //3：消息ID
	uint8_t equipment_id_1;//4：设备ID1
	uint8_t equipment_id_2;//5：设备ID2
	uint8_t equipment_id_3;//6：设备ID3
	uint8_t equipment_id_4;//7：设备ID4
	uint8_t LAC_H;         //8：LAC高8位
	uint8_t LAC_L;         //9：LAC低8位
	uint8_t CID_H;         //10：CID高8位
	uint8_t CID_L;         //11：CID低8位
	uint8_t money_H;       //12：本地余额高8位
	uint8_t money_M;       //13：本地余额中8位
	uint8_t money_L;       //14：本地余额低8位
	uint8_t produce_H;     //15：本次生产消毒液体积高8位
	uint8_t produce_L;     //16：本次生产消毒液体积低8位
	uint8_t stoste_last_1; //17：原液剩余1
	uint8_t stoste_last_2; //18：原液剩余2
	uint8_t CO2_last_1;    //19：CO2-1余量
	uint8_t CO2_last_2;    //20：CO2-2余量
	uint8_t status;        //21：设备工作状态
	uint8_t fault_1;       //22：设备故障代码1
	uint8_t fault_2;       //23：设备故障代码2
	uint8_t fault_3;       //24：设备故障代码3
	uint8_t fault_4;       //25：设备故障代码4
	uint8_t fault_5;       //26：设备故障代码5
	uint8_t check_L;       //27：校验位（低8位）
	uint8_t check_H;       //28：校验位（高8位）
}gprs_msg_TX_s;

//GPRS通信协议-接收
typedef struct gprs_msg_RX_
{
	uint8_t head;          //0:帧头
	uint8_t length;        //1：长度
	uint8_t sequence;      //2：数据包序列
	uint8_t msg_id;        //3：消息ID
	uint8_t equipment_id_1;//4：设备ID1（后半高8位）
	uint8_t equipment_id_2;//5：设备ID2（后半低8位）
	uint8_t equipment_id_3;//6：设备ID3（前半高8位）
	uint8_t equipment_id_4;//7：设备ID4（前半低8位）
	uint8_t respond;       //8：响应码
	uint8_t isDisable;     //9：设备禁用标志
	uint8_t isMoneyRemain; //10：设备余额标志位
	uint8_t money_H;       //11：当前账户余额高8位
	uint8_t money_M;       //12：当前账户余额中8位
	uint8_t money_L;       //13：当前账户余额低8位
	uint8_t unit_cost_H;   //14：当前账户单价高8位
	uint8_t unit_cost_L;   //15：当前账户单价低8位
	uint8_t reserve1;      //16：备用字节1
	uint8_t reserve2;      //17：备用字节2
	uint8_t reserve3;      //18：备用字节3
	uint8_t reserve4;      //19：备用字节4
	uint8_t check_L;        //20：校验位（低八位）
	uint8_t check_H;        //21：校验位（高八位）
}gprs_msg_RX_s;

typedef union gprs_msg_
{
	gprs_msg_RX_s msg_RX;
	uint8_t RX_string[22];
	gprs_msg_TX_s msg_TX;
	uint8_t TX_string[29];
}gprs_msg_union;    //通信协议共用体，用于解析数据

extern gprs_msg_TX_s GPRS_msg_TX;
extern gprs_msg_RX_s GPRS_msg_RX;
extern gprs_msg_union GPRS_msg;
extern uint8_t GPRS_isRead;

void GPRS_Init(void);
void GPRS_Order(void);
void GPRS_Send(gprs_msg_TX_s *msg);

#endif
