/***************************************************************************************/
//GPRS驱动程序
//型号：有人物联网  USR-GPRS232-7S3 
/***************************************************************************************/
#include "gprs.h"
gprs_msg_TX_s GPRS_msg_TX;
gprs_msg_RX_s GPRS_msg_RX;
gprs_msg_union GPRS_msg;
uint8_t GPRS_isRead = 1;

static void RCC_TIMConfig(USART_TypeDef *USARTx)
{
	if(USARTx == USART1){RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);}
	if(USARTx == USART2){RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);}
	if(USARTx == USART3){RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);}
}

void GPRS_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	//初始化GPIO和USART的总线时钟
	RCC_APB2PeriphClockCmd(GPRS_GPIO_CLK,ENABLE);
	RCC_TIMConfig(GPRS_USART);
	
	//利用初始化结构体初始化GPIO
	GPIO_InitStructure.GPIO_Pin = GPRS_TX_PIN; //TX引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPRS_TX_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPRS_RX_PIN;//RX引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPRS_RX_PORT,&GPIO_InitStructure);
	
	//利用初始化结构体初始化USART
	USART_InitStructure.USART_BaudRate = 115200;  //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;   //校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;    //usart模式
  USART_Init(GPRS_USART, &USART_InitStructure); //初始化
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(GPRS_USART,USART_IT_RXNE,ENABLE);
	USART_Cmd(GPRS_USART, ENABLE);   //使能USART
}

//发送GPRS信息
void GPRS_Send(gprs_msg_TX_s *msg)
{
	uint8_t size= sizeof(gprs_msg_TX_s);
	uint8_t *p = (uint8_t*)msg;
	uint8_t temp = 0;
	
	for(temp=0;temp<size;temp++)
	{
		USART_SendData(GPRS_USART,*p);
		while(USART_GetFlagStatus(GPRS_USART,USART_FLAG_TXE) == RESET);
		p++;
	}
}

//整理GPRS数据包
void GPRS_Order()
{
	uint8_t res;
	static uint8_t temp = 0;
	static uint8_t Reading_flag = 0;
	res = GPRS_USART->DR;
	if(res == 0xFE)
	{
		Reading_flag = 1; //开始接收
		GPRS_msg.RX_string[0] = res;
		return;
	}
	else if(Reading_flag == 1)
	{
		GPRS_msg.RX_string[1] = res;
		Reading_flag = 2;
	}
	else if(Reading_flag == 2)
	{
		GPRS_msg.RX_string[temp+2] = res;
		temp++;
		if(temp == GPRS_msg.msg_RX.length-2)
		{
			temp = 0;
			Reading_flag = 0;
			GPRS_msg_RX = GPRS_msg.msg_RX;
			GPRS_isRead = 0;  //设为未读
			return;
		}
		else
		{
			return;
		}
	}
}
