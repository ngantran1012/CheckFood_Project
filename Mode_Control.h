#ifndef MODE_CONTROL_H
#define MODE_CONTROL_H


#include "stm32f10x.h"
#include "StepMotor_Control.h"
#if ENABLE_I2C_INT
#include "i2c_interface.h"
#endif
#include "rs485_interface.h"



#define TRANS_MODE_SW			GPIO_Pin_11 //GPIOB
#define ESTOP_SW					GPIO_Pin_10 //GPIOB
#define PULL_UP_BT				GPIO_Pin_1	//GPIOB
#define	PULL_DOWN_BT			GPIO_Pin_0	//GPIOB
#define Capture_Manual		GPIO_Pin_7  //GPIOA
#define Turn_off_Jetson		GPIO_Pin_4	//GPIOA
#define SENSOR_1					GPIO_Pin_6	//GPIOA
#define SENSOR_2					GPIO_Pin_5	//GPIOA
#define FAN_ON						GPIO_Pin_8  //GPIOA


void Manual_Mode(void);
void Auto_Mode(void);

#endif
