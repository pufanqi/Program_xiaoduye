/***************************************************************************************/
//屏幕通信驱动程序
//型号：
/***************************************************************************************/
#include "screen.h"

screen_TX_s message_TX;
screen_RX_s message_RX;

static void RCC_TIMConfig(USART_TypeDef *USARTx)
{
	if(USARTx == USART1){RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);}
	if(USARTx == USART2){RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);}
	if(USARTx == USART3){RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);}
	if(USARTx == UART4){RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);}
}

void Screen_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//初始化GPIO和USART的总线时钟
	RCC_APB2PeriphClockCmd(SCREEN_GPIO_CLK,ENABLE);
	RCC_TIMConfig(SCREEN_USART);
	
	//利用初始化结构体初始化GPIO
	GPIO_InitStructure.GPIO_Pin = SCREEN_TX_PIN; //TX引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SCREEN_TX_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SCREEN_RX_PIN;//RX引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SCREEN_RX_PORT,&GPIO_InitStructure);
	
	//利用初始化结构体初始化USART
	USART_InitStructure.USART_BaudRate = 115200;  //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;   //校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;    //usart模式
  USART_Init(SCREEN_USART, &USART_InitStructure); //初始化
	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(SCREEN_USART,USART_IT_RXNE,ENABLE);
	USART_Cmd(SCREEN_USART,ENABLE);
	
	message_TX.state = 0;
	message_TX.CO2_1 = 0;
	message_TX.CO2_2 = 0;
	message_TX.stoste1 = 0;
	message_TX.stoste2 = 0;
	message_RX.command = 0xFF;
	message_RX.data = 0;
	message_RX.flag = 0;
	message_RX.isRead = 1;
}

//在中断函数中调用
void Screen_Order()
{
	uint8_t res;
	  res = SCREEN_USART->DR;
		if(message_RX.flag == 0 && res == 0xFE)
		{
			message_RX.flag = 1;return;
		}
		else if(message_RX.flag == 1 && res == 0xEF)
		{
			message_RX.flag = 2;return;
		}
		else if(message_RX.flag ==2)
		{
			message_RX.command = res;
			message_RX.flag = 3;return;
		}
		else if(message_RX.flag ==3)
		{
			message_RX.data = res;
			message_RX.flag = 0;
			message_RX.isRead = 0;return;
		}
}

void Screen_Getdata(screen_RX_s *msg)
{
	msg->command = message_RX.command;
	msg->data = message_RX.data;
	msg->isRead = message_RX.isRead;
	
	if(message_RX.isRead == 0)
	{
		message_RX.isRead = 1;
	  return;
	}
	else
	{
		return;
	}
}

void Screen_Senddata(screen_TX_s *msg)
{
	uint8_t size= sizeof(screen_TX_s);
	uint8_t *p = (uint8_t*)&message_TX;
	uint8_t temp = 0;
	
	message_TX.front1 = 0xFE;
	message_TX.front2 = 0xEF;
	message_TX.CO2_1 = msg->CO2_1;
	message_TX.CO2_2 = msg->CO2_2;
	message_TX.stoste1 = msg->stoste1;
	message_TX.stoste2 = msg->stoste2;
	message_TX.state = msg->state;
	message_TX.ID_Type = msg->ID_Type;
	for(temp=0;temp<5;temp++)
	{
		message_TX.ID[temp] = msg->ID[temp];
	}
	message_TX.money_H = msg->money_H;
	message_TX.money_M = msg->money_M;
	message_TX.money_L = msg->money_L;
	
	for(temp=0;temp<size;temp++)
	{
		USART_SendData(SCREEN_USART,*p);
		while(USART_GetFlagStatus(SCREEN_USART,USART_FLAG_TXE) == RESET);
		p++;
	}
}
