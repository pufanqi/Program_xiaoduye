#include "test.h"

uint8_t test = 0;
uint16_t Periodval=10000;
uint16_t test_approach=0;
uint16_t test_id1=0;
uint16_t test_id2=0;
uint16_t pulse = 500;
uint8_t Stop_Flag = 0;
direction_t Dire_Flag = DIRE_PUSH;
gprs_msg_TX_s gprs_msg;

uint8_t STOP_DBG = 0;
uint16_t PERIOD_DGB = 9000;
void Test_stoste(uint16_t Period)
{
	Relay_Set(relay2,RELAY_ON);
	Relay_Set(relay7,RELAY_OFF);
	Motor_Setperiod(25);
	Motor_Enable(DIRE_PUSH);
	
	while(Approach_State() != 2)
	{
	
	}
	Motor_Disable();
	
	Relay_Set(relay7,RELAY_ON);
	Relay_Set(relay2,RELAY_OFF);
	Motor_Setperiod(PERIOD_DGB);
	Motor_Enable(DIRE_PULL);
	while(Approach_State() != 1)
	{
		if(STOP_DBG == 1)
		{
			break;
		}
	}
	Motor_Disable();
	
}
void TestModule(module_e m)
{
	switch(m)
	{
		case Relay:
		{
			Relay_Init();
		
	    while(1)
	    {
	    	if(test == 1)
	    	{
	    		GPIO_SetBits(RELAY1_PORT,RELAY6_PIN|RELAY5_PIN|RELAY4_PIN|RELAY3_PIN|RELAY2_PIN|RELAY1_PIN);
	    	}
	    	else
	    	{
	    		GPIO_ResetBits(RELAY1_PORT,RELAY6_PIN|RELAY5_PIN|RELAY4_PIN|RELAY3_PIN|RELAY2_PIN|RELAY1_PIN);
	    	}
	    }
		}
		case CO2_flux:
		{
			CO2Flux_Init();
	    while(1)
	    {
	    	
	    }
		}
		case stepping:
		{
      Motor_Init();
			Relay_Init();
			Approach_Init();
	    Motor_Enable(DIRE_PUSH);
	    
	    Water_Init();
			Motor_Enable(DIRE_PUSH);
			Motor_SetDirection(DIRE_PULL);
	    while(1)
	    {
				if(Stop_Flag == 1)
				{
					Motor_Disable();
				}
				else
				{					
					TIM_SetCompare2(TIM1,pulse);
	    	  Motor_Setperiod(Periodval);
					Motor_SetDirection(Dire_Flag);
					
				}
				if(test==0)
				{
					GPIO_ResetBits(RELAY1_PORT,RELAY1_PIN);
					GPIO_SetBits(RELAY1_PORT,RELAY2_PIN);
				}
				else
				{
					GPIO_ResetBits(RELAY1_PORT,RELAY2_PIN);
					GPIO_SetBits(RELAY1_PORT,RELAY1_PIN);
				}
				
	    }
		}
		case Approach_Switch:
		{
      Approach_Init();
	    while(1)
	    {
	    	test_approach=Approach_State();
	    }
		}
		case stoste:
		{
      Stoste_Init();
			while(1)
			{
				test_id1=Stoste_GetStatus(0);
				test_id2=Stoste_GetStatus(1);
			}
		}
		case gprs:
		{
			GPRS_Init();
			
			GPRS_Send(&gprs_msg);
			while(1)
			{
				
			}
		}			
	}
}
