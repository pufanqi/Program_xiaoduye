#include "schedule.h"

#define DEBUG 1
#define PRODUCE_LIMIT 1000

static uint16_t limit = 0;
uint16_t limit_flag = 0;
state_t production_state = STOP;
//���ڵ��Եļ����
uint16_t water_flux_dbg = 1000; 
uint16_t CO2_flux_dbg = 0;
stoste_state_t stoste1_dbg = EMPTY;
stoste_state_t stoste2_dbg = NOTEMPTY;

/*************************************************************************************************************************************************/
/*************************************************���ȣ���������״̬(BEGIN)************************************************************************************************/
//���ȣ���������״̬
void Schedule_Normal()
{
	screen_RX_s msg_rx;
	//��������ָ��
	Screen_Getdata(&msg_rx);
	//��ȡ����������
	Production_SensorData();
	
	//�ж��Ƿ������ԭ��
	if(msg_rx.command == 0x02 && msg_rx.isRead == 0)
	{
		switch(msg_rx.data)
		{
			case 0x01:material_last.stoste1 = 100;client.fault_1=0;break;
			case 0x02:material_last.stoste2 = 100;client.fault_1=0;break;
			case 0x03:material_last.CO2_1 = 100;client.fault_2=0;break;
			case 0x04:material_last.CO2_2 = 100;client.fault_2=0;break;
		}
		Client_Send();
	}
	
#ifdef DEBUG   //�õ��Ա�������������ֵ
	product.Water_Flux = water_flux_dbg;
	product.CO2_Flux = CO2_flux_dbg;
	if(product.Stoste_now == 0)
	{
		product.stoste_state = stoste1_dbg;
	}
	else
	{
		product.stoste_state = stoste2_dbg;
	}
#endif
	
	//�ж��Ƿ�ֹͣ
	if(msg_rx.command == 0x01 && msg_rx.isRead == 0)
	{
		Production_Stop();
		production_state = STOP;
		return;
	}
	
	if(product.Water_Flux<=0)
	{
		Production_Wait();
		production_state = PRODUCTION_WAIT;
		return;
	}

	//CO2���޿���
	if(product.CO2_Flux<CO2_LIMIT)
	{
		if(product.CO2_now == 0)
		{
			material_last.CO2_1 = 0;
			if(material_last.CO2_2 <= 0)
			{
				Production_Wait();production_state = PRODUCTION_WAIT; SysTick_Delay_ms(500);
				client.fault_2=4;//����Ӧ�þ���
				Client_Send();
				return;
			}
			else
			{
				ChangeCO2(1);product.CO2_now = 1; 
				client.fault_2=1;
				Client_Send();
				return;
			}	
		}
		else if(product.CO2_now == 1)
		{
			material_last.CO2_2 = 0;
			if(material_last.CO2_1 <= 0)
			{
				Production_Wait();production_state = PRODUCTION_WAIT; SysTick_Delay_ms(500);
				client.fault_2=4;
				Client_Send();
				return;
			}
			else
			{
				ChangeCO2(0);product.CO2_now = 0;
				client.fault_2=2;
				Client_Send();
				return;
			}
		}
	}
	
	//ԭҺ���޿���
	if(product.stoste_state == EMPTY)
	{
		if(product.Stoste_now == 0)
		{
			material_last.stoste1 = 0;
			if(material_last.stoste2<= 0)
			{
				Production_Wait();production_state = PRODUCTION_WAIT; 
				client.fault_1=4;
				Client_Send();
				return;
			}
			else
			{
				ChangeStoste(1);product.Stoste_now = 1;
				client.fault_1=1;
				Client_Send();
				return;
			}
		}
		else if(product.Stoste_now == 1)
		{
			material_last.stoste2 = 0;
			if(material_last.stoste1<= 0)
			{
				Production_Wait();production_state = PRODUCTION_WAIT;
				client.fault_1=4;
				Client_Send();
				return;
			}
			else
			{
				ChangeStoste(0);product.Stoste_now = 0;
				client.fault_1=2;
				Client_Send();
				return;
			}
		}
	}
	//�����������100Lʱת�������״̬
	if(connect_flag == 0)
	{
		if(limit_flag == 0)
		{
			limit = client.volume_produce_next.volume;
			limit_flag = 1;
		}
		else
		{
				if(client.volume_produce_next.volume >= limit+PRODUCE_LIMIT)
			{
				Production_Wait();production_state = PRODUCTION_WAIT;
			}
		}
		
		
	}
	
	//��λ���ؿ���
	switch(product.Approach_state)
	{
		case 1://Motor_SetDirection(DIRE_PUSH);product.Motor_DR = DIRE_PUSH; break;
		case 2:Motor_Enable(DIRE_PULL);product.Motor_DR = DIRE_PULL; break;
	}
	MotorControl(product.Water_Flux);
}
/*****************************************************���ȣ���������״̬(END)********************************************************************************************/
/*****************************************************���ȣ��ȴ�״̬(BEGIN)********************************************************************************************/
//���ȣ��ȴ�״̬
void Shedule_Wait()
{
	screen_RX_s msg_rx;
	
	//��������ָ��
	Screen_Getdata(&msg_rx);
	
	if(msg_rx.isRead == 0)
	{
		if(msg_rx.command == 0x00)
		{
			Production_Start();production_state = PRODUCTION_NORMAL;
		}
		else if(msg_rx.command == 0x01)
	  {
	  	Production_Stop();production_state = STOP;
	  }
		else if(msg_rx.command == 0x02)
		{
			switch(msg_rx.data)
			{
				case 0x01: //����ԭҺ1
				{
					ChangeStoste(0);
					stoste1_state = 1;
					material_last.stoste1 = 100;
					product.Stoste_now = 0;
					client.state=CHANGE_STOSTE1_FINISH;
					client.fault_1=0;
					Production_Adjust();
					Production_Start();
					production_state = PRODUCTION_NORMAL;
				}break;
				case 0x02: //����ԭҺ2
				{
					ChangeStoste(1);
					stoste2_state = 1;
					material_last.stoste2 = 100;
					product.Stoste_now = 1;
					client.state=CHANGE_STOSTE2_FINISH;
					client.fault_1=0;
					Production_Adjust();
					Production_Start();
					production_state = PRODUCTION_NORMAL;
				}break;
				case 0x03: //����CO2����1
				{
					ChangeCO2(0);
					material_last.CO2_1 = 100;
					product.CO2_now = 0;
					client.state=CHANGE_CO2_1_FINISH;
					client.fault_2=0;
					Production_Start();
					production_state = PRODUCTION_NORMAL;
				}break;
				case 0x04: //����CO2����2
				{
					ChangeCO2(1);
					material_last.CO2_2 = 100;
					product.CO2_now = 1;
					client.state=CHANGE_CO2_2_FINISH;
					client.fault_2=0;
					Production_Start();
					production_state = PRODUCTION_NORMAL;
				}break;
			}
		}
	}

}
/*******************************************************���ȣ��ȴ�״̬(END)******************************************************************************************/
/*******************************************************���ȣ�ֹͣ״̬(BEGIN)******************************************************************************************/
//���ȣ�ֹͣ״̬
void Schedule_Stop()
{
	screen_RX_s msg_rx;
	
	//��������ָ��
	Screen_Getdata(&msg_rx);
	
	if(msg_rx.command == 0x02 && msg_rx.isRead == 0)
	{
		switch(msg_rx.data)
		{
			case 0x01:material_last.stoste1 = 100;client.state=CHANGE_STOSTE1_FINISH;client.fault_1=0;break;
			case 0x02:material_last.stoste2 = 100;client.state=CHANGE_STOSTE2_FINISH;client.fault_1=0;break;
			case 0x03:material_last.CO2_1 = 100;client.state=CHANGE_CO2_1_FINISH;client.fault_2=0;break;
			case 0x04:material_last.CO2_2 = 100;client.state=CHANGE_CO2_2_FINISH;client.fault_2=0;break;
		}
		Client_Send();
	}
	
	if(msg_rx.isRead == 0)
	{
		if(msg_rx.command == 0x00)
		{
			Production_Start();
			production_state = PRODUCTION_NORMAL;
		}
	}
}
/***********************************************************���ȣ�ֹͣ״̬(END)**************************************************************************************/
/***********************************************************״̬��(BEGIN)**************************************************************************************/
//״̬��
void Schedule_StateMchine()
{
	switch(production_state)
	{
		case STOP:Schedule_Stop();break;
		case PRODUCTION_NORMAL:Schedule_Normal();break;
		case PRODUCTION_WAIT:Shedule_Wait();break;
	}
}
/***********************************************************״̬��(END)**************************************************************************************/
/***********************************************************����ɨ�������(BEGIN)**************************************************************************************/
//��GPRS������Ӧ
static void Schedule_Handle_GPRS()
{
	uint16_t check = 0;
	uint8_t *msg_p;
	uint8_t temp = 0;
	
	msg_p = (uint8_t*)&GPRS_msg_RX;
	
	//���У��λ
	for(temp=0;temp<GPRS_msg_RX.length-2;temp++)
	{
		check += *(msg_p+temp);
	}
	if(check == ((GPRS_msg_RX.check_H<<8)+GPRS_msg_RX.check_L))
	{
		//��������ݰ���������
		check = 0;
		return;
	}
	return;
}
//����Ļ���ݵ���Ӧ
static void Schedule_Handle_Screen()
{
	id_data.ID_type = Card_GetType();
	//����ˢ����Ϣ
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
	id_data.ID_type = 0;
	for(temp=0;temp<5;temp++)
	{
		msg.ID[temp] = id_data.ID[temp];
	}
	msg.money_H = GPRS_msg_RX.money_H;
	msg.money_M = GPRS_msg_RX.money_M;
	msg.money_L = 100;
	
	Screen_Senddata(&msg);
}

//����ɨ�������
void Schedule_Scan()
{
	//����ˢ����Ϣ
	if(id_data.isreading == 0)
	{
		id_data.isreading = 1;
		Schedule_Handle_Screen();
	}
	
	//����GPRS��Ϣ
	if(GPRS_isRead == 0)
	{
		GPRS_isRead = 1;
		Schedule_Handle_GPRS();
	}
}
/***********************************************************����ɨ�������(END)**************************************************************************************/
/*************************************************************************************************************************************************/
