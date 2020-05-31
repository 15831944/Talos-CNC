#ifndef _C_SYS_TIMER_CONFIG_
#define _C_SYS_TIMER_CONFIG_

#include <stm32h7xx_hal.h>
#include <stm32_hal_legacy.h>

#define STEP_TIMER TIM2
#define STEP_TIMER_CLK_ENABLE __HAL_RCC_TIM2_CLK_ENABLE()
#define STEP_TIMER_INTERRUPT TIM2_IRQn

#define STEP_RST_TIMER TIM1
#define STEP_RST_TIMER_CLK_ENABLE __HAL_RCC_TIM1_CLK_ENABLE()
#define STEP_RST_TIMER_INTERRUPT TIM1_UP_IRQn

static void step_timer_config(void)
{

	/* Peripheral clock enable */
	STEP_TIMER_CLK_ENABLE;
	STEP_RST_TIMER_CLK_ENABLE;

	//clear everything we dont want
	STEP_TIMER->CR1 &= ~(TIM_CR1_CEN);
	STEP_TIMER->CR1 &= ~TIM_CR1_CKD;
	STEP_TIMER->SMCR &= ~TIM_SMCR_TS;
	STEP_TIMER->CCER &= ~TIM_CCER_CC1P;
	STEP_TIMER->CCMR1 &= ~TIM_CCMR1_OC1M;

	//clear everything we dont want
	STEP_RST_TIMER->CR1 &= ~(TIM_CR1_CEN);
	STEP_RST_TIMER->CR1 &= ~TIM_CR1_CKD;
	STEP_RST_TIMER->SMCR &= ~TIM_SMCR_TS;
	STEP_RST_TIMER->CCER &= ~TIM_CCER_CC1P;
	STEP_RST_TIMER->CCMR1 &= ~TIM_CCMR1_OC1M;

	STEP_TIMER->CR1 |= TIM_CLOCKDIVISION_DIV1;//<--clock divisor
	STEP_TIMER->PSC = 0; //<--prescaler
	STEP_TIMER->ARR = 1; //<--wait time
	STEP_TIMER->DIER |= (TIM_DIER_UIE);


	STEP_RST_TIMER->CR1 |= TIM_CLOCKDIVISION_DIV1;//<--clock divisor
	STEP_RST_TIMER->PSC = 0; //<--prescaler
	STEP_RST_TIMER->ARR = 0; //<--wait time
	STEP_RST_TIMER->DIER |= (TIM_DIER_UIE);

	//make timer 2 stop if I break 
	//DBGMCU->APB1LFZ1 |= DBGMCU_APB1LFZ1_DBG_TIM2;
}
#endif