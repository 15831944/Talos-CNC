
/*
*  hardware_def.h - NGC_RS274 controller.
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

#include "..\..\Talos.h"
#ifndef RECORDS_DEF_H
#define RECORDS_DEF_H
//#define G_CODE_MULTIPLIER 100

//#define MACHINE_AXIS_COUNT 3
namespace BinaryRecords
{
	//#define MOTION_RECORD 1
	//#define MOTION_CONTROL_SETTING_RECORD 2
	//#define SER_ACK_PROCEED 100 //proceed with more instructions
	//#define SER_BAD_READ_RE_TRANSMIT 101 //data is bad, re-send
	#ifndef __C_RAMP_TYPE
	#define __C_RAMP_TYPE
	enum class e_ramp_type
	{Accel, Cruise, Decel, Decel_Override};
	#endif
	
	#ifndef __C_MOTION_TYPE
	#define __C_MOTION_TYPE
	enum class e_motion_type : uint8_t
	{rapid_linear = 0,feed_linear = 1,arc_cw = 2,arc_ccw = 3};
	#endif
	
	#ifndef __C_BLOCK_FLAG
	#define __C_BLOCK_FLAG
	enum class e_block_flag : uint8_t
	{normal = 0, compensation = 1};
	#endif
	
	#ifndef __C_FEED_MODES
	#define __C_FEED_MODES
	enum class e_feed_modes : uint16_t
	{	FEED_RATE_MINUTES_PER_UNIT_MODE = 93 * G_CODE_MULTIPLIER,
		FEED_RATE_UNITS_PER_MINUTE_MODE = 94 * G_CODE_MULTIPLIER,
		FEED_RATE_UNITS_PER_ROTATION = 95 * G_CODE_MULTIPLIER,
		FEED_RATE_CONSTANT_SURFACE_SPEED = 96 * G_CODE_MULTIPLIER,
		FEED_RATE_RPM_MODE = 97 * G_CODE_MULTIPLIER
	};
	#endif
	
	#ifndef __C_BINARY_RECORD_TYPES
	#define __C_BINARY_RECORD_TYPES
	enum class e_binary_record_types : uint8_t
	{
		Unknown = 0,
		Motion = 1,
		Motion_Control_Setting = 2,
		Spindle_Control_Setting = 3,
		Jog = 4,
		Peripheral_Control_Setting = 5,
		Status = 6
	};
	#endif
	
	#ifndef __C_SYSTEM_STATE_RECORD_TYPES
	#define __C_SYSTEM_STATE_RECORD_TYPES
	enum class e_system_state_record_types : uint8_t
	{
		Motion_Active =		01,
		Motion_Complete =	02,
		Motion_Idle =		03,
		Motion_Jogging =	04,
		Spindle_Stopped =	20,
		Spindle_Running =	21,
	};
	#endif
	
	#ifndef __C_SYSTEM_SUB_STATE_RECORD_TYPES
	#define __C_SYSTEM_SUB_STATE_RECORD_TYPES
	enum class e_system_sub_state_record_types : uint8_t
	{
		Block_Complete = 01,
		Block_Starting = 02,
		Block_Queuing = 02,
	};
	#endif
	
	#ifndef __C_BINARY_RESPONSES
	#define __C_BINARY_RESPONSES
	enum class e_binary_responses : uint8_t
	{Ok = 1, Data_Error = 2,Response_Time_Out = 3, Jog_Complete = 4};
	#endif
	
	#ifndef __C_PERIPHERAL_PANEL_PROCESSING
	#define __C_PERIPHERAL_PANEL_PROCESSING
	enum class e_peripheral_panel_processing : uint16_t
	{
		Block_Skip = 1, //when on any lines begining with the skip '/' char are not executed
		Single_block = 2, // only on line at a time is processed.
		Spindle_On_Off = 3,// spindle manual on or off
		Coolant_On_Off = 4,
		Coolant_Mist_On_Off = 5,
		Coolant_Flood_On_Off = 6
	};
	#endif
	
	#ifndef __C_PERIPHERAL_PANEL_OVERRIDE_RAPIDS
	#define __C_PERIPHERAL_PANEL_OVERRIDE_RAPIDS
	enum class e_peripheral_panel_override_rapids : uint8_t
	{
		Zero = 0,
		_25 = 25,
		_50 = 50,
		_100 = 100
	};
	#endif
	//****************************************************************************************
	//structs in here are packed and byte aligned on 1. This is due to sending serial data in
	//binary format from one mcu to another and the mcu's have different architectures.
	//Some are 8 bit, some are 32 bit. When/if all mcus are using the same architecture this
	//struct packing can be removed. I am aware that there is a processor cost for this.
	//****************************************************************************************
	struct s_motion_arc_values
	{
		float horizontal_center = 0 ;
		float vertical_center = 0;
		float Radius = 0;
	}__attribute__((packed,aligned(1)));;

	struct s_motion_data_block
	{
		const BinaryRecords::e_binary_record_types record_type = BinaryRecords::e_binary_record_types::Motion;
		BinaryRecords::e_motion_type motion_type = BinaryRecords::e_motion_type::rapid_linear;
		float feed_rate = 0;
		BinaryRecords::e_feed_modes feed_rate_mode = BinaryRecords::e_feed_modes::FEED_RATE_UNITS_PER_MINUTE_MODE;
		//float word_values[26];
		uint32_t line_number = 0 ;
		float axis_values[MACHINE_AXIS_COUNT];
		s_motion_arc_values arc_values;
		BinaryRecords::e_block_flag flag = BinaryRecords::e_block_flag::normal;
		uint32_t sequence; //system set value, used to track when a block of code as completed.
		
	}__attribute__((packed,aligned(1)));;
	
	struct s_peripheral_group_processing
	{
		e_peripheral_panel_processing Toggles; //Bit values set for on/off panel options
	}__attribute__((packed,aligned(1)));;
	struct s_peripheral_group_overrides
	{
		e_peripheral_panel_override_rapids RapidFeed;
		uint8_t TravelFeed; //0-150
		uint8_t SpindleSpeed; //0-150
	}__attribute__((packed,aligned(1)));;
	struct s_peripheral_group_jogging
	{
		uint8_t Axis;
		float Scale;
	}__attribute__((packed,aligned(1)));;
	
	struct s_peripheral_panel
	{
		const BinaryRecords::e_binary_record_types record_type = BinaryRecords::e_binary_record_types::Peripheral_Control_Setting;
		s_peripheral_group_processing Processing;
		s_peripheral_group_overrides OverRides;
		s_peripheral_group_jogging Jogging;
		
	}__attribute__((packed,aligned(1)));;
	
	struct s_jog_data_block
	{
		const BinaryRecords::e_binary_record_types record_type = BinaryRecords::e_binary_record_types::Jog;
		float axis_value;
		uint32_t axis;
		BinaryRecords::e_block_flag flag = BinaryRecords::e_block_flag::normal;
	}__attribute__((packed,aligned(1)));
	
	
	struct s_motion_control_axis_settings
	{
		float acceleration;
		float max_rate;
		uint16_t steps_per_mm;
		float back_lash_comp_distance;
		float max_travel;
		uint32_t pulse_pin_assignment;
		uint32_t direction_pin_assignment;
	}__attribute__((packed,aligned(1)));
	
	struct s_motion_control_settings
	{
		const BinaryRecords::e_binary_record_types record_type = BinaryRecords::e_binary_record_types::Motion_Control_Setting;
		uint8_t axis_count;
		float junction_deviation;
		float arc_tolerance;
		uint16_t pulse_length;
		float interpolation_error_distance;
		uint16_t arc_angular_correction = 12;
		uint8_t invert_mpg_directions = 0;
		s_motion_control_axis_settings axis_configuration[];
		
	}__attribute__((packed,aligned(1)));
	
	//struct s_motion_control_settings
	//{
	//const BinaryRecords::e_binary_record_types record_type = BinaryRecords::e_binary_record_types::Motion_Control_Setting;
	//float acceleration[MACHINE_AXIS_COUNT];
	//float max_rate[MACHINE_AXIS_COUNT];
	//uint16_t steps_per_mm[MACHINE_AXIS_COUNT];
	//float junction_deviation;
	//float arc_tolerance;
	//uint16_t pulse_length;
	//float back_lash_comp_distance[MACHINE_AXIS_COUNT];
	//float interpolation_error_distance;
	//uint16_t arc_angular_correction = 12;
	//uint8_t invert_mpg_directions = 0;
	//
	//}__attribute__((packed,aligned(1)));
	
	struct s_spindle_control_settings
	{
		
	}__attribute__((packed,aligned(1)));
	struct s_status_message
	{
		const BinaryRecords::e_binary_record_types record_type = BinaryRecords::e_binary_record_types::Status;
		BinaryRecords::e_system_state_record_types system_state;
		BinaryRecords::e_system_sub_state_record_types system_sub_state;
		float num_message;
		char chr_message[17];
	}__attribute__((packed,aligned(1)));
	
	
	//static char motion_stream[sizeof(BinaryRecords::s_motion_data_block)];
	//static char setting_stream[sizeof(BinaryRecords::s_motion_data_block)];
	//
};
#endif /* RECORDS_DEF_H */