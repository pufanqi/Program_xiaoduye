/***************************************************************************************/
//ˮ��������������
//�ͺţ�GEMS��˾FT-110ϵ��
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
	
	//GPIO��ʼ��
	RCC_APB2PeriphClockCmd(WATER_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = WATER_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(WATER_PORT,&GPIO_InitStructure);
	
	//��ʱ����ʼ��
	RCC_WATER_FUN(WATER_TIM_CLK,ENABLE);
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;
	TIM_TimeBaseInit(WATER_TIM,&TIM_TimeBaseStructure);
	
	TIM_ICStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;     //�����ش���
	TIM_ICStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICStructure.TIM_ICFilter = 0;
  TIM_ICInit(WATER_TIM,&TIM_ICStructure);
	
	TIM_ClearFlag(WATER_TIM,TIM_FLAG_Update|TIM_IT_CC2);
	TIM_ITConfig(WATER_TIM,TIM_FLAG_Update|TIM_IT_CC2,ENABLE);
	
	TIM_Cmd(WATER_TIM,ENABLE);
	
	//�ж�Ƕ������
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
     TIM_ClearITPendingBit(WATER_TIM, TIM_IT_CC2);    //����жϱ�־λ
     if(state==0)    //�����һ��������
     {
       state=1;
       timecount=TIM_GetCapture2(WATER_TIM);        //��¼��һ�������ص�CNTֵ
     }
     else if(state==1)//����ڶ���������
     {
       state=0;
       timecount1=TIM_GetCapture2(WATER_TIM);   //��¼�ڶ��������ص�CNTֵ
       if(timecount<timecount1)
       {
         temp=timecount1-timecount;           //���������صĲ�ֵ
       }
       else if(timecount>timecount1)
       {
         temp=(0xffff-timecount)+timecount1;  //�����½��صĲ�ֵ
       }
       else
			 {
         temp=0;
			 }
       freq=1000000/temp;                     //�����Ƶ��
     }
		 pulse_num++;
   }
}

//��ȡ����Ƶ��
uint16_t Water_GetFreq()
{
	return freq;
}
