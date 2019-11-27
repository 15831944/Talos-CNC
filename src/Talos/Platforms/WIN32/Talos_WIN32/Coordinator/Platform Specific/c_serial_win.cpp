/* 
* c_serial_avr_328.cpp
*
* Created: 2/27/2019 10:45:57 AM
* Author: jeff_d
*/


#include "c_serial_win.h"
#include "c_core_win.h"
#include <iostream>
#include "../../../../../c_ring_template.h"
#include "../../../../../Coordinator/Processing/Events/extern_events_types.h"
c_ring_buffer<char> Hardware_Abstraction_Layer::Serial::_usart0_buffer;
static char _usart0_data[256];


void Hardware_Abstraction_Layer::Serial::initialize(uint8_t Port, uint32_t BaudRate)
{
	_usart0_buffer.initialize(_usart0_data, 256);
}

void Hardware_Abstraction_Layer::Serial::send(uint8_t Port, char byte)
{
	std::cout << byte << std::flush;// rxBuffer[0].Buffer;
}

void Hardware_Abstraction_Layer::Serial::add_to_buffer(uint8_t port, const char * data)
{
	while (*data != 0)
	{
		Hardware_Abstraction_Layer::Serial::_usart0_buffer.put(*data);
		data++;
	}
	extern_data_events.event_manager.set((int)s_data_events::e_event_type::Usart0DataArrival);
	/*rxBuffer[port].Buffer[rxBuffer[port].Head++] = 13;
	rxBuffer[port].EOL++;*/
}
