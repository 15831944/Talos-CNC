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
#include "c_ngc_data_handler.h"
#include "../../../../Shared Data/FrameWork/Data/cache_data.h"
#include "../../../Core/c_gateway.h"
#include "../../../../NGC_RS274/ngc_block_view.h"
#include "../../../Core/c_motion_core.h"
#include "../../../../Shared Data/FrameWork/Error/c_framework_error.h"

void Talos::Motion::Data::Ngc::load_block_from_cache()
{
	s_ngc_block * ngc_blk = &Talos::Shared::c_cache_data::ngc_block_record;
	NGC_RS274::Block_View blk_view = NGC_RS274::Block_View(ngc_blk);

	Motion_Core::initialize();

	//convert this to a motion gateway segment, store it and move on. 
	s_motion_data_block mot_blk;
	memset(&mot_blk, 0, sizeof(mot_blk));
	mot_blk.arc_values.horizontal_offset = blk_view.arc_values.horizontal_offset;
	mot_blk.axis_values[0] = 10; mot_blk.axis_values[1] = 10; //= ngc_blk->target_motion_position;
	mot_blk.feed_rate = *blk_view.persisted_values.feed_rate_F;
	mot_blk.feed_rate_mode = e_feed_modes::FEED_RATE_UNITS_PER_MINUTE_MODE;//  *blk_view.current_g_codes.Feed_rate_mode;
	mot_blk.line_number = *blk_view.persisted_values.active_line_number_N;
	mot_blk.motion_type = e_motion_type::rapid_linear;// *blk_view.current_g_codes.Motion;
	mot_blk.spindle_speed = *blk_view.persisted_values.active_spindle_speed_S;
	mot_blk.flag = 0;
	//mot_blk.spindle_state = 
	mot_blk.station = ngc_blk->__station__;

	Motion_Core::Gateway::add_motion(mot_blk);
	Motion_Core::Gateway::process_motion();
	//while (1)
	//{
	//	Motion_Core::Gateway::process_loop();
	//}

	//Clear the block event that was set when the line was loaded waaaaayyyy back in the dataevent handler
	Talos::Shared::FrameWork::StartUp::CpuCluster[Talos::Shared::FrameWork::StartUp::cpu_type.Host]
		.h_host_events.Data.clear((int)e_system_message::messages::e_data::NgcDataLine);

	//create an oubound request for ngc block data. 
	//Talos::Shared::FrameWork::Events::Router.outputs.event_manager.set((int)c_event_router::s_out_events::e_event_type::NgcBlockRequest);
	Talos::Shared::FrameWork::StartUp::CpuCluster[Talos::Shared::FrameWork::StartUp::cpu_type.Motion]
		.h_host_events.Data.set((int)e_system_message::messages::e_data::NgcDataRequest);
}

void  Talos::Motion::Data::Ngc::__raise_error(char * ngc_line)
{
	Talos::Shared::FrameWork::Error::ngc_error_handler(ngc_line);
	__reset();
}

void Talos::Motion::Data::Ngc::__reset()
{
	Talos::Shared::FrameWork::StartUp::CpuCluster[Talos::Shared::FrameWork::StartUp::cpu_type.Host]
		.h_host_events.Data.clear((int)e_system_message::messages::e_data::NgcDataLine);

	Talos::Shared::c_cache_data::ngc_block_record.__station__ = 0;


}