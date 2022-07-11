#include "tim2delay.h"

volatile uint8_t f_timer_2_end;

void TIM2_init(void)
{
	TIM_TimeBaseInitTypeDef TIMER_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructInit(&TIMER_InitStructure);
	
	TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIMER_InitStructure.TIM_Prescaler = 8;
	TIMER_InitStructure.TIM_Period = 1;
	TIM_TimeBaseInit(TIM2, &TIMER_InitStructure);

}

void TIM2_IRQHandler(void)
{
	extern volatile uint8_t f_timer_2_end;
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM2->SR &= ~TIM_SR_UIF;
	f_timer_2_end = 1;
	
	TIM_Cmd(TIM2, DISABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
}

void delay_us(uint32_t n_usec)
{
	f_timer_2_end = 0;
	TIM2->PSC = 100-1;
	TIM2->ARR = (uint16_t)(8 * n_usec);
	//TIM_Cmd(TIM6, ENABLE);
	
	TIM2->EGR |= TIM_EGR_UG;
	TIM2->SR	&= ~TIM_SR_UIF;
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	
	while(f_timer_2_end == 0);
}

void delay_ms(uint16_t n_msec)
{
	f_timer_2_end = 0;
	TIM2->PSC = 1000 - 1;
	TIM2->ARR = (uint16_t)(8 * n_msec);
	
	TIM2->EGR |= TIM_EGR_UG;
	TIM2->SR &= ~TIM_SR_UIF;
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	
	while(f_timer_2_end == 0);
	
}