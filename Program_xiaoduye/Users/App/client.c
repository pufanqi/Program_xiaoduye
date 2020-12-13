#include "client.h"

client_s client = {0};
uint16_t connect_flag=0;

void Client_Send_Origin(client_state_e state)
{
	uint8_t temp = 0;
	uint8_t *p;
	gprs_msg_TX_s msg;
	uint16_t check=0;
	
	client.volume_produce_last.volume=0;
	client.volume_produce_next.volume=0;
	
	msg.head = 0xFE;
	msg.length = 0x1D;
	msg.sequence = 0x00;
	msg.msg_id = 0x01;
	msg.equipment_id_1 = 0x01;
	msg.equipment_id_2 = 0x01;
	msg.equipment_id_3 = 0x00;
	msg.equipment_id_4 = 0x01;
	msg.LAC_H = 0x53;
	msg.LAC_L = 0x15;
	msg.CID_H = 0x76;
	msg.CID_L = 0x71;
	msg.money_H = 0x00;//client.money_last.money_B[2];
	msg.money_M = 0x00;//client.money_last.money_B[1];
	msg.money_L = 0x7B;//client.money_last.money_B[0];
	msg.produce_H = 0x00;//client.volume_produce.volume_B[1];
	msg.produce_L = 0x7B;//client.volume_produce.volume_B[0];
	msg.stoste_last_1 = 0x46;//material_last.stoste1;
	msg.stoste_last_2 = 0x64;//material_last.stoste2;
	msg.CO2_last_1 = 0x46;//material_last.CO2_1;
	msg.CO2_last_2 = 0x64;//material_last.CO2_2;
	msg.status = 0x02;//state;
	msg.fault_1 = 0x00;//client.fault_1;
	msg.fault_2 = 0x00;//client.fault_2;
	msg.fault_3 = 0x00;//client.fault_3;
	msg.fault_4 = 0x00;//client.fault_4;
	msg.fault_5 = 0x00;//client.fault_5;
	
	p = (uint8_t*)&msg;
	for(temp=0;temp<27;temp++)
	{
		check += *(p+temp);
	}
	msg.check_L = 0xBA;//check&0x00FF;
	msg.check_H = 0x04;//check&0xFF00;
	GPRS_Send(&msg);
}

//更新生产的体积
void Client_VolumeUpdata()
{
	client.volume_produce_next.volume += pulse_num/220;
	pulse_num = 0;
}



//结算金额
uint32_t Client_Settlemen()
{
	uint32_t money = 0;
	uint16_t unit_price = 0;
	
	unit_price = (GPRS_msg_RX.unit_cost_H<<8)+(GPRS_msg_RX.unit_cost_L);
	money = (client.volume_produce_next.volume-client.volume_produce_last.volume) * unit_price/10;//生产体积单位0.1升，价格0.01元/升
	client.volume_produce_last.volume=client.volume_produce_next.volume;
	return money;
}

//处理接受数据信息
void Client_Order()
{
	
	//判断是否允许工作
	if(GPRS_msg_RX.respond != 0)//请求是否成功
	{
		Production_Stop();
		production_state = STOP;
		return;
	}
	if(GPRS_msg_RX.isDisable==1)//是否禁用
	{
		Production_Stop();
		production_state = STOP;
		return;
	}
	if(GPRS_msg_RX.isMoneyRemain==1)//余额是否充足
	{
		Production_Stop();
		production_state = STOP;
		return;
	}
	
	
	client.unit_price.unit_B[0]=GPRS_msg_RX.unit_cost_L;
	client.unit_price.unit_B[1]=GPRS_msg_RX.unit_cost_H;
	client.money_last.money_B[0]=GPRS_msg_RX.money_L;
	client.money_last.money_B[1]=GPRS_msg_RX.money_M;
	client.money_last.money_B[2]=GPRS_msg_RX.money_H;
	client.money_last.money -= Client_Settlemen();
	
	limit_flag = 0 ;//清除限制生产标记
	
}

//定时发送客户数据
void Client_Send()
{
	static uint16_t repeat_count=0;
	uint8_t temp = 0;
	uint8_t *p;
	gprs_msg_TX_s msg;
	uint16_t check=0;
	
	
	if(response_flag==1)
	{
		response_flag=0;
		repeat_count=0;
	}
	
	msg.head = 0xFE;
	msg.length = 0x1D;
	msg.sequence = 0x00;
	msg.msg_id = 0x01;
	msg.equipment_id_1 = 0x01;
	msg.equipment_id_2 = 0x01;
	msg.equipment_id_3 = 0x00;
	msg.equipment_id_4 = 0x01;
	msg.LAC_H = 0x53;
	msg.LAC_L = 0x15;
	msg.CID_H = 0x76;
	msg.CID_L = 0x71;
	msg.money_H = client.money_last.money_B[2];
	msg.money_M = client.money_last.money_B[1];
	msg.money_L = client.money_last.money_B[0];
	msg.produce_H = client.volume_produce_next.volume_B[1];
	msg.produce_L = client.volume_produce_next.volume_B[0];
	msg.stoste_last_1 = material_last.stoste1;
	msg.stoste_last_2 = material_last.stoste2;
	msg.CO2_last_1 = material_last.CO2_1;
	msg.CO2_last_2 = material_last.CO2_2;
	msg.status = client.state;//state;
	msg.fault_1 = client.fault_1;
	msg.fault_2 = client.fault_2;
	msg.fault_3 = client.fault_3;
	msg.fault_4 = client.fault_4;
	msg.fault_5 = client.fault_5;
	
	p = (uint8_t*)&msg;
	for(temp=0;temp<27;temp++)
	{
		check += *(p+temp);
	}
	msg.check_L = check&0x00FF;
	msg.check_H = check&0xFF00;
	GPRS_Send(&msg);
	repeat_count++;
	//超过三次未响应代表连接失败，连接标志位置零
	if(repeat_count >= 3)
	{
		connect_flag=0;
		repeat_count=0;
		response_flag=1;//这里重置标志位，中止继续发送，恢复60min的定时发送
	}
}


