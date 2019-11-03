/*
*  c_data_events.cpp - NGC_RS274 controller.
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

#include "c_serial_event_handler.h"
#include "../../Main/Main_Process.h"
#include "../extern_events_types.h"
#include "../../Data/DataHandlers/c_binary_data_handler.h"
#include "../../Data/DataHandlers/c_ngc_data_handler.h"
#include "../../../../communication_def.h"

void(*c_serial_event_handler::pntr_data_handler)(c_ring_buffer<char> * buffer);

//We know a serial data event has occurred but we dont have any details.
//This event processor will determine the type of data, and in whatway
//the data consumer needs to use it.
void c_serial_event_handler::process(c_ring_buffer<char> * buffer)
{
	/*
	When we receive the first bye of data, we determine which handler to use.
	Until all of that particular data type is received we will not change
	the handler. Ngc data will be handled until we encounter a CR or LF,
	binary data will handled until the specified length is reached,	and
	control data will be read until we determine what the control is supposed
	to be assigned to.
	*/
	
	//Is there a handler assigned for this data class already?
	if (c_serial_event_handler::pntr_data_handler == NULL)
	{
		c_serial_event_handler::__assign_handler(buffer);
	}
	//Once a handler has been assigned we should be able to process
	//the data pretty fast since there is no more switching or if
	//statements
	c_serial_event_handler::pntr_data_handler(buffer);
	
}

void c_serial_event_handler::__assign_handler(c_ring_buffer <char> * buffer)
{
	//Tail is always assumed to be at the 'start' of data
	
	char peek_tail = buffer->peek_tail();
	
	//Printable data is ngc line data. We need to check cr or lf because those are
	//special line ending characters for ngc data. We will NEVER use 10 or 13 as a
	//binary record type.
	if ((peek_tail >=32 && peek_tail <= 127) || (peek_tail == CR || peek_tail == LF)) 
	{
		//Assign a specific handler for this data type
		c_serial_event_handler::pntr_data_handler = c_ngc_data_handler::assign_handler(buffer);
		//Assign a release call back function. The handler knows nothing about serial events
		//and we want to keep it that way.
		c_ngc_data_handler::pntr_data_handler_release = c_serial_event_handler::data_handler_releaser;
		
	}
	else if (peek_tail >0 && peek_tail < 32) //non-printable and below 32 is a binary record
	{
		//Assign a specific handler for this data type
		c_serial_event_handler::pntr_data_handler = c_binary_data_handler::assign_handler(buffer);
		//Assign a release call back function. The handler knows nothing about serial events
		//and we want to keep it that way.
		c_binary_data_handler::pntr_data_handler_release = c_serial_event_handler::data_handler_releaser;
		
	}
	else if (peek_tail >127) //non-printable and above 127 is a control code
	{
		//this is control data, probably just a single byte
		c_serial_event_handler::pntr_data_handler = c_serial_event_handler::__control_handler;
		//c_ngc_data_handler::pntr_data_handler_release = c_serial_event_handler::data_handler_releaser;
	}
	else //we dont know what kind of data it is
	{
		c_serial_event_handler::pntr_data_handler = c_serial_event_handler::__unkown_handler;
		//c_ngc_data_handler::pntr_data_handler_release = c_serial_event_handler::data_handler_releaser;
	}
}

void c_serial_event_handler::__unkown_handler(c_ring_buffer <char> * buffer)
{
	//this was some unknown type of data.
	char peek_newest = buffer->peek_newest();
	Talos::Coordinator::Main_Process::host_serial.print_string("UKNOWN:");
	Talos::Coordinator::Main_Process::host_serial.print_int32(peek_newest);
	c_serial_event_handler::pntr_data_handler = NULL;
}

void c_serial_event_handler::__control_handler(c_ring_buffer <char> * buffer)
{
	//release the handler because we should be done with it now.
	c_serial_event_handler::pntr_data_handler = NULL;
	Talos::Coordinator::Main_Process::host_serial.print_string("control\r");
}

void c_serial_event_handler::data_handler_releaser()
{
	c_serial_event_handler::pntr_data_handler = NULL;
}

//// default constructor
//c_serial_event_handler::c_serial_event_handler()
//{
//}
//// default destructor
//c_serial_event_handler::~c_serial_event_handler()
//{
//}
