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

//GPRSͨ��Э��-����
typedef struct gprs_msg_TX_
{
	uint8_t head;          //0:֡ͷ
	uint8_t length;        //1������
	uint8_t sequence;      //2�����ݰ�����
	uint8_t msg_id;        //3����ϢID
	uint8_t equipment_id_1;//4���豸ID1
	uint8_t equipment_id_2;//5���豸ID2
	uint8_t equipment_id_3;//6���豸ID3
	uint8_t equipment_id_4;//7���豸ID4
	uint8_t LAC_H;         //8��LAC��8λ
	uint8_t LAC_L;         //9��LAC��8λ
	uint8_t CID_H;         //10��CID��8λ
	uint8_t CID_L;         //11��CID��8λ
	uint8_t money_H;       //12����������8λ
	uint8_t money_M;       //13�����������8λ
	uint8_t money_L;       //14����������8λ
	uint8_t produce_H;     //15��������������Һ�����8λ
	uint8_t produce_L;     //16��������������Һ�����8λ
	uint8_t stoste_last_1; //17��ԭҺʣ��1
	uint8_t stoste_last_2; //18��ԭҺʣ��2
	uint8_t CO2_last_1;    //19��CO2-1����
	uint8_t CO2_last_2;    //20��CO2-2����
	uint8_t status;        //21���豸����״̬
	uint8_t fault_1;       //22���豸���ϴ���1
	uint8_t fault_2;       //23���豸���ϴ���2
	uint8_t fault_3;       //24���豸���ϴ���3
	uint8_t fault_4;       //25���豸���ϴ���4
	uint8_t fault_5;       //26���豸���ϴ���5
	uint8_t check_L;       //27��У��λ����8λ��
	uint8_t check_H;       //28��У��λ����8λ��
}gprs_msg_TX_s;

//GPRSͨ��Э��-����
typedef struct gprs_msg_RX_
{
	uint8_t head;          //0:֡ͷ
	uint8_t length;        //1������
	uint8_t sequence;      //2�����ݰ�����
	uint8_t msg_id;        //3����ϢID
	uint8_t equipment_id_1;//4���豸ID1������8λ��
	uint8_t equipment_id_2;//5���豸ID2������8λ��
	uint8_t equipment_id_3;//6���豸ID3��ǰ���8λ��
	uint8_t equipment_id_4;//7���豸ID4��ǰ���8λ��
	uint8_t respond;       //8����Ӧ��
	uint8_t isDisable;     //9���豸���ñ�־
	uint8_t isMoneyRemain; //10���豸����־λ
	uint8_t money_H;       //11����ǰ�˻�����8λ
	uint8_t money_M;       //12����ǰ�˻������8λ
	uint8_t money_L;       //13����ǰ�˻�����8λ
	uint8_t unit_cost_H;   //14����ǰ�˻����۸�8λ
	uint8_t unit_cost_L;   //15����ǰ�˻����۵�8λ
	uint8_t reserve1;      //16�������ֽ�1
	uint8_t reserve2;      //17�������ֽ�2
	uint8_t reserve3;      //18�������ֽ�3
	uint8_t reserve4;      //19�������ֽ�4
	uint8_t check_L;        //20��У��λ���Ͱ�λ��
	uint8_t check_H;        //21��У��λ���߰�λ��
}gprs_msg_RX_s;

typedef union gprs_msg_
{
	gprs_msg_RX_s msg_RX;
	uint8_t RX_string[22];
	gprs_msg_TX_s msg_TX;
	uint8_t TX_string[29];
}gprs_msg_union;    //ͨ��Э�鹲���壬���ڽ�������

extern gprs_msg_TX_s GPRS_msg_TX;
extern gprs_msg_RX_s GPRS_msg_RX;
extern gprs_msg_union GPRS_msg;
extern uint8_t GPRS_isRead;

void GPRS_Init(void);
void GPRS_Order(void);
void GPRS_Send(gprs_msg_TX_s *msg);

#endif
