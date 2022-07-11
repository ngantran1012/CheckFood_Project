#include "StepMotor_Control.h"

void Enable_Step_Motor(void)
{
	GPIO_WriteBit(GPIOB, Enable_Step_Pin, 1);
}

void Disable_Step_Motor(void)
{
	GPIO_WriteBit(GPIOB, Enable_Step_Pin , Bit_RESET);
}

void Set_Direction_Forward(void)
{
	GPIO_WriteBit(GPIOB, Dir_Step_Pin, Bit_RESET);
}

void Set_Direction_Backward(void)
{
	//GPIO_WriteBit(GPIOB, Dir_Step_Pin, Bit_SET);
	GPIO_SetBits(GPIOB, Dir_Step_Pin);
}

void Run_Step_Motor(uint16_t speed)
{

	//PTO PULSE OUTPUT FOR DRIVER
	GPIO_WriteBit(GPIOB, Pulse_Step_Pin, Bit_SET);
	delay_us(speed);
	GPIO_WriteBit(GPIOB, Pulse_Step_Pin, Bit_RESET);
	delay_us(speed);
	
}

void Run_Step_Motor_Position(uint16_t speed, uint8_t direction, int position)
{
	uint8_t check_direction = 0;
	check_direction = direction;
	switch(check_direction)
	{
		case Run_Forward:
			Set_Direction_Forward();
		break;
		case Run_Backward:
			Set_Direction_Backward();
		break;
		default:
		break;
	}
	
	position = position * 200;
	
	for(int i = 0; i < position; i++)
	{
		Run_Step_Motor(speed);
	}
	
		
}