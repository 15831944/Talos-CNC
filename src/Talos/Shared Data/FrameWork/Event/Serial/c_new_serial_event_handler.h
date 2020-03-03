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


#ifndef __C_NEW_SHARED_SERIAL_DATA_EVENTS_H__
#define __C_NEW_SHARED_SERIAL_DATA_EVENTS_H__

#include <stdint.h>
#include "../../../../c_ring_template.h"
#include "../../../FrameWork/extern_events_types.h"
#include "../../../_s_framework_error.h"
#include "../../../FrameWork/event/c_event_router.h"

typedef void(*write_pointer)(uint8_t port, char byte);
class c_new_serial_event_handler
{
	//variables
public:
	static void(*pntr_data_read_handler)(c_ring_buffer<char> * buffer);
	static void(*pntr_data_write_handler)(char **buffer, uint8_t(*pntr_hw_write)(uint8_t port, char byte));
	

protected:
private:


	//functions
public:
	static void process(char **buffer, c_event_router::ss_outbound_data * event_object, c_event_router::ss_outbound_data::e_event_type event_id);
	static void process(c_ring_buffer<char> * buffer, c_event_router::ss_inbound_data * event_object, c_event_router::ss_inbound_data::e_event_type event_id);
	
	static void read_data_handler_releaser(c_ring_buffer<char> * has_data);
	static void write_data_handler_releaser(c_ring_buffer<char> * has_data);

protected:
private:
	static void __raise_error(c_ring_buffer <char> * buffer_source, e_error_behavior e_behavior
		, uint8_t data_size, e_error_group e_group, e_error_process e_process, e_record_types e_rec_type
		, e_error_source e_source, e_error_code e_code);

	static void __assign_handler(c_ring_buffer<char> *buffer, c_event_router::ss_inbound_data * event_object, c_event_router::ss_inbound_data::e_event_type event_id);
	static void __assign_handler(char * buffer, c_event_router::ss_outbound_data * event_object, c_event_router::ss_outbound_data::e_event_type event_id);

}; //c_serial_events
#endif //__C_DATA_EVENTS_H__
