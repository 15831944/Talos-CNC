/*
*  c_encoder.h - NGC_RS274 controller.
*  A component of Talos
*
*  Copyright (c) 2016-2019 Jeff Dill
*
*  Talos is free software: you can redistribute it and/or modify
*  it under the terms of the GNU LPLPv3 License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Talos is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with Talos.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "..\Talos.h"

#ifndef __C_ENCODER_H__
#define __C_ENCODER_H__

namespace Spindle_Controller
{
	class c_encoder
	{
		//variables
		public:
		class hal_callbacks
		{
			public:
			static void timer_capture(uint16_t time_at_vector,uint8_t port_values);
			static void timer_overflow();
			static void position_change(uint16_t time_at_vector,uint8_t port_values);
		};
		
		static float encoder_rpm_multiplier;
		static float encoder_angle_multiplier;
		static uint16_t encoder_ticks_per_rev;
		static void initialize(uint16_t encoder_ticks_per_rev);
		protected:
		private:

		//functions
		public:
		static void update_time(uint16_t time_at_vector);
		static float current_rpm();
		static float current_angle_deg();

		protected:
		private:
		//c_encoder();
		//~c_encoder();
		//c_encoder( const c_encoder &c );
		//c_encoder& operator=( const c_encoder &c );

	}; //c_encoder
};
#endif //__C_ENCODER_H__
