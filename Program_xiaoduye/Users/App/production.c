#include "production.h"

production_t product;
material_last_t material_last;
uint16_t response_flag = 0;

//������ʼ��
void Production_Init()
{
	
  if(Init_Read_Flash()==0)//δ�����ϵ��򸳳�ʼֵ
  {
    product.Stoste_now = 1;
    product.CO2_now = 0;
    
    material_last.stoste1 = 100;
    material_last.stoste2 = 100;
    material_last.CO2_1 = 100;
    material_last.CO2_2 = 100;
    material_last.CO2_max = 100;
    material_last.stoste_max = 100;
  }
  product.Motor_DR = DIRE_PULL;  //��ʼ״̬Ϊ��Զ�벽�����
	product.Motor_MF = MF_ENABLE; //��ʼ״̬Ϊ��ʹ���ƶ�
	product.CO2_Flux = 0;
	product.Water_Flux = 0;

	
	
	
	
	client.state=CLIENT_INIT;
	client.fault_1=0;
	client.fault_2=0;
	client.fault_3=0;
	client.fault_4=0;
	client.fault_5=0;
  
	Approach_Init();  //��λ����
	Motor_Init();     //�������
	CO2Flux_Init();   //CO2���
	Water_Init();     //ˮ�������
	Relay_Init();     //��ŷ�
	Stoste_Init();    //ԭҺ���
	Screen_Init();    //��Ļ��ʼ��
	Card_Init();      //��������ʼ��
	GPRS_Init();      //GPRS��ʼ��
	
	Production_Adjust(); //�������У׼
	
}

//ת������״̬�Ĳ���
void Production_Start()
{
	Motor_Enable(product.Motor_DR);
	product.Motor_MF = MF_ENABLE;
	Relay_OpenWater();
	ChangeCO2(product.CO2_now);
	ChangeStoste(product.Stoste_now);
	client.state=CLIENT_BEGIN;
	Client_Send();
}

//ת��ֹͣ״̬�Ĳ���
void Production_Stop()
{
	
//	GPIO_ResetBits(RELAY2_PORT,RELAY2_PIN);
//	GPIO_SetBits(RELAY7_PORT,RELAY7_PIN);
//	Motor_Enable(DIRE_PUSH);
//	Motor_Setperiod(25);
//	while(Approach_State() != 2)
//	{
//		
//	}
//	SysTick_Delay_ms(500);
	Motor_Disable();
	product.Motor_MF = MF_DISABLE;
	Relay_CloseALL();  //����������ˮ��
	Relay_CloseWater();
	client.state=CLIENT_STOP;
	Client_Send();
}

void Production_Wait()
{
//	GPIO_ResetBits(RELAY2_PORT,RELAY2_PIN);
//	GPIO_SetBits(RELAY7_PORT,RELAY7_PIN);
//	Motor_Enable(DIRE_PUSH);
//	Motor_Setperiod(25);
//	while(Approach_State() != 2)
//	{
//		
//	}
//	SysTick_Delay_ms(500);
	Motor_Disable();
	product.Motor_MF = MF_DISABLE;
	Relay_CloseALL(); //����������ˮ��
	Relay_CloseWater();
	client.state=CLIENT_STOP;
	Client_Send();
	
}

//���������ݼ��
void Production_SensorData()
{
	//��ȡ��λ����״̬����ֹ�ϵ���Ǵ��������²��ܴ����жϣ�
	product.Approach_state = Approach_State();
	//��ȡˮ����
//	product.Water_Flux = Water_GetFreq();
	product.Water_Flux = Water_GetFreq();
	//���CO2����
	product.CO2_Flux = GetCO2Val();
	//���ԭҺ״̬
	product.stoste_state = Stoste_GetStatus(product.Stoste_now);
}

//�������������ڵĹ�ϵ���Ʋ������
void MotorControl(uint16_t flux)
{
	Motor_Setperiod(65535/flux);   //flux=2100  <-> period = 100us
}

//����ԭ��ʣ����
void Material_Updata()
{
	//���¶�����̼����
	if(product.CO2_now == 0)
	{
		if(material_last.CO2_1 != 0)
		{
			material_last.CO2_1 -= 1;
		}
	}
	else
	{
		if(material_last.CO2_2 != 0)
		{
			material_last.CO2_2 -= 1;
		}
	}
	
	//����ԭҺ����
	if(product.Stoste_now == 0)
	{
		if(material_last.stoste1 != 0)
		{
			material_last.stoste1 -= 1;
		}
	}
	else
	{
		if(material_last.stoste2 != 0)
		{
			material_last.stoste2 -= 1;
		}
	}
}

//�������У׼
void Production_Adjust()
{
	//Relay_Set(relay1,RELAY_ON);  //У׼��ʱ��ر�1��ԭҺ�ޣ�����Ϊ���Ե�ʱ��1�Ź�û��ԭҺ����������
//	Relay_Set(relay2,RELAY_ON);
	
	NVIC_DisableIRQ(APPROACH_IRQ1);
	NVIC_DisableIRQ(APPROACH_IRQ2);

	//PULL:�ŵ�ʣ���Һ��
	Relay_Set(relay7,RELAY_ON);
	Motor_Setperiod(3);
	Motor_Enable(DIRE_PULL);
	while(Approach_State() != 1)
	{
		
	}
	SysTick_Delay_ms(80);
	
	//PUSH����ȡԭҺ
	Relay_Set(relay7,RELAY_OFF);
	Relay_Set(relay2,RELAY_ON);
	Motor_Setperiod(25);
	Motor_SetDirection(DIRE_PUSH);
	while(Approach_State() != 2)
	{
		
	}
	SysTick_Delay_ms(500);

	Motor_Disable();
	
	Relay_Set(relay2,RELAY_OFF);
	
	NVIC_EnableIRQ(APPROACH_IRQ1);
	NVIC_EnableIRQ(APPROACH_IRQ2);
}
