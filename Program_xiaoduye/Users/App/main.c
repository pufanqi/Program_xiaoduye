#include <stm32f10x.h>
#include "production.h"
#include "schedule.h"
#include "client.h"
#include "test.h"

uint8_t StopFlag = 0;  //���ڵ���ʱ����ֹͣ
uint16_t period_test = 25;
int main()
{
	//module_e m=stepping;
	PVD_Init();						//�������ʼ��
	Production_Init();   //��ģ���ʼ��
	Client_Send_Origin(CLIENT_INIT);   //���ͳ�ʼ����GPRS��Ϣ
	Timer_Init();        //��ʱ����ʼ��
	//������
	//Production_Start();
	//production_state = PRODUCTION_NORMAL;
	GPIO_SetBits(RELAY1_PORT,RELAY1_PIN);
	
	while(1)
	{
//		if(StopFlag == 0)
//		{
//		  Schedule_StateMchine();   //״̬������
//			Schedule_Scan();          //��Ҫѭ��ɨ��ĺ���
//		}
//		else
//		{
//			Production_Stop();
//		}
		  Test_stoste(period_test);
	}
}

const uint32_t Screen_Period = 200;    //����Ļ���͵�����
const uint32_t Material_Period = 200;  //����ԭ��ʣ�������
const uint32_t VolumeProduce_period = 1000;  //�����������
const uint32_t GPRS_Period = 3600;			//����������������źţ�ʵ������3600s
const uint32_t Response_Period = 4;			//�ȴ���������Ӧ���Ȳ�����5s�ط�һ��
void TIM6_IRQHandler()
{
	static uint32_t Screen_Slice = 0;
	static uint32_t Material_Slice = 0;
	static uint32_t VolumeProduce_Slice = 0;
	static uint32_t GPRS_Slice = 0;
	static uint32_t Response_Slice = 0;
	
	//��ʱ����ԭ��ʣ��
	if(production_state == PRODUCTION_NORMAL)
	{
		if(Material_Slice>=Material_Period)
	  {
	  	Material_Updata();
	  	Material_Slice = 0;
	  }
	  else
	  {
	  	Material_Slice++;
	  }
	}
	
		//��ʱ�����������
	if(production_state == PRODUCTION_NORMAL)
	{
		if(VolumeProduce_Slice>=VolumeProduce_period)
	  {
	  	Client_VolumeUpdata();
	  	VolumeProduce_Slice = 0;
	  }
	  else
	  {
	  	VolumeProduce_Slice++;
	  }
	}
	
	//��ʱ�������ݸ���Ļ��GPRS
	if(Screen_Slice>=Screen_Period)
	{
		screen_TX_s msg;
		uint8_t temp;
		
		msg.CO2_1 = material_last.CO2_1;
		msg.CO2_2 = material_last.CO2_2;
		msg.stoste1 = material_last.stoste1;
		msg.stoste2 = material_last.stoste2;
		if(production_state == PRODUCTION_NORMAL)
		{
			msg.state = 0;
		}
		else
		{
			msg.state = 1;
		}
		msg.ID_Type = id_data.ID_type;
		for(temp=0;temp<5;temp++)
		{
			msg.ID[temp] = id_data.ID[temp];
		}
		msg.money_H = GPRS_msg_RX.money_H;
		msg.money_M = GPRS_msg_RX.money_M;
		msg.money_L = GPRS_msg_RX.money_L;
		
		Screen_Senddata(&msg);
		
		Screen_Slice = 0;
		
		if(Response_Slice>=Response_Period)
		{
			Response_Slice = 0;
			if(response_flag == 0)
			{
				Client_Send();
			}
		}
		else 
		{
			Response_Slice++;
		}
		
		if(GPRS_Slice>=GPRS_Period)
		{
			Client_Send();
			GPRS_Slice = 0;
		}
		else 
		{
			GPRS_Slice++;
		}
	}
	else
	{
		Screen_Slice++;
	}
	
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM6,TIM_FLAG_Update);
	}
}
