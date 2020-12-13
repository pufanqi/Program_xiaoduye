/***************************************************************************************/
//水流量计驱动程序
//型号：GEMS公司FT-110系列
/***************************************************************************************/
#include "water_flux.h"

uint16_t freq = 0;
uint32_t pulse_num = 0;

void Water_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICStructure;
	NVIC_InitTypeDef NVIC_InitStruture;
	
	//GPIO初始化
	RCC_APB2PeriphClockCmd(WATER_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = WATER_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(WATER_PORT,&GPIO_InitStructure);
	
	//定时器初始化
	RCC_WATER_FUN(WATER_TIM_CLK,ENABLE);
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;
	TIM_TimeBaseInit(WATER_TIM,&TIM_TimeBaseStructure);
	
	TIM_ICStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;     //上升沿触发
	TIM_ICStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICStructure.TIM_ICFilter = 0;
  TIM_ICInit(WATER_TIM,&TIM_ICStructure);
	
	TIM_ClearFlag(WATER_TIM,TIM_FLAG_Update|TIM_IT_CC2);
	TIM_ITConfig(WATER_TIM,TIM_FLAG_Update|TIM_IT_CC2,ENABLE);
	
	TIM_Cmd(WATER_TIM,ENABLE);
	
	//中断嵌套配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStruture.NVIC_IRQChannel = WATER_TIM_IRQ;
	NVIC_InitStruture.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruture.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruture.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruture);
}

void WATER_TIM_IRQHANDLER()
{
	static uint8_t state = 0;
	static uint16_t timecount,timecount1 = 0;
	uint16_t temp = 0;
	
	if(TIM_GetITStatus(WATER_TIM,TIM_IT_CC2)!=RESET)
  {
     TIM_ClearITPendingBit(WATER_TIM, TIM_IT_CC2);    //清除中断标志位
     if(state==0)    //捕获第一个上升沿
     {
       state=1;
       timecount=TIM_GetCapture2(WATER_TIM);        //记录第一次上升沿的CNT值
     }
     else if(state==1)//捕获第二个上升沿
     {
       state=0;
       timecount1=TIM_GetCapture2(WATER_TIM);   //记录第二次上升沿的CNT值
       if(timecount<timecount1)
       {
         temp=timecount1-timecount;           //两次上升沿的差值
       }
       else if(timecount>timecount1)
       {
         temp=(0xffff-timecount)+timecount1;  //两次下降沿的差值
       }
       else
			 {
         temp=0;
			 }
       freq=1000000/temp;                     //脉冲的频率
     }
		 pulse_num++;
   }
}

//获取脉冲频率
uint16_t Water_GetFreq()
{
	return freq;
}
