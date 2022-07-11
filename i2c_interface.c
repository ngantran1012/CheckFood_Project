#include "i2c_interface.h"

unsigned char i2c1_mode = I2C1_MODE_WAITING;

volatile uint8_t Tx_Index, Rx_Index;

uint8_t Data_Jetson;

uint8_t I2C_Buffer_Rx[2], I2C_Buffer_Tx[2];
uint8_t I2C_Response[2];

uint16_t Data_Recieve = 0;
int tempin = 0;

void I2C_Interface_Init(void)
{
	I2C_InitTypeDef I2C_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	/* I2C Configuration */
	I2C_InitStructure.I2C_Mode = I2C_Mode_SMBusDevice;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDR;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	
	/* I2C Peripheral Enable */
	I2C_Cmd(I2C1, ENABLE);
	
	/* Apply I2C configuration after enabling it */
	I2C_Init(I2C1, &I2C_InitStructure);
	
	I2C_ITConfig(I2C1, I2C_IT_EVT, ENABLE);
	I2C_ITConfig(I2C1, I2C_IT_BUF, ENABLE);
	I2C_ITConfig(I2C1, I2C_IT_ERR ,ENABLE);
}


void I2C1_ClearFlag(void)
{
	while((I2C1->SR1 & I2C_SR1_ADDR) == I2C_SR1_ADDR){
		I2C1->SR1;
		I2C1->SR2;
	}
	while((I2C1->SR1&I2C_SR1_STOPF) == I2C_SR1_STOPF){
		I2C1->SR1;
		I2C1->CCR |= 0x1;
	}
}

/*====================================TRANSMIT FUNCTION FOR MASTER==================================================*/
void i2c_start(void)
{
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	//GENERATE START CONDITION
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)));
}

void i2c_stop(void)
{
	I2C_GenerateSTOP(I2C1, ENABLE);
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF));
}

void I2C_Transmit(uint8_t byte)
{
	MODIFY_REG(I2C1->DR, I2C_DR_DR, byte);
}

void I2C_SendAddress(uint8_t address, uint8_t read)
{
	address = address << 1;
	if(read != 0)
	{
		I2C_AcknowledgeConfig(I2C1, ENABLE);
	}else{
		I2C_AcknowledgeConfig(I2C1, DISABLE);
	}
	
	I2C_Transmit(address);
	while(!READ_BIT(I2C1->SR1, I2C_SR1_ADDR));
	while(READ_BIT(I2C1->SR2, I2C_SR2_TRA) != read);
}

void I2C_Write(uint8_t address, uint8_t *data)
{
	i2c_start();
	I2C_SendAddress(address, 0);
	for(uint8_t i = 0; i < 16; i ++)
	{
		while(!(READ_BIT(I2C1->SR1,I2C_SR1_TXE) || READ_BIT(I2C1->SR1, I2C_SR1_BTF)));
		I2C_Transmit(*data++);
	}
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	i2c_stop();
}

/*=================================SLAVE INTERRUPT RECIEVE=========================================*/

void I2C1_EV_IRQHandler(void)
{
	uint32_t event;
	void test();
	//Reading last event
	event = I2C_GetLastEvent(I2C1);
	
	switch(event)
	{
		//Slave Receiver
		case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:
			Rx_Index = 0;
			break;
		case I2C_EVENT_SLAVE_BYTE_RECEIVED:
			//reading data
			I2C_Buffer_Rx[Rx_Index++] = (uint8_t) READ_REG(I2C1->DR);
			break;
		case I2C_EVENT_SLAVE_STOP_DETECTED:
			//(void)I2C_GetITStatus(I2C1,I2C_IT_STOPF);
			
			I2C_Cmd(I2C1, ENABLE);
			//I2C1_ClearFlag();
			Rx_Index = 0;
		  break;
		//Slave Transmitter
		case I2C_EVENT_SLAVE_BYTE_TRANSMITTED:
			I2C_SendData(I2C1, I2C_Response[Tx_Index++]);
			Tx_Index = 0;
			tempin++;
			break;
		case I2C_EVENT_SLAVE_BYTE_TRANSMITTING:
			I2C_SendData(I2C1, I2C_Response[Tx_Index]);
			tempin++;
			break;
		  default:
			break;
	}


}


void I2C1_ER_IRQHandler(void) {
  if (I2C_GetITStatus(I2C1, I2C_IT_AF)) {
    I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
  }
}
/*==================================READ & SEND DATA==================================================*/
uint8_t Read_Data_From_Master(void)
{
//	Data_Recieve = I2C_Buffer_Rx[0];
//	Data_Recieve = Data_Recieve << 8;
//	Data_Recieve = Data_Recieve | I2C_Buffer_Rx[1];
	return Data_Jetson = I2C_Buffer_Rx[0];
	
}

void Resp_Data_Master(uint8_t data)
{
	I2C_Response[0] = data;
}


