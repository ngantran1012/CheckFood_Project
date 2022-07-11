#ifndef I2C_INTERFACE
#define	I2C_INTERFACE

/*
Frame Transmit and Receive between Master and Slave
***Master-Jetson Nano ____ Slave-STM32F103C8T6***
__Move up signal: 0xA1 (Master -> Slave)
__Capture Image signal: 0xC2 (Slave -> Master)
__Move down signal: 0xB1 (Master -> Slave)
__Error system signal: 0xE0 (Slave -> Master)
__Turn off Jetson Signal: 0xC0 (Slave -> Master)
__Moving up Status: 0xA2 (Slave -> Master)
__Moving down Status: 0xB2 (Slave -> Master)
*/

#include "stm32f10x.h"


#define ADC_ADDR_START	0x20
#define ADC_CHANNELS		8
#define I2C_SLAVE_ADDR 0x4E;

#define   I2C1_CLOCK_FRQ   100000   // I2C-Frq in Hz (100 kHz)
#define   I2C1_RAM_SIZE    0xFF     // RAM Size in Byte (1...255)
																		// First 8 bytes is a ADC Data,
																		//   next addresses You can use for
																		//   reading and writing.

#define I2C1_MODE_WAITING				0    // Waiting for commands
#define I2C1_MODE_SLAVE_ADR_WR	1	 	 // Received slave address (writing)
#define I2C1_MODE_ADR_BYTE			2    // Received ADR byte
#define I2C1_MODE_DATA_BYTE_WR	3    // Data byte (writing)
#define I2C1_MODE_SLAVE_ADR_RD	4 	 // Received slave address (to read)
#define I2C1_MODE_DATA_BYTE_RD	5    // Data byte (to read)

#if ENABLE_I2C_INT
#define Move_Up_Signal					0xA1
#define Move_Down_Signal				0xB1
#define Capture_Image_Signal		0xC1
#define E_Sys_Signal						0xE0
#define Off_Jetson_Signal				0xC0
#define Moving_UP_Status				0xA2
#define Moving_Down_Status			0xB2
#endif
void I2C_Interface_Init(void);
void I2C1_ClearFlag(void);
void i2c_start(void);
void i2c_stop(void);
void I2C_Transmit(uint8_t byte);
void I2C_SendAddress(uint8_t address, uint8_t read);
void I2C_Write(uint8_t address, uint8_t *data);
void I2C1_EV_IRQHandler(void);
void Resp_Data_Master(uint8_t data);
uint8_t Read_Data_From_Master(void);
void I2C1_ER_IRQHandler(void);
void test();




#endif
