/***************************************************************************************/
//电磁阀驱动程序
//型号：
/***************************************************************************************/
#include "relay.h"
#include "delay.h"

//设置电磁阀通断
void Relay_Set(Relay_e relay_x,RelayState_e state)
{
	switch(relay_x)
	{
		case relay1:
		{
			if(state == RELAY_ON)
			{
				GPIO_ResetBits(RELAY1_PORT,RELAY1_PIN);
			}
			else
			{
				GPIO_SetBits(RELAY1_PORT,RELAY1_PIN);
			}
		}break;
		case relay2:
		{
			if(state == RELAY_ON)
			{
				GPIO_ResetBits(RELAY2_PORT,RELAY2_PIN);
			}
			else
			{
				GPIO_SetBits(RELAY2_PORT,RELAY2_PIN);
			}
		}break;
		case relay3:
		{
			if(state == RELAY_ON)
			{
				GPIO_ResetBits(RELAY3_PORT,RELAY3_PIN);
			}
			else
			{
				GPIO_SetBits(RELAY3_PORT,RELAY3_PIN);
			}
		}break;
		case relay4:
		{
			if(state == RELAY_ON)
			{
				GPIO_ResetBits(RELAY4_PORT,RELAY4_PIN);
			}
			else
			{
				GPIO_SetBits(RELAY4_PORT,RELAY4_PIN);
			}
		}break;
		case relay5:
		{
			if(state == RELAY_ON)
			{
				GPIO_ResetBits(RELAY5_PORT,RELAY5_PIN);
			}
			else
			{
				GPIO_SetBits(RELAY5_PORT,RELAY5_PIN);
			}
		}break;
		case relay6:
		{
			if(state == RELAY_ON)
			{
				GPIO_ResetBits(RELAY6_PORT,RELAY6_PIN);
			}
			else
			{
				GPIO_SetBits(RELAY6_PORT,RELAY6_PIN);
			}
		}break;
		case relay7:
		{
			if(state == RELAY_ON)
			{
				GPIO_ResetBits(RELAY7_PORT,RELAY7_PIN);
			}
			else
			{
				GPIO_SetBits(RELAY7_PORT,RELAY7_PIN);
			}
		}break;
	}
}

void Relay_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RELAY1_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = RELAY1_PIN|RELAY2_PIN|RELAY3_PIN|RELAY4_PIN|RELAY5_PIN|RELAY6_PIN|RELAY7_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RELAY1_PORT,&GPIO_InitStructure);
	
	Relay_Set(relay1,RELAY_OFF);
	Relay_Set(relay2,RELAY_OFF);
	Relay_Set(relay3,RELAY_OFF);
	Relay_Set(relay4,RELAY_OFF);
	Relay_Set(relay5,RELAY_OFF);
	Relay_Set(relay6,RELAY_OFF);
}

//打开水阀
void Relay_OpenWater()
{
	Relay_Set(relay6,RELAY_ON);
}
//关闭水阀
void Relay_CloseWater()
{
	Relay_Set(relay6,RELAY_OFF);
}

//更换CO2罐
void ChangeCO2(uint8_t id)
{
	if(id == 0)  
	{
		//Relay_Set(relay4,RELAY_ON);
		Relay_Set(relay5,RELAY_OFF);
		SysTick_Delay_ms(500);//二氧化碳换罐后需先通一段时间的气，防止系统再检测到流速过慢
		return;
	}
	if(id == 1)  
	{
		Relay_Set(relay5,RELAY_ON);
		//Relay_Set(relay4,RELAY_OFF);
		SysTick_Delay_ms(500);
		return;
	}
}

//更换原液电磁阀（需要调试具体开通哪一个）
void ChangeStoste(uint8_t temp)
{
	//更换到第1罐
	if(temp == 0)
	{
		Relay_Set(relay1,RELAY_ON);
		Relay_Set(relay2,RELAY_OFF);
		//Relay_Set(relay3,RELAY_OFF);
	}
	//更换到第2罐
	if(temp == 1)
	{
		Relay_Set(relay1,RELAY_OFF);
		Relay_Set(relay2,RELAY_ON);
		//Relay_Set(relay3,RELAY_OFF);
	}
}

void Relay_CloseALL()
{
	Relay_Set(relay1,RELAY_OFF);
	Relay_Set(relay2,RELAY_OFF);
	//Relay_Set(relay3,RELAY_OFF);
	Relay_Set(relay4,RELAY_OFF);
	Relay_Set(relay5,RELAY_OFF);
}
