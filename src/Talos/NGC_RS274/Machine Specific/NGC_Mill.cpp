/*
*  NGC_Mill.cpp - NGC_RS274 controller.
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


#include "NGC_Mill.h"
#include <string.h>
#include "..\..\bit_manipulation.h"
#include "..\NGC_Errors.h"
#include "..\NGC_G_Groups.h"
#include "..\NGC_G_Codes.h"
#include "..\NGC_Interpreter.h"


/*
If a canned cycle (g81-g89) command was specified, perform detailed parameter check that applies
only to canned cycles.
*/
int NGC_RS274::Interpreter::NGC_Machine_Specific::error_check_canned_cycle()
{
	//Verify there is a retraction mode specified
	if (NGC_RS274::Interpreter::Processor::local_block.g_group[NGC_RS274::Groups::G::RETURN_MODE_CANNED_CYCLE] != NGC_RS274::G_codes::CANNED_CYCLE_RETURN_TO_R
		&& NGC_RS274::Interpreter::Processor::local_block.g_group[NGC_RS274::Groups::G::RETURN_MODE_CANNED_CYCLE] != NGC_RS274::G_codes::CANNED_CYCLE_RETURN_TO_Z)
	{
		return  NGC_RS274::Interpreter::Errors::CAN_CYCLE_RETURN_MODE_UNDEFINED;
	}

	//Check for rotational axis words. Those should not be used during canned cycles. RS274 standards page 30 section 3.5.16
	//Technically these CAN be specified, but their values have to be the same as their resting state. In other words if they
	//are specified they CANNOT cause movement
	if (BitTst(NGC_RS274::Interpreter::Processor::local_block.word_defined_in_block_A_Z, A_WORD_BIT))
		return  NGC_RS274::Interpreter::Errors::CAN_CYLCE_ROTATIONAL_AXIS_A_DEFINED;
	if (BitTst(NGC_RS274::Interpreter::Processor::local_block.word_defined_in_block_A_Z, B_WORD_BIT))
		return  NGC_RS274::Interpreter::Errors::CAN_CYLCE_ROTATIONAL_AXIS_B_DEFINED;
	if (BitTst(NGC_RS274::Interpreter::Processor::local_block.word_defined_in_block_A_Z, C_WORD_BIT))
		return  NGC_RS274::Interpreter::Errors::CAN_CYLCE_ROTATIONAL_AXIS_C_DEFINED;

	//Cutter radius compensation cannot be active in a canned cycle.
	if (NGC_RS274::Interpreter::Processor::local_block.g_group[NGC_RS274::Groups::G::CUTTER_RADIUS_COMPENSATION] != NGC_RS274::G_codes::CANCEL_CUTTER_RADIUS_COMPENSATION)
		return  NGC_RS274::Interpreter::Errors::CAN_CYLCE_CUTTER_RADIUS_COMPENSATION_ACTIVE;

	//if L word specified it must be a positive integer > 1 (L is not required, only use it to repeat a cycle at the present location)
	if (BitTst(NGC_RS274::Interpreter::Processor::local_block.word_defined_in_block_A_Z, L_WORD_BIT))
	{
		//L was defined, make sure it >1 and integer
		if (NGC_RS274::Interpreter::Processor::local_block.block_word_values['L' - 65] < 2)
			return  NGC_RS274::Interpreter::Errors::CAN_CYCLE_WORD_L_LESS_THAN_2;
		//L was > 1, make sure it is an int
		//first cast the value to an int, then subtract the float value multiplied by 100. This will give the decimal only result as a whole number
		int mantissa = (NGC_RS274::Interpreter::Processor::local_block.block_word_values['L' - 65] - (int)NGC_RS274::Interpreter::Processor::local_block.block_word_values['L' - 65]) * 100;
		if (mantissa > 0)
			return  NGC_RS274::Interpreter::Errors::CAN_CYCLE_WORD_L_NOT_POS_INTEGER;
	}

	//Make sure at least one linear axis was defined on the line

	if (!NGC_RS274::Interpreter::Processor::local_block.any_linear_axis_was_defined())
		return  NGC_RS274::Interpreter::Errors::CAN_CYCLE_LINEAR_AXIS_UNDEFINED;

	//if Using R retract method (G99), make sure R was set.
	if (NGC_RS274::Interpreter::Processor::local_block.g_group[NGC_RS274::Groups::G::RETURN_MODE_CANNED_CYCLE] == NGC_RS274::G_codes::CANNED_CYCLE_RETURN_TO_R)
	{
		if (!BitTst(NGC_RS274::Interpreter::Processor::local_block.word_defined_in_block_A_Z, R_WORD_BIT) && NGC_RS274::Interpreter::Processor::local_block.get_value('R') == 0)
			return  NGC_RS274::Interpreter::Errors::CAN_CYCLE_MISSING_R_VALUE;
		//R value was set and we are in R retract mode. We cannot have an R value that is less than the current Z
		//(or whatever the retract axis is in the current plane). We cannot check that in the interpreter though.
		//The interpreter has no idea where the machine is, so we will check this value in the machine object class.

	}

	//if dwell cycle make sure P is set
	if (NGC_RS274::Interpreter::Processor::local_block.g_group[NGC_RS274::Groups::G::MOTION] == NGC_RS274::G_codes::CANNED_CYCLE_DRILLING_WITH_DWELL)
	{
		if (!BitTst(NGC_RS274::Interpreter::Processor::local_block.word_defined_in_block_A_Z, P_WORD_BIT) && NGC_RS274::Interpreter::Processor::local_block.get_value('P') == 0)
			return  NGC_RS274::Interpreter::Errors::CAN_CYCLE_MISSING_P_VALUE;
	}

	//if neither axis defined, it is an error
	if (!NGC_RS274::Interpreter::Processor::local_block.active_plane.horizontal_axis.is_defined() && !NGC_RS274::Interpreter::Processor::local_block.active_plane.vertical_axis.is_defined())
		return  NGC_RS274::Interpreter::Errors::CAN_CYCLE_LINEAR_AXIS_UNDEFINED;

	return  NGC_RS274::Interpreter::Errors::OK;
}
