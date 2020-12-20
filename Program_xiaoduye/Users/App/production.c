#include "production.h"

production_t product;
material_last_t material_last;
uint16_t response_flag = 0;

//生产初始化
void Production_Init()
{
	
  if(Init_Read_Flash()==0)//未产生断电则赋初始值
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
  product.Motor_DR = DIRE_PULL;  //初始状态为：远离步进电机
	product.Motor_MF = MF_ENABLE; //初始状态为：使能制动
	product.CO2_Flux = 0;
	product.Water_Flux = 0;

	
	
	
	
	client.state=CLIENT_INIT;
	client.fault_1=0;
	client.fault_2=0;
	client.fault_3=0;
	client.fault_4=0;
	client.fault_5=0;
  
	Approach_Init();  //限位开关
	Motor_Init();     //步进电机
	CO2Flux_Init();   //CO2检测
	Water_Init();     //水流量检测
	Relay_Init();     //电磁阀
	Stoste_Init();    //原液检测
	Screen_Init();    //屏幕初始化
	Card_Init();      //读卡器初始化
	GPRS_Init();      //GPRS初始化
	
	Production_Adjust(); //步进电机校准
	
}

//转入生产状态的操作
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

//转入停止状态的操作
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
	Relay_CloseALL();  //不包括纯净水阀
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
	Relay_CloseALL(); //不包括纯净水阀
	Relay_CloseWater();
	client.state=CLIENT_STOP;
	Client_Send();
	
}

//传感器数据监测
void Production_SensorData()
{
	//获取限位开关状态（防止上电就是触发，导致不能触发中断）
	product.Approach_state = Approach_State();
	//获取水流量
//	product.Water_Flux = Water_GetFreq();
	product.Water_Flux = Water_GetFreq();
	//检查CO2流量
	product.CO2_Flux = GetCO2Val();
	//检查原液状态
	product.stoste_state = Stoste_GetStatus(product.Stoste_now);
}

//根据流量和周期的关系控制步进电机
void MotorControl(uint16_t flux)
{
	Motor_Setperiod(65535/flux);   //flux=2100  <-> period = 100us
}

//更新原料剩余量
void Material_Updata()
{
	//更新二氧化碳用量
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
	
	//更新原液用量
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

//步进电机校准
void Production_Adjust()
{
	//Relay_Set(relay1,RELAY_ON);  //校准的时候关闭1号原液罐，是因为调试的时候1号罐没有原液，会进入空气
//	Relay_Set(relay2,RELAY_ON);
	
	NVIC_DisableIRQ(APPROACH_IRQ1);
	NVIC_DisableIRQ(APPROACH_IRQ2);

	//PULL:排掉剩余的液体
	Relay_Set(relay7,RELAY_ON);
	Motor_Setperiod(3);
	Motor_Enable(DIRE_PULL);
	while(Approach_State() != 1)
	{
		
	}
	SysTick_Delay_ms(80);
	
	//PUSH：抽取原液
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
