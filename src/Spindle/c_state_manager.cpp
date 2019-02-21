/*
*  c_state_manager.cpp - NGC_RS274 controller.
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


#include "c_state_manager.h"
#include "c_driver.h"
#include "c_encoder.h"
#include "c_processor.h"
#include "..\Common\Hardware_Abstraction_Layer\c_hal.h"

uint8_t Spindle_Controller::c_state_manager::check_driver_state()
{
	//Check the current RPM
	float this_rpm = 0.0;
	uint16_t illegal_rotations = 0;
	this_rpm = Spindle_Controller::c_encoder::current_rpm();
	
	
	//Check for a direction change (cw,ccw,stop). If these match then no changes have been commanded.
	while ((uint8_t)Spindle_Controller::c_processor::local_block.get_m_code_value(NGC_RS274::Groups::M::SPINDLE) != Spindle_Controller::c_driver::Drive_Control.direction)
	{
		
		Spindle_Controller::c_processor::host_serial.print_string("setting state");
		Spindle_Controller::c_processor::host_serial.Write(CR);
		
		//Clear all PID values because we are changing direction
		//c_PID::Clear(c_PID::spindle_terms);
		//accel_output = c_PID::spindle_terms.Min_Val;
		//
		/*
		There is a difference in the programmed rotation, and the current rotation.
		We want to end the state of current rotation, and start the state of
		programmed rotation
		*/
		
		//determine the state of current rotation
		switch (Spindle_Controller::c_driver::Drive_Control.direction)
		{
			case M04: //<--drive is currently in CCW rotation (M04)
			{
				/*
				since we were in a rotating state, and now we are going to be stopping/changing direction,
				this is obviously deceleration.
				*/
				//Set state to decelerate
				Spindle_Controller::c_driver::Set_State(STATE_BIT_DECELERATE);

				//new direction is either stop, or cw. Either way, we must brake first
				Spindle_Controller::c_driver::Brake_Drive();
				//brake is active, and we continue in this state until rpm reaches 0.
				//If a stop is all we needed, we are essentially done. If we are going a new direction
				//however we set the direction pin, and then set directions to be =

				if (this_rpm < 1 && (uint8_t)Spindle_Controller::c_processor::local_block.get_m_code_value(NGC_RS274::Groups::M::SPINDLE) == M03) //(M03)
				{
					//if this is a new direction, set the direction pin, and then match the directions
					//between the driver and the parser
					Spindle_Controller::c_driver::Drive_Control.direction = (uint8_t)Spindle_Controller::c_processor::local_block.get_m_code_value(NGC_RS274::Groups::M::SPINDLE);

					//Set direction to forward
					Spindle_Controller::c_driver::Forward_Drive();
					//Set state to accelerate
					Spindle_Controller::c_driver::Set_State(STATE_BIT_ACCELERATE);
					//Clear decelerate bit
					Spindle_Controller::c_driver::Clear_State(STATE_BIT_DECELERATE);
				}
				//If the spindle was rotating, and then set to an off state, the brake will remain on.
			}
			break;
			case M03: //<--drive is currently in CW rotation (M03)
			{
				/*
				since we were in a rotating state, and now we are going to be stopping/changing direction,
				this is obviously deceleration.
				*/
				//Set state to decelerate
				Spindle_Controller::c_driver::Set_State(STATE_BIT_DECELERATE);

				//new direction is either stop, or ccw. Either way, we must brake first
				Spindle_Controller::c_driver::Brake_Drive();
				//brake is active, and we continue in this state until rpm reaches 0.
				//If a stop is all we needed, we are essentially done. If we are going a new direction
				//however we set the direction pin, and then set directions to be =

				if (this_rpm < 1 && (uint8_t)Spindle_Controller::c_processor::local_block.get_m_code_value(NGC_RS274::Groups::M::SPINDLE) == M04) //(M04)
				{
					//if this is a new direction, set the direction pin, and then match the directions
					//between the driver and the parser
					Spindle_Controller::c_driver::Drive_Control.direction = Spindle_Controller::c_processor::local_block.get_m_code_value(NGC_RS274::Groups::M::SPINDLE);

					//Set direction to reverse
					Spindle_Controller::c_driver::Reverse_Drive();
					//Set state to accelerate
					Spindle_Controller::c_driver::Set_State(STATE_BIT_ACCELERATE);
					//Clear decelerate bit
					Spindle_Controller::c_driver::Clear_State(STATE_BIT_DECELERATE);
				}
				//If the spindle was rotating, and then set to an off state, the brake will remain on.
			}
			break;
			case 0: //<--on start up this might be zero. Coudl default this to M5 though I guess
			case M05: //<--drive is currently not rotating at all (we think) (M05)
			{
				//This one is simple. Just set the direction and enable the drive. Just in case
				//we check the rpm anyway, because we expect spindle to not be rotating.

				if (this_rpm > 0)
				{
					illegal_rotations++;
					//rotation detected but we dont expect it while in an off state
					break; //<--brake out of the switch/case here. loop again
				}

				//new direction is either stop, cw, or ccw. Either way, brake first
				Spindle_Controller::c_driver::Brake_Drive();
				//assume new rpm of zero for now
				Spindle_Controller::c_driver::Drive_Control.target_rpm = 0; //(M05) spindle stop
				Spindle_Controller::c_driver::Drive_Control.target_rpm = *Spindle_Controller::c_processor::local_block.persisted_values.active_spindle_speed_S;

				//Set the rotation directions to match
				Spindle_Controller::c_driver::Drive_Control.direction = Spindle_Controller::c_processor::local_block.get_m_code_value(NGC_RS274::Groups::M::SPINDLE);

				if (Spindle_Controller::c_processor::local_block.get_m_code_value(NGC_RS274::Groups::M::SPINDLE) == M03) //(M03)
				{
					Spindle_Controller::c_driver::Forward_Drive();
				}

				if (Spindle_Controller::c_processor::local_block.get_m_code_value(NGC_RS274::Groups::M::SPINDLE) == M04) //(M04)
				{
					Spindle_Controller::c_driver::Reverse_Drive();
				}
				//If we are going to stop and remain stopped, these bit flag will be updated below.
				//since we were in an off state, and now we are going to be rotating, this is obviously acceleration
				//Set state to accelerate
				Spindle_Controller::c_driver::Set_State(STATE_BIT_ACCELERATE);
				//Clear decelerate bit
				Spindle_Controller::c_driver::Clear_State(STATE_BIT_DECELERATE);

			}
			break;
		}

		//With every loop check the RPM
		this_rpm = Spindle_Controller::c_encoder::current_rpm();

		/*
		When we do NOT expect rotations, we count the number of times we looped and detected movement.
		If this value reaches the maximum, we will throw an error. This might happen if the operator
		is moving the spindle by hand when we expect it to be off, and an M3/M4 command is trying to
		execute. It may also happen if there is electrical interference between the encoder	and the controller.
		*/

		if (illegal_rotations > 650) //<-- processor will loop incredibly fast. 650 should be hit in about .00001 seconds
		{
			return NGC_Machine_Errors::SPINDLE_ROTATION_NOT_EXPECTED;
		}
		//Last state to check. We were rotating, and now we have commanded to stop
		if (Spindle_Controller::c_processor::local_block.get_m_code_value(NGC_RS274::Groups::M::SPINDLE) == M05)
		{
			//Since our brake is on, we can just let it come to a stop. Nothing else for us to do.
			//Set the rotation directions to match
			Spindle_Controller::c_driver::Drive_Control.direction = Spindle_Controller::c_processor::local_block.get_m_code_value(NGC_RS274::Groups::M::SPINDLE);
			break; //<--brake out of our loop
		}
	}
	
	//Spindle_Controller::c_processor::host_serial.print_string("state is : ");
	//Spindle_Controller::c_processor::host_serial.print_int32(Spindle_Controller::c_driver::Drive_Control.direction);
	//Spindle_Controller::c_processor::host_serial.Write(CR);
	
	//
	if (Spindle_Controller::c_driver::Drive_Control.direction == M05)
	return NGC_Machine_Errors::OK;
	//
	Spindle_Controller::c_state_manager::process_state(this_rpm);

	return NGC_Machine_Errors::OK;
}

uint8_t Spindle_Controller::c_state_manager::process_state(float current_rpm)
{
	/*
	If we get to here, the spindle is expected to be on. Keep calculating the PID loop.
	any input from the host that requires a direction change, stop, rpm adjustment, feed rate
	change will cause the PID to update automatically.
	*/
	
	
	//If we don't call release_drive, the pwm output will be ignored.
	if (Spindle_Controller::c_driver::Get_State(STATE_BIT_BRAKE)) Spindle_Controller::c_driver::Release_Drive();
	
	//Set the drive to enabled if it was disabled.
	if (Spindle_Controller::c_driver::Get_State(STATE_BIT_DISABLE))
	{
		Spindle_Controller::c_processor::host_serial.print_string("ENABLE : ");
		Spindle_Controller::c_processor::host_serial.Write(CR);
		Spindle_Controller::c_driver::Enable_Drive();
	}
	//Spindle_Controller::c_driver::Enable_Drive();
	Spindle_Controller::c_processor::host_serial.print_string("output is : ");
	Spindle_Controller::c_processor::host_serial.print_int32(Spindle_Controller::c_processor::local_block.get_value('S'));
	Spindle_Controller::c_processor::host_serial.Write(CR);
	c_hal::driver.PNTR_DRIVE_ANALOG!=NULL?c_hal::driver.PNTR_DRIVE_ANALOG(Spindle_Controller::c_processor::local_block.get_value('S')):void();
	
	
	//if (Spindle_Controller::c_driver::Get_State(STATE_BIT_ACCELERATE))
	//{
	//if (Spindle_Controller::c_driver::accel_output< c_PID::spindle_terms.Min_Val)
	//{
	//Spindle_Controller::c_driver::accel_output = c_PID::spindle_terms.Min_Val;
	//}
	//Spindle_Controller::c_driver::Set_Acceleration(current_rpm);
	//}
	//else if (Spindle_Controller::c_driver::Get_State(STATE_BIT_DECELERATE))
	//{
	//Spindle_Controller::c_driver::Set_Decelerate(current_rpm);
	//}
	//else
	{
		//save current accel value in case we increase rpm from here.
		Spindle_Controller::c_driver::accel_output = Spindle_Controller::c_driver::current_output;
		
		current_rpm = Spindle_Controller::c_encoder::current_rpm();
		//c_Spindle_Drive::current_output = c_PID::Calculate(current_rpm,c_Spindle_Drive::Drive_Control.target_rpm,c_PID::spindle_terms);
	}
	//c_hal::driver.PNTR_DRIVE_ANALOG(Spindle_Controller::c_driver::current_output);
	c_hal::driver.PNTR_DRIVE_ANALOG!=NULL ? c_hal::driver.PNTR_DRIVE_ANALOG(Spindle_Controller::c_processor::local_block.get_value('S')):void();
	return NGC_Machine_Errors::OK;
}
