/*
* c_motion_core_win_spindle.h
*
* Created: 3/7/2019 10:22:11 AM
* Author: jeff_d
*/
#include <stdint.h>
#include "../../../../../talos_hardware_def.h"

#ifdef MSVC

#include <stdint.h>
#ifndef __C_MOTIONCORE_WIN_SPINDLE_H__
#define __C_MOTIONCORE_WIN_SPINDLE_H__

namespace Hardware_Abstraction_Layer
{
	namespace MotionCore
	{
		class Spindle
		{
			//typedef void(*function_pointer)(uint32_t parameter);
			
			//variables
			public:
			//static function_pointer Pntr_timer_function;
			//static s_encoders * spindle_encoder;
			protected:
			private:
			

			
			//functions
			public:
			/*static void initialize(s_encoders * encoder_data);
			static void configure_timer_for_at_speed_delay();
			static void stop_at_speed_timer();
			static void configure_timer_for_rpm_update(uint32_t interval);
			static void OCR1A_set(uint32_t delay);
			static int32_t get_rpm();*/
			protected:
			private:

			
			
			
		};
	};
};
#endif
#endif