/*
*  c_data_events.h - NGC_RS274 controller.
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


#ifndef __C_COORDINATOR_ANCILLARY_EVENTS_H__
#define __C_COORDINATOR_ANCILLARY_EVENTS_H__

#include <stdint.h>
#include "../../../../c_ring_template.h"

class c_ancillary_event_handler
{
	//variables
	public:
	protected:
	private:


	//functions
	public:
	static void process();

	protected:
	private:
	static void __unkown_handler(c_ring_buffer <char> * buffer);
	static void __control_handler(c_ring_buffer <char> * buffer);
	static void __assign_handler(c_ring_buffer<char> *buffer);
	
}; //c_serial_events
#endif //__C_DATA_EVENTS_H__
