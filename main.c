#include "main.h"

uint8_t count=0, count1 = 0;
uint8_t data_read;
uint8_t cmp = 1;
uint8_t TX_BUFF[3] = {'$',0x1A,0xC1};

int main(void)
{

  SetSysClockToHSE();
	SET_GPIO();
	GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);
	
	TIM2_init();
	_NVIC_Init();
	#if ENABLE_I2C_INT
		I2C_Interface_Init();
	#endif
	#if ENABLE_USART_INT
		usart_init();
	#endif

	while(1)
	{
			
		if(GPIO_ReadInputDataBit(GPIOB,TRANS_MODE_SW) == 0)
		{		
			delay_ms(10);
			if(GPIO_ReadInputDataBit(GPIOB,TRANS_MODE_SW) == 0)
			{
				Manual_Mode();
			}

		}
		else if(GPIO_ReadInputDataBit(GPIOB,TRANS_MODE_SW) == 1)
		{
			delay_ms(10);
			if(GPIO_ReadInputDataBit(GPIOB,TRANS_MODE_SW) == 1)
			{
				Auto_Mode();				
			}
	
		}
		else
		{
			//Do nothing
		}
	}
	return 0;
}

void SetSysClockToHSE(void)
{
	ErrorStatus HSEStartUpStatus;
	
	RCC_DeInit();
	
	//Enable HSE
	RCC_HSEConfig(RCC_HSE_ON);
	
	//Waiting for HSE is ready
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	
	if(HSEStartUpStatus == SUCCESS)
	{
		//HCLK = SYSCLK
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		
		//PCLK2 = HCLK
		RCC_PCLK2Config(RCC_HCLK_Div1);
		
		//PCLK1 = HCLK
		RCC_PCLK1Config(RCC_HCLK_Div1);
		
		/* Select HSE as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
		
		//Waiting for PLL is used as system clock source
		while(RCC_GetSYSCLKSource() != 0x04)
		{
		}
	}
	else
	{
		while(1)
		{
			
		}
	}
	
}

void SET_GPIO(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//Config OUTPUT CONTROL STEP MOTOR
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//Config Output Fan
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	#if ENABLE_I2C_INT
	//CONFIG SCL_SDA PIN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	#endif
	
	#if ENABLE_USART_INT
	//CONFIG USART1
		/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	#endif
	
	//Config INPUT GPIO B
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	//Config INPUT GPIO A
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//Config INPUT GPIO A IPD FOR TWO SENSOR
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//Config Pin A3 OUTPUT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

}

void _NVIC_Init(void)
{
		//CONFIGURE NVIC FOR I2C
	NVIC_InitTypeDef NVIC_InitStructure;
	
	#if ENABLE_USART_INT
	/* NVIC Configuration */
	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	#endif
	#if ENABLE_I2C_INT
	NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel =I2C1_ER_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	#endif
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_SelectOnePulseMode(TIM2, TIM_OPMode_Single);
}