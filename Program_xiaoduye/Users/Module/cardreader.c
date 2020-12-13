#include "cardreader.h"

ID_data_t id_data;
uint8_t Card_flag_get = 0;  //0��ʾû�н��յ��µ�ˢ����Ϣ��1��ʾ���µ�ˢ����Ϣ

static void RCC_USARTConfig(USART_TypeDef *USARTx)
{
	if(USARTx == USART1){RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);}
	if(USARTx == USART2){RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);}
	if(USARTx == USART3){RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);}
}

void Card_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	//��ʼ��GPIO��USART������ʱ��
	RCC_APB2PeriphClockCmd(CARDREADER_GPIO_CLK,ENABLE);
	RCC_USARTConfig(CARDREADER_USART);
	
	//���ó�ʼ���ṹ���ʼ��GPIO
	GPIO_InitStructure.GPIO_Pin = CARDREADER_TX_PIN; //TX����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CARDREADER_TX_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CARDREADER_RX_PIN;//RX����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CARDREADER_RX_PORT,&GPIO_InitStructure);
	
	//���ó�ʼ���ṹ���ʼ��USART
	USART_InitStructure.USART_BaudRate = 9600;  //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;   //У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx;    //usartģʽ
  USART_Init(CARDREADER_USART, &USART_InitStructure); //��ʼ��
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(CARDREADER_USART,USART_IT_RXNE,ENABLE);
	USART_Cmd(CARDREADER_USART,ENABLE);
	
	id_data.isreading = 1;
	id_data.flag = 0;
	id_data.type = 0;
}
	
void Card_Order()
{
	uint8_t res;
	static uint8_t temp = 0;
	  res = CARDREADER_USART->DR;
		if(res == 0x02)
		{
			id_data.flag = 1;
			return;
		}
		else if(id_data.flag == 1)
		{
			id_data.length = res;
			id_data.flag = 2;return;
		}
		else if(id_data.flag ==2)
		{
			id_data.type = res;
			id_data.flag = 3;return;
		}
		else if(id_data.flag ==3)
		{
			id_data.ID[temp] = res;
			temp++;
			if(temp == id_data.length-5)
			{
				temp = 0;
				id_data.check = res;
				id_data.flag = 4;return;
			}
			else
			{
				return;
			}
		}
		else if(res == 0x03)
		{
			id_data.flag = 0;
			id_data.isreading = 0;
		}
}


//�ض���C�⣬�����ӳ�䵽�����ϣ����ڵ���ʱ��ӡ���
/*****************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	USART_SendData(DEBUG_USARTx, (uint8_t) ch); 
	while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET); 
	return (ch); 
}
int fgetc(FILE *f) 
{
	 while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET); 
	 return (int)USART_ReceiveData(DEBUG_USARTx); 
}
/*****************************************************************************************************************/

uint8_t ID_Admin[10][5]=
{
	{0x97,0x0D,0xCF,0x9A,0x00},
};
uint8_t ID_User[10][5]=
{
	{0x5C,0xF0,0X8F,0X46,0X00},
	{0X2E,0X43,0X3F,0X8C,0X00},
};

void Card_SaveID(uint8_t length,uint8_t *ID)
{
	static uint8_t array_p = 0;
	uint8_t *p = ID;
	uint8_t i = 0;
	
	array_p++;
	if(array_p >=10)
	{
		array_p = 0;
	}
	
	for(i=0;i<length;i++)
	{
		ID_Admin[i][array_p] = *(p+i);
	}
	
}

//��ѯ��ǰID�Ƿ�����洢��IDƥ��
static uint8_t Card_MatchID(uint8_t *ID_Get,uint8_t *ID_Save)
{
	uint8_t i = 0,j=0;
	uint8_t flag = 1;
	for(i=0;i<10;i++)
	{
		for(j=0;j<5;j++)
		{
			if(*(ID_Save+i*5+j) == *(ID_Get+j))
			{
				flag = 1;
				continue;
			}
			else
			{
				flag = 0;
				break;
			}
		}
		
		if(flag == 1)
		{
			return 1;  //����1��ƥ�䵽ID
		}
		else
		{
			continue;
		}
	}
	return 0;      //����0��û��ƥ�䵽ID
}

//��ȡ��������
uint8_t Card_GetType()
{
	if(Card_MatchID(id_data.ID,(uint8_t*)ID_Admin))
	{
		return 1;
	}
	else if(Card_MatchID(id_data.ID,(uint8_t*)ID_User))
	{
		return 2;
	}
	else
	{
		return 0;
	}
}
