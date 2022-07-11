#include "Mode_Control.h"


uint8_t save_status = 0, data_receive;
int count_up = 0, count_down = 0, Max_Pos = 40000, count_time = 0, move_down_er_signal = 0;
uint8_t temp = 0, temp_s1 = 0, temp_s2 = 0, temp_est = 0;



void Manual_Mode(void)
{
	
	if(GPIO_ReadInputDataBit(GPIOB,PULL_UP_BT) == 0)
	{
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOB,PULL_UP_BT) == 0)
		{
			GPIO_WriteBit(GPIOA,FAN_ON,Bit_RESET);
			Enable_Step_Motor();
			Set_Direction_Forward();
			while(GPIO_ReadInputDataBit(GPIOB,PULL_UP_BT) == 0)
			{
				Run_Step_Motor(Max_Speed);
			}
		}	
	}
	else if(GPIO_ReadInputDataBit(GPIOB,PULL_DOWN_BT) == 0)
	{
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOB,PULL_DOWN_BT) == 0)
		{
			GPIO_WriteBit(GPIOA,FAN_ON,Bit_RESET);
			Enable_Step_Motor();
			Set_Direction_Backward();
			while(GPIO_ReadInputDataBit(GPIOB,PULL_DOWN_BT) == 0)
			{
				Run_Step_Motor(Max_Speed);
			}		
		}
	}
	else if(GPIO_ReadInputDataBit(GPIOB,ESTOP_SW) == 0)
	{
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOB,ESTOP_SW) == 0)
		{
			Disable_Step_Motor();
		}
	}
	else if(GPIO_ReadInputDataBit(GPIOA,Capture_Manual) == 0)
	{
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOA,Capture_Manual) == 0)
		{
			USARTSendFrameData(Start_Code,Capture_Image_Manual_Signal);
			delay_ms(100);
			USARTSendFrameData(Start_Code,0x33);
		
		}
			
	}
	else if(GPIO_ReadInputDataBit(GPIOA,Turn_off_Jetson)==0)
	{
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOA,Turn_off_Jetson)==0)
		{
			USARTSendFrameData(Start_Code,Off_Jetson_Signal);
			delay_ms(100);
			USARTSendFrameData(Start_Code,0x33);
		}
	}
	else
	{
		Disable_Step_Motor();
		GPIO_WriteBit(GPIOA,FAN_ON,Bit_SET);
	}
	
}

void Auto_Mode(void)
{	
	
	if(save_status == 0) Disable_Step_Motor();
  data_receive = USARTGetData();
	if(data_receive == Move_Up_Signal)
	{	
		if(save_status == 0)
		{

			save_status++;
			Enable_Step_Motor();
			Set_Direction_Forward();
			GPIO_WriteBit(GPIOA,FAN_ON,Bit_RESET);
			USARTSendFrameData(Start_Code,Moving_UP_Status);
			while(1)
			{
				Run_Step_Motor(Max_Speed);
				if(GPIO_ReadInputDataBit(GPIOA,SENSOR_1) == 1)
				{
					temp_s1++;
				}
				if(GPIO_ReadInputDataBit(GPIOB,ESTOP_SW) == 0)
				{
					temp_est++;
				}
				count_up++;				
				if((count_up > Max_Pos) ||(temp_s1 == 50) || (temp_est == 50)) break;				
			}
			
			//GPIO_WriteBit(GPIOB,Enable_Step_Pin,0);
			if(count_up < Max_Pos)
			{
				temp++;
				Run_Step_Motor_Position(Max_Speed,Run_Forward,20); /*Add 2 Round*/
				
			}
			else if(count_up >= Max_Pos)
			{
				USARTSendFrameData(Start_Code,E_Sys_Signal);
				delay_ms(1500);
				temp++;
			}
			else
			{
				//Do Nothing
			}
			
			
			if(temp != 0)
			{
				USARTSendFrameData(Start_Code,Capture_Image_Signal);
				//Resp_Data_Master(Capture_Image_Signal);
			}
		}
	}
	data_receive = USARTGetData();
	if((data_receive == Move_Down_Signal) || (move_down_er_signal))
	{
		if(save_status == 1)
		{
			Set_Direction_Backward();
			USARTSendFrameData(Start_Code,Moved_Down_Status);
			while(count_down < count_up)
			{
				Run_Step_Motor(Max_Speed);
				count_down++;
			}
			if(count_up < Max_Pos)
			{
				Run_Step_Motor_Position(Max_Speed,Run_Backward,20);
			}
			
			save_status = 0;
			Disable_Step_Motor();
			count_down = 0;
			count_up =0;
			count_time = 0;
			move_down_er_signal = 0;
			temp = 0;
			temp_s1 = 0;
			temp_est = 0;
			GPIO_WriteBit(GPIOA,FAN_ON,Bit_SET);
			data_receive = 0;
			
		}
	}

	
	if(save_status == 1)
	{
		delay_us(10);
		count_time++;
		if(count_time > 6000) move_down_er_signal = 1;
	}
		
	
}