#ifndef STEPMOTOR_CONTROL_H
#define STEPMOTOR_CONTROL_H

#include "stm32f10x.h"
#include "tim2delay.h"

#define Enable_Step_Pin				GPIO_Pin_12 //GPIOB
#define Pulse_Step_Pin				GPIO_Pin_13 //GPIOB
#define Dir_Step_Pin					GPIO_Pin_14 //GPIOB

#define Run_Forward						0
#define Run_Backward					1

#define Max_Speed							5


void Enable_Step_Motor(void);
void Disable_Step_Motor(void);
void Set_Direction_Forward(void);
void Set_Direction_Backward(void);
void Run_Step_Motor(uint16_t speed);
void Run_Step_Motor_Position(uint16_t speed, uint8_t direction, int position);
	
#endif