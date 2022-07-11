#include "rs485_interface.h"


volatile char RX_FLAG_END_LINE = 0;
volatile char RXi;
volatile char RXc;
int receiver_checksum = 0;
int transmitter_checksum = 0;
int kt = 0;
//volatile char RX_BUF[RX_BUF_SIZE] = {'\0'};
char RX_BUF[RX_BUF_SIZE] = {'\0'};
char TX_BUF[3] = {'\0'};
int temp1 = 0;
char cmID = 0;

void clear_RXBuffer(void) {
	for (RXi=0; RXi<RX_BUF_SIZE; RXi++)
		RX_BUF[RXi] = '\0';
	RXi = 0;
}

void clear_TXBuffer(void)
{
	for(size_t TXi = 0; TXi < 3; TXi++)
	{
		TX_BUF[TXi] = '\0';
	}
	USARTSendData(TX_BUF);
}

void usart_init(void)
{
	/* Enable USART1 and GPIOA clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	/* Configure the USART1 */
	USART_InitTypeDef USART_InitStructure;

	/* USART1 configuration ------------------------------------------------------*/
	/* USART1 configured as follow:
		- BaudRate = 115200 baud
		- Word Length = 8 Bits
		- One Stop Bit
		- No parity
		- Hardware flow control disabled (RTS and CTS signals)
		- Receive and transmit enabled
		- USART Clock disabled
		- USART CPOL: Clock is active low
		- USART CPHA: Data is captured on the middle
		- USART LastBit: The clock pulse of the last data bit is not output to
			the SCLK pin
	 */
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);

	/* Enable USART1 */
	USART_Cmd(USART1, ENABLE);

	/* Enable the USART1 Receive interrupt: this interrupt is generated when the
		USART1 receive data register is not empty */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void USART1_IRQHandler(void)
{
  if ((USART1->SR & USART_FLAG_RXNE) != (u16)RESET)
	{
    	RXc = USART_ReceiveData(USART1);
    	RX_BUF[RXi] = RXc;
    	RXi++;
			if(RXi > RX_BUF_SIZE-1) clear_RXBuffer();
			RX_FLAG_END_LINE = 1;	
			
	}
	   
}

void USARTSendData(char *pucBuffer)
{
    while (*pucBuffer)
    {
      USART_SendData(USART1, *pucBuffer++);		
      while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
      {
      }
    }
}

uint8_t USARTGetData()
{	
	uint8_t _Cmd_ID_R = 0, _Start_Code_R;
	_Start_Code_R = RX_BUF[RXi - 3];
	receiver_checksum = (RX_BUF[RXi - 3] + RX_BUF[RXi - 2] + RX_BUF[RXi -1]);
	if(receiver_checksum == 0xFF)
	{
		if(_Start_Code_R == Start_Code)
		{
			_Cmd_ID_R = RX_BUF[RXi - 2];
			receiver_checksum = 0;
		}
	}
	else
	{
		//Do nothing
	}

	return _Cmd_ID_R;	
	
}

void USARTSendFrameData(uint8_t _Start_Code_T, uint8_t _Cmd_ID_T)
{
	   
	TX_BUF[0] = _Start_Code_T;
	TX_BUF[1] = _Cmd_ID_T;
	transmitter_checksum = ~(TX_BUF[0] + TX_BUF[1]);
	TX_BUF[2] = transmitter_checksum;
	USARTSendData(TX_BUF);
	clear_TXBuffer();
}



