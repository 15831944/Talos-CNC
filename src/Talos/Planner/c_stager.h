/*
*  c_stager.h - NGC_RS274 controller.
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


#ifndef __C_STAGER_H__
#define __C_STAGER_H__
#include <stdint.h>
#include "..\NGC_RS274\NGC_Block.h"
#include "..\Common\Serial\c_Serial.h"
#include "..\System\tool_table.h"
//namespace ns_planner
//{
class c_stager
{
	typedef struct
	{
		float X;
		float Y;
		float Z;
		float A;
		float B;
		float C;
		float U;
		float V;
	}s_coord_datum;
	//variables


public:
	//typedef struct s_persisted_values
	//{
		//float feed_rate;
		//uint16_t active_t;
		//uint8_t active_d;
		//uint8_t active_h;
		//uint16_t active_s;
		//uint8_t overrides_enabled;
	//};

	//static c_globals::s_tool_table tool_table[];
	//static s_persisted_values persisted_values;
	//static uint16_t stager_state_g_group[]; //There are 14 groups of gcodes (0-13)
	//static uint16_t stager_state_m_group[]; //There are 5 groups of mcodes (0-4)
	static c_Serial *local_serial;
	static float end_motion_position[];
	static float start_motion_position[];
	static uint32_t line_number;
	static s_coord_datum coordinate_datum[9];
	static NGC_RS274::NGC_Binary_Block*previous_block;
	static c_globals::s_tool_table tool_table[];

protected:
private:

	//functions
public:
	static void initialize();
	static int16_t stage_block_motion();
	static int16_t update_cutter_compensation(NGC_RS274::NGC_Binary_Block* local_block);
	static void update_non_modals(NGC_RS274::NGC_Binary_Block* block);
	static void parmeter_write(NGC_RS274::NGC_Binary_Block* local_block);
	static void update_coordinate_datum(uint16_t parameter_slot);
	static int16_t calculate_vector_distance(NGC_RS274::NGC_Binary_Block* plan_block);
	static void report();
	static uint8_t pre_stage_check();
	static uint8_t post_stage_check();
	static NGC_RS274::NGC_Binary_Block*get_added_block();
	//static c_Path current_path;
	//static c_Path forward_path;
protected:

private:



	//c_stager( const c_planner &c );
	//c_stager& operator=( const c_planner &c );
	//c_stager();
	//~c_stager();

}; //c_stager
//}
#endif //__C_STAGER_H__
