/*
*  c_gcode_buffer.h - NGC_RS274 controller.
*  A component of Talos
*
*  Copyright (c) 2016-2018 Jeff Dill
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

#ifndef __C_BUFFER_H__
#define __C_BUFFER_H__
#include "c_block.h"
#include "../Interpreter/c_interpreter.h"

class c_gcode_buffer
{
	//variables
	public:
	static c_block collection[NGC_BUFFER_SIZE];
	static uint8_t buffer_head;
	static uint8_t buffer_tail;

	static uint8_t queued_count;

	protected:
	private:

	//functions
	public:	
	//static int8_t get_previous_queued_index(uint8_t from);
	static c_block *get();
	static int8_t can_add();
	static int16_t add();
	static uint8_t count();
	static void drop();
	static uint8_t initialize();
	protected:
	private:
	//c_buffer( const c_buffer &c );
	//c_buffer& operator=( const c_buffer &c );
	//c_buffer();
	//~c_buffer();

}; //c_buffer

#endif //__C_BUFFER_H__
