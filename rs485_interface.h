#include "stm32f10x.h"
#include "string.h"


/*Frame Transmit and Receive
|-----------|-------|---------------|
|Start Code	|Cmd ID	| Checksum (CRC)|	
|-----------|-------|---------------|
	 1 byte		 1 byte    1 byte

Start code: '$'

Cmd ID: 
***Jetson Nano ____ STM32F103C8T6***
__Move up signal: 0xA1 (Jetson Nano -> STM32F103C8T6)
__Capture Image signal: 0xC1 (STM32F103C8T6 -> Jetson Nano)
__Capture Image Signal: 0xC3 (STM32F103C8T6 -> Jetson Nano)
__Move down signal: 0xB1 (Jetson Nano -> STM32F103C8T6)
__Error system signal: 0x30 (STM32F103C8T6 -> Jetson Nano)
__Turn off Jetson Signal: 0xC0 (STM32F103C8T6 -> Jetson Nano)
__Moving up Status: 0xA2 (STM32F103C8T6 -> Jetson Nano)
__Moving down Status: 0xB2 (STM32F103C8T6 -> Jetson Nano)

*/

#define RX_BUF_SIZE 80
#define Start_Code	'$'
/*==========Defind Cmd_ID ==========*/
#define Move_Up_Signal								0xA1
#define Move_Down_Signal							0xB1
#define Capture_Image_Signal					0xC1
#define Capture_Image_Manual_Signal		0xC3
#define E_Sys_Signal									0x30
#define Off_Jetson_Signal							0xC0
#define Moving_UP_Status							0xA2
#define Moved_Down_Status							0xB2

extern volatile char RX_FLAG_END_LINE;
extern char RX_BUF[RX_BUF_SIZE];

extern int receiver_checksum, transmitter_checksum;
extern char FrameStart;
extern char cmID;


void USARTSendData(char *pucBuffer);
void clear_RXBuffer(void);
void usart_init(void);
uint8_t USARTGetData();
void USARTSendFrameData(uint8_t _Start_Code_T, uint8_t _Cmd_ID_T);
