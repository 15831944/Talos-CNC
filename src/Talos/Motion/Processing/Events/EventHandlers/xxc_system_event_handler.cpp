/*
*  c_motion_events.cpp - NGC_RS274 controller.
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

#include "c_system_event_handler.h"
#include "../../Data/DataHandlers/c_system_data_handler.h"
#include "c_motion_control_event_handler.h"
#include "c_motion_controller_event_handler.h"
#include "../../../../Shared Data/FrameWork/Data/cache_data.h"
#include "../../../../Shared Data/FrameWork/Event/c_event_router.h"

s_bit_flag_controller<uint32_t> Talos::Motion::Events::System::event_manager;

void Talos::Motion::Events::System::process(c_cpu *active_cpu, c_cpu *this_cpu)
{
	//Talos::Shared::FrameWork::StartUp::string_writer("router\r\n");
	Talos::Shared::FrameWork::Events::Router::process();
	//Talos::Shared::FrameWork::StartUp::string_writer("system\r\n");
	if( active_cpu->system_events._flag!=0)
	{
		//Talos::Shared::FrameWork::StartUp::int32_writer(this_cpu->system_events._flag);
		
		if (active_cpu->system_events.get_clr((int)c_cpu::e_event_type::SystemRecord))
		{
			switch ((e_system_message::e_status_type) active_cpu->sys_message.type)
			{
				case e_system_message::e_status_type::Informal:
				{
					switch ((e_system_message::messages::e_informal)active_cpu->sys_message.message)
					{
						case e_system_message::messages::e_informal::SpindleAvailable:
						{
							break;
						}
						case e_system_message::messages::e_informal::Reboot:
						{
							Talos::Shared::FrameWork::StartUp::string_writer("CC_TC=");
							Talos::Shared::FrameWork::StartUp::int32_writer(active_cpu->sys_message.time_code);
							Talos::Shared::FrameWork::StartUp::string_writer("\r\n");
							//this_cpu->system_events.set((int)c_cpu::e_event_type::ReBoot);
							break;
						}
						default:
						{
							/* Your code here */
							break;
						}
					}
				}
			}
		}
	}
	
	
	//	/*
	//	System records are potential events that came from something else (off board)
	//	They could trigger system events if another processor indicated an error.
	//	See if there is an event set indicating we have a system record
	//	*/
	//	//The router determines which event handler needs to process the message
	//	Talos::Shared::FrameWork::Events::Router.process();
	//
	//	if (Talos::Shared::FrameWork::Events::Router.ready.event_manager._flag > 0)
	//	{
	//
	//		//See if there is an event set indicating we have a status record
	//		if (Talos::Shared::FrameWork::Events::Router.ready.event_manager.get_clr((int)c_event_router::ss_ready_data::e_event_type::System))
	//		{
	//			//We got a system record in the working buffer. We need to get it out and assign it to the correct record address
	//
	//			//Copy the temp system record to the its final destination now that we know where it goes.
	//			memcpy(&Talos::Shared::c_cache_data::system_message_group[Talos::Shared::c_cache_data::temp_system_message.rx_from]
	//				, &Talos::Shared::c_cache_data::temp_system_message, s_control_message::__size__);
	//
	//				Talos::Motion::Data::System::process_system_eventing(&Talos::Shared::c_cache_data::system_message_group[Talos::Shared::c_cache_data::temp_system_message.rx_from]);
	//		}
	//
	//		//if (Talos::Shared::FrameWork::Events::Router.ready.event_manager.get_clr((int)c_event_router::ss_ready_data::e_event_type::MotionConfiguration))
	//		//{
	//			///*
	//			//We have an event that the motion configuration was updated. The framework would have already updated the data
	//			//but if we need to do something because the configuration was updated, we can do it here.
	//			//*/
	//			//
	//		//}
	//
	//	}
	//
	//	//This code is running in the motion controller, so we need to see if the coordinator and spindle are ready
	//	if (!MotionControl::event_manager.get((int)MotionControl::e_event_type::CycleStart))
	//	{
	//		if (System::event_manager.get((int)System::e_event_type::CoordinatorReady)
	//			&& System::event_manager.get((int)System::e_event_type::SpindleReady))
	//		{
	//			//prepare to run a program
	//			//1.Set drive system to lock
	//			//2.Enable drive motors
	//			//3.Set cycle start flag
	//			MotionControl::event_manager.set((int)MotionControl::e_event_type::CycleStart);
	//
	//		}
	//	}

}
