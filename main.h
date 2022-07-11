#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_usart.h"
#include "stdio.h"
#include "misc.h"
#include "tim2delay.h"
#if ENABLE_I2C_INT
#include "i2c_interface.h"
#endif
#include "StepMotor_Control.h"
#include "Mode_Control.h"
#include "rs485_interface.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#define ENABLE_I2C_INT			0
#define ENABLE_USART_INT		1

void SET_GPIO(void);
void SetSysClockToHSE(void);
void _NVIC_Init(void);
