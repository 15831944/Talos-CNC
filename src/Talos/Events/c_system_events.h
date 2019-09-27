
/*
*  c_system_events.h - NGC_RS274 controller.
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


#ifndef __C_SYSTEM_EVENTS_H__
#define __C_SYSTEM_EVENTS_H__

#include <stdint.h>
#include "..\records_def.h"
#include "..\Common\Serial\c_Serial.h"
namespace Events
{
	
	class System
	{
		//variables
		public:
		enum class e_event_type : uint8_t
		{
			Critical_Must_Shutdown = 0,
			Non_Critical_Hardware = 1,
			NGC_Error = 10
		};
		static BinaryRecords::s_bit_flag_controller_16 event_manager;
		static BinaryRecords::s_status_message events_statistics;
		static c_Serial *local_serial;

		protected:
		private:
		//static bool active;

		//functions
		public:
		static void check_events();

		protected:
		private:
	}; 
};
#endif //__C_SYSTEM_EVENTS_H__
