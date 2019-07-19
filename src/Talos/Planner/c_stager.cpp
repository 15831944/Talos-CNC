/*
*  c_stager.cpp - NGC_RS274 controller.
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


#include "c_stager.h"
#include "c_gcode_buffer.h"
#include "..\communication_def.h"
#include "..\NGC_RS274\NGC_G_Groups.h"
#include "..\NGC_RS274\ngc_defines.h"
#include "..\NGC_RS274\NGC_Parameters.h"
#include "..\NGC_RS274\NGC_Errors.h"
#include "..\Events\c_block_events.h"
#include "..\Events\c_motion_events.h"
#include "..\Events\c_data_events.h"
#include "CRC\c_cutter_comp.h"
#include "Canned Cycles\c_canned_cycle.h"
#include "c_machine.h"
#include "Stager_Errors.h"

//uint16_t c_stager::stager_state_g_group[COUNT_OF_G_CODE_GROUPS_ARRAY]; //There are 14 groups of gcodes (0-13)
//uint16_t c_stager::stager_state_m_group[COUNT_OF_M_CODE_GROUPS_ARRAY]; //There are 5 groups of mcodes (0-4)
//c_globals::s_tool_table c_stager::tool_table[COUNT_OF_TOOL_TABLE];

//c_stager::s_persisted_values c_stager::persisted_values;
uint32_t c_stager::line_number;
c_stager::s_coord_datum c_stager::coordinate_datum[9];
NGC_RS274::NGC_Binary_Block*c_stager::previous_block;
c_Serial *c_stager::local_serial;
c_globals::s_tool_table c_stager::tool_table[COUNT_OF_TOOL_TABLE];

/*
Where were the axis's located before we executed this block?
pre_motion_position will always contain the location the planner was at before the block executed.
Obviously the code block that we are processing will contain the new position we are going to.
*/
float c_stager::end_motion_position[MACHINE_AXIS_COUNT];
float c_stager::start_motion_position[MACHINE_AXIS_COUNT];


void c_stager::initialize()
{


	c_gcode_buffer::buffer_head = 0;
	c_gcode_buffer::buffer_tail = 0;

	//initialize the first g/m code block to match the machine modal values. These are the defaults to start with.
	//memcpy(c_stager::stager_state_g_group, c_machine::machine_state_g_group, COUNT_OF_G_CODE_GROUPS_ARRAY * sizeof(uint16_t));
	//memcpy(c_stager::stager_state_m_group, c_machine::machine_state_m_group, COUNT_OF_M_CODE_GROUPS_ARRAY * sizeof(uint16_t));
	memcpy(c_gcode_buffer::collection[c_gcode_buffer::buffer_head].g_group, c_machine::machine_state_g_group, COUNT_OF_G_CODE_GROUPS_ARRAY * sizeof(uint16_t));
	memcpy(c_gcode_buffer::collection[c_gcode_buffer::buffer_head].m_group, c_machine::machine_state_m_group, COUNT_OF_M_CODE_GROUPS_ARRAY * sizeof(uint16_t));
	//previous block is the same as the current block at start up.
	c_stager::previous_block = &c_gcode_buffer::collection[0];
}

void c_stager::report()
{
	////We peeked into serial and found a ?, thats how we got here.
	////But maybe its a more detailed inquiry, peek again to see if
	////its a detail query
	//uint8_t peek_value = c_stager::local_serial->Peek();
	//switch (peek_value)
	//{
		////case 'G'://<--request is for a detail of all gcode states.
		////{
		////c_stager::local_serial->Write("Sta G St:");
		////c_status::modal_codes(c_stager::stager_state_g_group, COUNT_OF_G_CODE_GROUPS_ARRAY);
		////c_stager::local_serial->Write(CR);
		////}
		////break;
		////case 'M'://<--request is for a detail of all mcode states.
		////{
		////c_stager::local_serial->Write("Sta M St:");
		////c_status::modal_codes(c_stager::stager_state_m_group, COUNT_OF_M_CODE_GROUPS_ARRAY);
		////c_stager::local_serial->Write(CR);
		////}
	//default://<--request is for single word value
	//{
		//if (peek_value >= 'A' && peek_value <= 'Z')
		//{
			//c_stager::local_serial->Write(peek_value);
			//c_stager::local_serial->Write('=');
			//c_stager::local_serial->print_float(c_stager::get_added_block()->get_value((char)peek_value));
			//c_stager::local_serial->Write(CR);
			//return;
		//}
	//}
	//break;
//
	//}
//
	//for (uint8_t i = 0; i < NGC_BUFFER_SIZE; i++)
	//{
		//c_stager::local_serial->print_string("Blk #"); c_stager::local_serial->print_int32(i); c_stager::local_serial->print_string(":");
		//c_stager::local_serial->Write(CR); c_stager::local_serial->Write("\t");
//
		//for (uint8_t axis_id = 0; axis_id < MACHINE_AXIS_COUNT; axis_id++)
		//{
			////print the axis name X,Y,Z,A,B,C,U,V
			////c_stager::local_serial->Write("Axis ");
			//c_stager::local_serial->print_int32(c_machine::machine_axis_names[axis_id]);
			////0=no motion,1=positive motion,-1=negative motion
			//c_stager::local_serial->Write(" Dir:"); c_stager::local_serial->Write_ni(c_gcode_buffer::collection[i].motion_direction[axis_id]);
			//if (axis_id + 1 < MACHINE_AXIS_COUNT)
				//c_stager::local_serial->Write(", ");
			//else
				//c_stager::local_serial->Write(CR);
		//}
//
		//NGC_RS274::Interpreter::Processor::convert_to_line_index(i);
		//c_stager::local_serial->print_string("\tString "); c_stager::local_serial->print_string(NGC_RS274::Interpreter::Processor::Line);
		//c_stager::local_serial->Write(CR);
		//c_stager::local_serial->print_string("\tPos @ exe: ");
		//c_stager::local_serial->Write(CR);
		//for (uint8_t axis_id = 0; axis_id < MACHINE_AXIS_COUNT; axis_id++)
		//{
			//c_stager::local_serial->print_string("\t"); c_stager::local_serial->print_string(c_machine::machine_axis_names[axis_id]);
			//c_stager::local_serial->print_float(c_gcode_buffer::collection[i].unit_target_positions[axis_id]);
			//c_stager::local_serial->print_string(" ");
		//}
		//c_stager::local_serial->Write(CR);
//
//
	//}
}

uint8_t c_stager::pre_stage_check()
{
	//anything that needs to happen to the block before staging goes here. 
	return Stager_Errors::OK;
}

uint8_t c_stager::post_stage_check()
{
	//anything that needs to happen to the block after staging goes here. 

	if (c_gcode_buffer::buffer_tail == NGC_BUFFER_SIZE)
		c_gcode_buffer::buffer_tail = 0;

	//Does cutter compensation have a block buffered waiting for a motion command?
	if (c_Cutter_Comp::previous_block_pointer != NULL
		&& c_Cutter_Comp::previous_block_pointer->state & (1 << BLOCK_STATE_PLANNED))
	{
		//point the machine block to the previous block in cutter compensation. This block
		// is being held for cutter compensation
		c_machine::machine_block = c_Cutter_Comp::previous_block_pointer;

	}
	//NO blocks held in compensation buffer
	else if (c_Cutter_Comp::previous_block_pointer == NULL)
	{
		//point the machine block to the current tail block
		c_machine::machine_block = &c_gcode_buffer::collection[c_gcode_buffer::buffer_tail];
	}
	
	return Stager_Errors::OK;
}


NGC_RS274::NGC_Binary_Block*c_stager::get_added_block()
{
	//buffer_head-1 is the last block we added. Tail may still be way behind, but the machine executes the tail
	if (c_gcode_buffer::buffer_head == 0)
		return &c_gcode_buffer::collection[NGC_BUFFER_SIZE - 1];
	else
		return &c_gcode_buffer::collection[c_gcode_buffer::buffer_head - 1];
}

//Stages the motion for the block passed in.
int16_t c_stager::stage_block_motion()
{
	
	/*
	Ive decided to allow this to move forward in the block buffer each time a block comes in.
	Each block has a 'copy' of the g/m code states as the block was processed, so effectively each block
	can be used to update the machine state just before the block executes. This will help simplify the
	complexities of cutter radius compensation.
	*/

	NGC_RS274::NGC_Binary_Block* local_block = c_stager::get_added_block();

	int16_t return_value = 0;
	uint8_t group = 1;

	//copy the current position into the post_position
	memcpy(c_stager::start_motion_position, c_stager::end_motion_position, MACHINE_AXIS_COUNT*(sizeof(float)));

	//Update non modal gcodes if they were defined.
	if (local_block->get_g_code_defined(NGC_RS274::Groups::G::NON_MODAL))
	{
		Events::NGC_Block::event_manager.set((int)Events::NGC_Block::e_event_type::Non_modal);
		c_stager::update_non_modals(local_block);
	}

	//Update stager states to the values in the new block for everything else (except non modals, we just did those)
	//Im not using memcpy because I only want to update the ones that changed.
	for (group = 1; group < COUNT_OF_G_CODE_GROUPS_ARRAY; group++)
	{
		if (local_block->get_g_code_defined(group))
		{
			//Set events for the g groups that changed in this block
			if (group == NGC_RS274::Groups::G::MOTION && local_block->g_group[NGC_RS274::Groups::G::MOTION] != previous_block->g_group[NGC_RS274::Groups::G::MOTION])
			{
				Events::NGC_Block::event_manager.set((int)Events::NGC_Block::e_event_type::Motion);
			}
			if (group == NGC_RS274::Groups::G::CUTTER_RADIUS_COMPENSATION)
				Events::NGC_Block::event_manager.set((int)Events::NGC_Block::e_event_type::Cutter_radius_compensation);
			if (group == NGC_RS274::Groups::G::TOOL_LENGTH_OFFSET)
				Events::NGC_Block::event_manager.set((int)Events::NGC_Block::e_event_type::Tool_length_offset);
			if (group == NGC_RS274::Groups::G::FEED_RATE_MODE)
				Events::NGC_Block::event_manager.set((int)Events::NGC_Block::e_event_type::Feed_rate_mode);
			if (group == NGC_RS274::Groups::G::UNITS)
				Events::NGC_Block::event_manager.set((int)Events::NGC_Block::e_event_type::Units);


			//c_stager::stager_state_g_group[group] = local_block->g_group[group];
		}
	}

	if (Events::NGC_Block::event_manager.get_clr((int)Events::NGC_Block::e_event_type::Units)) //<--block has a units changed command
	{
		c_stager::local_serial->print_string("units changed\r");
		c_machine::unit_scaler = 1;
		if (local_block->g_group[NGC_RS274::Groups::G::UNITS] == NGC_RS274::G_codes::INCH_SYSTEM_SELECTION)
		{
			c_machine::unit_scaler = 25.4;
		}
	}


	//Update stager states to the values in the new block for everything else (except non modals, we just did those)
	//Im not using memcpy because I only want to update the ones that changed.
	for (group = 0; group < COUNT_OF_M_CODE_GROUPS_ARRAY; group++)
	{
		if (local_block->get_m_code_defined(group))
		{
			//Set events for the m groups that changed in this block
			//if (group == NGC_RS274::Groups::G::MOTION && local_block->g_group[NGC_RS274::Groups::G::MOTION]	!= previous_block->g_group[NGC_RS274::Groups::G::MOTION])
			//c_block_events::set_event(Block_Events::MOTION);
			//if (group == NGC_RS274::Groups::G::CUTTER_RADIUS_COMPENSATION)
			//c_block_events::set_event(Block_Events::CUTTER_RADIUS_COMPENSATION);
			//if (group == NGC_RS274::Groups::G::TOOL_LENGTH_OFFSET)
			//c_block_events::set_event(Block_Events::TOOL_LENGTH_OFFSET);
			//if (group == NGC_RS274::Groups::G::FEED_RATE_MODE)
			//c_block_events::set_event(Block_Events::FEED_RATE_MODE);
			//c_stager::stager_state_m_group[group] = local_block->m_group[group];
		}
	}

	//Update line number if it has changed. LIne numbers change all the time.
	//We dont need an event for that because honestly, we dont care..
	c_stager::line_number = local_block->get_value('N');

	//Comments are not feasible unless/until someone gets off their ass and adds display support
	//Set feed rate
	if (local_block->get_defined('F'))
	{
		Events::NGC_Block::event_manager.set((int)Events::NGC_Block::e_event_type::Feed_rate);
		//local_block->persisted_values.feed_rate = local_block->get_value('F');
	}

	//// [4. Set spindle speed ]:
	if (local_block->get_defined('S'))
	{
		Events::NGC_Block::event_manager.set((int)Events::NGC_Block::e_event_type::Spindle_rate);
		//local_block->persisted_values.active_s = local_block->get_value('S');
	}

	// [5. Select tool ]: NOT SUPPORTED. Only tracks tool value.
	if (local_block->get_defined('T'))
	{
		Events::NGC_Block::event_manager.set((int)Events::NGC_Block::e_event_type::Tool_id);
		//local_block->persisted_values.active_t = local_block->get_value('T');
	}

	// [6. Change tool ]: NOT SUPPORTED
	// TODO: implement tool changes
	// - stop spindle if started, save position
	// - [homing]
	// - [output tool ID on GPIO] up to 32 tools
	// - hold, wait for feed
	// - [restore position]
	// - restore spindle
	// New mode, process it in hold/feed

	// [7. Spindle control ]:
	//if (gc_state.modal.spindle != gc_block.modal.spindle)
	//{
	//// Update spindle control and apply spindle speed when enabling it in this block.
	//// NOTE: All spindle state changes are synced, even in laser mode. Also, pl_data,
	//// rather than gc_state, is used to manage laser state for non-laser motions.
	//spindle_sync(gc_block.modal.spindle, pl_data->spindle_speed);
	//gc_state.modal.spindle = gc_block.modal.spindle;
	//}
	//pl_data->condition |= gc_state.modal.spindle; // Set condition flag for planner use.

	// [8. Coolant control ]:
	//if (gc_state.modal.coolant != gc_block.modal.coolant)
	//{
	//// NOTE: Coolant M-codes are modal. Only one command per line is allowed. But, multiple states
	//// can exist at the same time, while coolant disable clears all states.
	//coolant_sync(gc_block.modal.coolant);
	//if (gc_block.modal.coolant == COOLANT_DISABLE)
	//gc_state.modal.coolant = COOLANT_DISABLE;
	//else
	//gc_state.modal.coolant |= gc_block.modal.coolant;
	//}
	//pl_data->condition |= gc_state.modal.coolant; // Set condition flag for planner use.

	// [9. Enable/disable feed rate or spindle overrides ]: NOT SUPPORTED. Always enabled.

	// [10. Dwell ]:
	//if (gc_block.non_modal_command == NON_MODAL_DWELL)
	//mc_dwell(gc_block.values.p);


	/*
	NOTE!!! Events only signal that something changed in the g or m code modal blocks!
	Events are not set every time we loop through here. Only when something has changed.
	*/

	if (Events::NGC_Block::event_manager.get((int)Events::NGC_Block::e_event_type::Cutter_radius_compensation)) //<--block has a compensation command
	{
		//c_block_events::clear_event(Block_Events::CUTTER_RADIUS_COMPENSATION);
		//c_stager::local_serial->Write("cutter comp change...");
		//c_stager::local_serial->Write(CR);
		/*
		cutter compensation has 2 possible parameters we must now load
		P - the size we must compensate.
		D - the tool number we load from the tool table. (0-99)
		We can have one or the other but not both. One of them had to be
		specified or the interpreter would have thrown an error
		----------------------------------------------------------
		H=Length | D=Diameter
		---------------------
		h1|4.8     | 0.1      |
		h2|2.9     | 0.2      |
		h3|3.5     | 0.3      |
		h4|6.7     | 0.5      |
		h5|5.8     | 1.0      |
		---------------------
		*/
		//if P value was sent, store it in slot 0. Slot 0 changes anytime a P value is sent
		if (local_block->get_defined('P'))
			c_stager::tool_table[0].diameter = local_block->get_value('P');
		//If D was not set, or it specified D0 we will now use the P value
		//If D = 0 and a previous P value was set, that previous value now becomes active
		//local_block->persisted_values.active_d = local_block->get_value('D');

	}
	
	
	if (Events::NGC_Block::event_manager.get_clr((int)Events::NGC_Block::e_event_type::Tool_length_offset)) //<--block has a tool length command
	{
		//c_stager::local_serial->Write("tool length change...");
		//c_stager::local_serial->Write(CR);
		/*
		tool length has 2 possible parameters we must now load
		P - the length we must offset.
		H - the tool length we load from the tool table. (0-99)
		We can have one or the other but not both. One of them had to be
		specified or the interpreter would have thrown an error
		----------------------------------------------------------
		H=Length | D=Diameter
		---------------------
		h1|4.8     | 0.1      |
		h2|2.9     | 0.2      |
		h3|3.5     | 0.3      |
		h4|6.7     | 0.5      |
		h5|5.8     | 1.0      |
		---------------------
		*/
		//if P value was sent, store it in slot 0. Slot 0 changes anytime a P value is sent
		if (local_block->get_defined('P'))
			c_stager::tool_table[0].height = local_block->get_value('P');
		//If H was not set, or it specified H0 we will now use the P value
		//If H = 0 and a previous P value was set, that previous value now becomes active
		//local_block->persisted_values.active_h = local_block->get_value('H');
	}

	/*
	Dont try to add data to the profile buffer if its full.
	This can happen if the motion system is not processing data fast enough (slow movements)
	and the ngc buffer was cleared as motions were added to the profiler. Until the profile
	buffer is empty, we will need to stop adding data to it. The ngc buffer will likely become
	full soon after this, and the controller will stop responding with NGC_Planner_Errors::OK
	so that the host will hold transmitting more data until the ngc buffer has space available.
	*/
	if (Events::NGC_Block::event_manager.get_clr((int)Events::NGC_Block::e_event_type::Motion)) //<--block has a motion command that is different than the previous block
	{

		//If motion type has changed and a canned cycle was running, we need to reset it
		c_canned_cycle::active_cycle_code = 0; //<--If this is zero when a cycle start, the cycle will re-initialize.
	}

	//is the motion a canned cycle? (But NOT cancel_cycle)
	if (local_block->g_group[NGC_RS274::Groups::G::MOTION] >= NGC_RS274::G_codes::CANNED_CYCLE_DRILLING
		&& local_block->g_group[NGC_RS274::Groups::G::MOTION] <= NGC_RS274::G_codes::CANNED_CYCLE_BORING_DWELL_FEED_OUT)
	{
		c_canned_cycle::initialize(local_block, *c_stager::previous_block->active_plane.normal_axis.value);
	}


	return_value = c_stager::update_cutter_compensation(local_block);
	
	if (return_value != NGC_Planner_Errors::OK)
		return return_value;

	if (c_Cutter_Comp::state == e_compensation_state::OFF)
	{
		//In order for the machine to execute a block, its state must be set to 'planned'
		//If compensation is on, then we let crc manage the plan state. But if its off,
		//we need to set the state to planned here.
		local_block->set_state(BLOCK_STATE_PLANNED);
	}

	//see if the motion state requires motion and a non modal was not on the local block.
	if (local_block->g_group[NGC_RS274::Groups::G::MOTION] != NGC_RS274::G_codes::MOTION_CANCELED
		&& !Events::NGC_Block::event_manager.get((int)Events::NGC_Block::e_event_type::Non_modal))
	{
		if (!Events::Data::event_manager.get((int)Events::Data::e_event_type::Staging_buffer_full) && local_block->any_axis_was_defined())
		{
			local_block->is_motion_block = true;
			Events::Motion::event_manager.set((int)Events::Motion::e_event_type::Motion_in_queue);
		}
	}

	//clear any non modal events after we finish
	Events::NGC_Block::event_manager.clear((int)Events::NGC_Block::e_event_type::Non_modal);

	////We need to know the position that this block took us too.
	////Set the stager starting position the the processed blocks end position
	//memcpy(c_stager::start_motion_position, local_block->unit_target_positions
		//, MACHINE_AXIS_COUNT * sizeof(float));

	c_stager::previous_block = local_block;
	return return_value;
}

int16_t c_stager::update_cutter_compensation(NGC_RS274::NGC_Binary_Block* local_block)
{
	uint16_t return_value = NGC_Planner_Errors::OK;

	if (c_Cutter_Comp::state == e_compensation_state::OFF)
	{
		c_Cutter_Comp::set_path(local_block);
	}

	c_Cutter_Comp::tool_radius = tool_table[(uint16_t)local_block->get_value('D')].diameter;

	//was cutter compensation flagged as changed
	if (Events::NGC_Block::event_manager.get_clr((int)Events::NGC_Block::e_event_type::Cutter_radius_compensation))
	{
		if (local_block->g_group[NGC_RS274::Groups::G::CUTTER_RADIUS_COMPENSATION] ==
			NGC_RS274::G_codes::START_CUTTER_RADIUS_COMPENSATION_LEFT)
		{

			//c_stager::update_cutter_compensation(local_block);
			c_Cutter_Comp::state = e_compensation_state::ON;
			c_Cutter_Comp::side = e_compensation_side::LEFT;
			//return NGC_Planner_Errors::OK;
		}
		else if (local_block->g_group[NGC_RS274::Groups::G::CUTTER_RADIUS_COMPENSATION] ==
			NGC_RS274::G_codes::START_CUTTER_RADIUS_COMPENSATION_RIGHT)
		{

			//c_stager::update_cutter_compensation(local_block);
			c_Cutter_Comp::state = e_compensation_state::ON;
			c_Cutter_Comp::side = e_compensation_side::RIGHT;
			//return NGC_Planner_Errors::OK;
		}
	}


	//if comp is on generate a compensated path
	if (c_Cutter_Comp::state == e_compensation_state::ON)
	{
		if (local_block->g_group[NGC_RS274::Groups::G::CUTTER_RADIUS_COMPENSATION] ==
			NGC_RS274::G_codes::CANCEL_CUTTER_RADIUS_COMPENSATION)
		{
			c_Cutter_Comp::state = e_compensation_state::OFF;
		}

		/*
		generate a comp move. If its the first move, it will be a lead in.
		If its the last move it will be a lead out. If it is neither first
		nor last then it will generate an offset and possibly a closing arc.
		*/
		
		return_value = c_Cutter_Comp::gen_comp(local_block);

	}

	return return_value;
}

void c_stager::update_non_modals(NGC_RS274::NGC_Binary_Block* local_block)
{
	//I know a non modal was defined but dont yet know which one.
	switch (local_block->g_group[NGC_RS274::Groups::G::NON_MODAL])
	{
	case NGC_RS274::G_codes::G10_PARAM_WRITE:
	{
		c_stager::parmeter_write(local_block);
	}
	break;
	default:
		/* Your code here */
		break;
	}

}

void c_stager::parmeter_write(NGC_RS274::NGC_Binary_Block* local_block)
{
	//Which parameter are we going to update
	switch ((uint16_t)local_block->get_value('L'))
	{
		//Using L2 and L20 as the same thing.. I'm kind of lazy these days.
	case NGC_RS274::Parmeters::L2:
	case NGC_RS274::Parmeters::L20:
	{
		//Get values for coordinate system origin G10 L/P/axis values
		c_stager::update_coordinate_datum((uint16_t)local_block->get_value('P'));
	}
	break;

	default:

		break;
	}

}

void c_stager::update_coordinate_datum(uint16_t parameter_slot)
{
	/*TODO:
	Dont forget that when the offsets are updated:
	if g90 is active the values are absolute
	if g91 is active the values are an offset from the current position
	*/
	//put the axis value into the coordinate slot specified
	float i_Address = 0;
	if (c_gcode_buffer::collection[c_gcode_buffer::buffer_tail].get_value_defined('A', i_Address))
		c_stager::coordinate_datum[parameter_slot].A = i_Address;
	if (c_gcode_buffer::collection[c_gcode_buffer::buffer_tail].get_value_defined('B', i_Address))
		c_stager::coordinate_datum[parameter_slot].B = i_Address;
	if (c_gcode_buffer::collection[c_gcode_buffer::buffer_tail].get_value_defined('C', i_Address))
		c_stager::coordinate_datum[parameter_slot].C = i_Address;
	if (c_gcode_buffer::collection[c_gcode_buffer::buffer_tail].get_value_defined('X', i_Address))
		c_stager::coordinate_datum[parameter_slot].X = i_Address;
	if (c_gcode_buffer::collection[c_gcode_buffer::buffer_tail].get_value_defined('Y', i_Address))
		c_stager::coordinate_datum[parameter_slot].Y = i_Address;
	if (c_gcode_buffer::collection[c_gcode_buffer::buffer_tail].get_value_defined('Z', i_Address))
		c_stager::coordinate_datum[parameter_slot].Z = i_Address;
	if (c_gcode_buffer::collection[c_gcode_buffer::buffer_tail].get_value_defined('U', i_Address))
		c_stager::coordinate_datum[parameter_slot].U = i_Address;
	if (c_gcode_buffer::collection[c_gcode_buffer::buffer_tail].get_value_defined('V', i_Address))
		c_stager::coordinate_datum[parameter_slot].V = i_Address;
}

int16_t c_stager::calculate_vector_distance(NGC_RS274::NGC_Binary_Block*plan_block)
{

	//for (int axis_id = 0; axis_id < MACHINE_AXIS_COUNT; axis_id++)
	//{
		////Before this is changed, we save of the start position for this motion block.
		////plan_block->start_position[axis_id] = c_stager::start_motion_position[axis_id];
		//float address = 0;
		////Check the axis value only if it was defined in the block
		//if (plan_block->get_value_defined(c_machine::machine_axis_names[axis_id], address))
		//{
			////If distance mode is absolute, we need to check to see if there is a
			////difference in the last position and the new position
			//if (plan_block->g_group[NGC_RS274::Groups::G::DISTANCE_MODE] == NGC_RS274::G_codes::ABSOLUTE_DISANCE_MODE)
			//{
//
				//plan_block->motion_direction[axis_id] = address - c_stager::start_motion_position[axis_id] < 0
					//? -1 : (address - c_stager::end_motion_position[axis_id] == 0 ? 0 : 1);
//
				////plan_block->vector_distance[axis_id] = (address - c_stager::start_motion_position[axis_id]);
				//plan_block->unit_target_positions[axis_id] = address;
			//}
			////If distance mode is incremental, we need to check to see if there is a value set
			//else if (plan_block->g_group[NGC_RS274::Groups::G::DISTANCE_MODE] == NGC_RS274::G_codes::INCREMENTAL_DISTANCE_MODE)
			//{
				//plan_block->motion_direction[axis_id] = address < 0 ? -1 : (address == 0 ? 0 : 1);
//
				//plan_block->unit_target_positions[axis_id] += address;
				////plan_block->vector_distance[axis_id] = address;
			//}
		//}
	//}
//
	////if (plan_block->motion_direction[MACHINE_X_AXIS] == 0 &&
		////plan_block->motion_direction[MACHINE_Y_AXIS] == 0 &&
		////plan_block->motion_direction[MACHINE_Z_AXIS] == 0 &&
		////plan_block->motion_direction[MACHINE_A_AXIS] == 0 &&
		////plan_block->motion_direction[MACHINE_B_AXIS] == 0 &&
		////plan_block->motion_direction[MACHINE_C_AXIS] == 0 &&
		////plan_block->motion_direction[MACHINE_U_AXIS] == 0 &&
		////plan_block->motion_direction[MACHINE_V_AXIS] == 0)
		////return NGC_Planner_Errors::NO_VECTOR_DISTANCE_MOTION_IGNORED;

	return NGC_Planner_Errors::OK;
}

//// default constructor
//c_planner::c_planner()
//{
//} //c_planner
//
//// default destructor
//c_planner::~c_planner()
//{
//} //~c_planner

