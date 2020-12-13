/***************************************************************************************/
//���������������
//�ͺţ�2D45M
/***************************************************************************************/
#include "stepping_motor.h"

//���������ʼ��
void Motor_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_OCInitTypeDef TIM_InitOCStructure;
	
	//DR��MF��GPIO��ʼ��
	RCC_APB2PeriphClockCmd(DR_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = DR_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DR_PORT,&GPIO_InitStructure);
	GPIO_SetBits(DR_PORT,DR_PIN);   //��ʼ����ΪPUSH
	
	RCC_APB2PeriphClockCmd(MF_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = MF_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MF_PORT,&GPIO_InitStructure);
	GPIO_SetBits(MF_PORT,MF_PIN);  //��ʼʱ���Ʋ������ֹͣ��MF��Ч
	
	//���巢����ʼ��
	RCC_APB2PeriphClockCmd(PU_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = PU_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PU_PORT,&GPIO_InitStructure);
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
	
	RCC_PU_FUN(PU_TIM_CLK,ENABLE);
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Period = 65535;    //��ʼ����1ms
	TIM_InitStructure.TIM_Prescaler = 720-1; //1usһ��
	TIM_TimeBaseInit(PU_TIM,&TIM_InitStructure);
	
	
	TIM_InitOCStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�ߵ�ƽ��Ч
	TIM_InitOCStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_InitOCStructure.TIM_Pulse = 0;    //��ʼ����10��
	TIM_InitOCStructure.TIM_OutputState = TIM_OutputState_Enable;  //���ڿ�����
	TIM_OC2Init(PU_TIM,&TIM_InitOCStructure);
	
	TIM_ARRPreloadConfig(PU_TIM,ENABLE);  
	TIM_OC2PreloadConfig(PU_TIM,TIM_OCPreload_Enable);
	TIM_Cmd(PU_TIM,ENABLE);
}

//���÷���
void Motor_SetDirection(direction_t dire)
{
	if(dire == DIRE_PULL)
	{
		GPIO_ResetBits(DR_PORT,DR_PIN);
	}
	if(dire == DIRE_PUSH)
	{
		GPIO_SetBits(DR_PORT,DR_PIN);
	}
}

//ʹ�����ʼ��ת
//������Ҫ��ӳ�ʼ������ж�
void Motor_Enable(direction_t dire)
{
	if(dire == DIRE_PULL)
	{
		GPIO_ResetBits(DR_PORT,DR_PIN);
	}
	if(dire == DIRE_PUSH)
	{
		GPIO_SetBits(DR_PORT,DR_PIN);
	}
	GPIO_ResetBits(MF_PORT,MF_PIN);
}

//ֹͣ���
void Motor_Disable()
{
	TIM_SetCompare2(PU_TIM,0);
	GPIO_SetBits(MF_PORT,MF_PIN);
}

//�������ڣ�����ת�٣�
void Motor_Setperiod(uint16_t Period)
{
	
	TIM_SetCompare2(PU_TIM,1); //10
	if(Period>1&&Period<=65535)
	{
		TIM_SetAutoreload(PU_TIM,Period);
	}
}


