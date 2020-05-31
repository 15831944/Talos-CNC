/*
* c_grbl_win_limits.cpp
*
* Created: 3/7/2019 10:22:11 AM
* Author: jeff_d
*/

//#include <avr/io.h>
//#include <avr/interrupt.h>
#include "c_stepper_stm32h745.h"
#include "c_core_stm32h745.h"
#include "../../../../Motion/NewCore/c_segment_to_hardware.h"
#include "sys_timer_configs.h"
#include "sys_gpio_configs.h"

namespace Hardware_Abstraction_Layer
{
	namespace MotionCore
	{
		
		void Stepper::initialize()
		{
			step_pulse_config();
			step_timer_config();
		}

		volatile static uint16_t on_time = 1000;
		__C void TIM2_IRQHandler(void)
		{

			if ((STEP_TIMER->SR & 0x0001) != 0)                  // check interrupt source
			{
				STEP_TIMER->SR &= ~(1 << 0);                          // clear UIF flag
				STEP_TIMER->CNT = 0;
				
				//prep the step reset timer values
				STEP_RST_TIMER->CNT = 0;
				STEP_RST_TIMER->ARR = on_time;
				
				Talos::Motion::Core::Output::Segment::pntr_driver();

				//uint32_t pending = __NVIC_GetPendingIRQ(STEP_RST_TIMER_INTERRUPT);
				//if (pending != 0)                  // check interrupt source
				//{
				//	//__NVIC_ClearPendingIRQ(STEP_RST_TIMER_INTERRUPT); 
				//	Stepper::step_port(0);
				//	int x = 0;
				//}
				
				//STEP_RST_TIMER->CNT = 8;
			}
		}

		__C void TIM1_UP_IRQHandler(void)
		{
			if ((STEP_RST_TIMER->SR & 0x0001) != 0)                  // check interrupt source
			{
				//NVIC_DisableIRQ(STEP_RST_TIMER_INTERRUPT);
				//STEP_RST_TIMER->CR1 &= ~TIM_CR1_CEN;
				STEP_RST_TIMER->ARR = 0;
				STEP_RST_TIMER->SR &= ~(1 << 0);                          // clear UIF flag
				
				Stepper::step_port(0);
			}
		}

		void Stepper::step_pul_high()
		{
			HAL_GPIO_WritePin(STEPPER_PUL_PORT, STEPPER_PUL_PIN_0, GPIO_PIN_SET);
		}

		void Stepper::step_port(uint16_t output)
		{
			STEPPER_PUL_PORT->ODR = (STEPPER_PUL_PORT->ODR & ~STEP_MASK) | output;
			
		}

		void Stepper::step_pul_low()
		{
			//set output pin low
			HAL_GPIO_WritePin(STEPPER_PUL_PORT, STEPPER_PUL_PIN_0, GPIO_PIN_RESET);
		}

		void Stepper::step_dir_high()
		{
			HAL_GPIO_WritePin(STEPPER_DIR_PORT, STEPPER_DIR_PIN_0, GPIO_PIN_SET);
		}

		void Stepper::step_dir_low()
		{
			//set output pin low
			HAL_GPIO_WritePin(STEPPER_DIR_PORT, STEPPER_DIR_PIN_0, GPIO_PIN_RESET);
		}

		void Stepper::wake_up()
		{
			STEP_TIMER->SR = 0;
			STEP_RST_TIMER->SR = 0;
			

			NVIC_ClearPendingIRQ(STEP_TIMER_INTERRUPT);
			__NVIC_SetPriority(STEP_TIMER_INTERRUPT, 1);
			NVIC_EnableIRQ(STEP_TIMER_INTERRUPT);      // Enable TIM2 Interrupt
			
			STEP_TIMER->CR1 |= TIM_CR1_CEN;   // Enable timer

			NVIC_ClearPendingIRQ(STEP_RST_TIMER_INTERRUPT);
			__NVIC_SetPriority(STEP_TIMER_INTERRUPT, 0);
			NVIC_EnableIRQ(STEP_RST_TIMER_INTERRUPT);
			STEP_RST_TIMER->CR1 |= TIM_CR1_CEN;
			
		}

		void Stepper::st_go_idle()
		{
			//disable interrupts for timer
			HAL_NVIC_DisableIRQ(STEP_TIMER_INTERRUPT);
			STEP_TIMER->CR1 &= ~TIM_CR1_CEN;
			STEP_TIMER->SR = 0;
			Stepper::step_pul_low();
		}

		void Stepper::set_delay(uint32_t delay)
		{
			STEP_TIMER->ARR = delay;
		}
	}
}