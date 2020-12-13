/***************************************************************************************/
//CO2流量计驱动程序
//型号：MEMS公司MF4000系列
/***************************************************************************************/
#include "CO2_flux.h"

//加权递推平均滤波相关参数
#define N 12     //递推个数
uint16_t weight[N] = {12,11,10,9,8,7,6,5,4,3,2,1}; //权值

//存储CO2流量计的全局变量
uint16_t CO2_data = 0;  

void CO2Flux_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	//GPIO初始化
	RCC_APB2PeriphClockCmd(CO2_CLK,ENABLE);  
	GPIO_InitStructure.GPIO_Pin = CO2_PIN;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CO2_PORT,&GPIO_InitStructure);
	
	//DMA初始化
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&CO2_data;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;   //DMA一个数据16位
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   //循环采集
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	//ADC初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //连续采样
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;  
	ADC_Init(ADC1, &ADC_InitStructure);    
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);

	ADC_RegularChannelConfig(ADC1,ADC_Channel_7,1,ADC_SampleTime_1Cycles5);
 
  ADC_DMACmd(ADC1,ENABLE);
 
	ADC_Cmd(ADC1,ENABLE);

	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	CO2_data = 0;
}

//一维信号滤波 （加权递推平均滤波）
uint16_t Filter(uint16_t NewValue)
{
	static uint16_t queue[N] = {0};
	uint8_t count = 0;
	uint32_t sum = 0;
	
	for(count = 1;count<N;count++)
	{
		queue[count] = queue[count-1];
	}
	queue[0] = NewValue;
	
	for(count = 0;count<N;count++)
	{
		sum += queue[count] * weight[count];
	}
	return sum/78;
}

//获得指定通道的采样值
uint16_t GetCO2Val()
{
	uint16_t NewVal = 0;
	
	NewVal = CO2_data;
	
	return Filter(NewVal);
}
