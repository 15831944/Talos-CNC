
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

#ifndef RECORDS_DEF_H
#define RECORDS_DEF_H
#include "physical_machine_parameters.h"
#include "NGC_RS274/ngc_defines.h"
#include "bit_manipulation.h"
#include <string.h>

namespace BinaryRecords
{
#ifndef __C_CRITICAL_ERRORS
#define __C_CRITICAL_ERRORS
	enum class e_critical_errors
	{
		Hardware_failure_force_shutdown = 0
	};
#endif

#ifndef __C_RAMP_TYPE
#define __C_RAMP_TYPE
	enum class e_ramp_type
	{
		Accel, Cruise, Decel, Decel_Override
	};
#endif

#ifndef __C_MOTION_TYPE
#define __C_MOTION_TYPE
	enum class e_motion_type : uint8_t
	{
		rapid_linear = 0 * G_CODE_MULTIPLIER,
		feed_linear = 1 * G_CODE_MULTIPLIER,
		arc_cw = 2 * G_CODE_MULTIPLIER,
		arc_ccw = 3 * G_CODE_MULTIPLIER
	};
#endif

#ifndef __C_BLOCK_FLAG
#define __C_BLOCK_FLAG
	enum class e_block_flag : uint8_t
	{
		block_state_normal = 0,
		block_state_skip_sys_pos_update = 1,
		motion_state_accelerating = 2,
		motion_state_decelerating = 3,
		motion_state_cruising = 4,
		motion_state_stopping = 5,
	};
#endif

#ifndef __C_FEED_MODES
#define __C_FEED_MODES
	enum class e_feed_modes : uint16_t
	{
		FEED_RATE_MINUTES_PER_UNIT_MODE = 93 * G_CODE_MULTIPLIER,
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
		//DO NOT use 10 or 13 as binary record type identifiers.
		Unknown = 0,
		Motion = 1,
		Motion_Control_Setting = 2,
		Spindle_Control_Setting = 3,
		Jog = 4,
		Peripheral_Control_Setting = 5,
		Status = 6
	};
#endif

#ifndef __C_UNIT_TYPES
#define __C_UNIT_TYPES
	enum class e_unit_types : uint8_t
	{
		MM = 1,
		INCHES = 2
	};
#endif

#ifndef __C_SYSTEM_STATE_RECORD_TYPES
#define __C_SYSTEM_STATE_RECORD_TYPES
	enum class e_system_state_record_types : uint8_t
	{
		Motion_Active = 1, //Motion states 1-9
		Motion_Complete = 2,
		Motion_Idle = 3,
		Motion_Jogging = 4,
		Motion_Cancel = 5,
		Motion_Discarded = 6,
		Spindle_Stopped = 10, //spindle states 10-19
		Spindle_Running = 11,
		System_Error = 99
	};
#endif

#ifndef __C_SYSTEM_SUB_STATE_RECORD_TYPES
#define __C_SYSTEM_SUB_STATE_RECORD_TYPES
	enum class e_system_sub_state_record_types : uint8_t
	{
		Block_Complete = 1, //The block has completed. Block sequence # is returned in the num_message
		Block_Starting = 2, //Block is starting execution
		Block_Queuing = 3, //The block has been placed in the motion queue
		Block_Holding = 4, //Block was executing, but feed hold was instructed
		Block_Resuming = 5, //Feed hold released, block resuming
		Block_Reserved2 = 6, //Reserved
		Block_Reserved3 = 7, //Reserved
		Block_Reserved4 = 8, //Reserved
		Block_Reserved5 = 9, //Reserved
		Jog_Complete = 30,
		Jog_Running = 31,
		Jog_Failed = 32,
		Error_Axis_Drive_Fault_X = 90, //Closed loop driver error
		Error_Axis_Drive_Fault_Y = 91, //Closed loop driver error
		Error_Axis_Drive_Fault_Z = 92, //Closed loop driver error
		Error_Axis_Drive_Fault_A = 93, //Closed loop driver error
		Error_Axis_Drive_Fault_B = 94, //Closed loop driver error
		Error_Axis_Drive_Fault_C = 95, //Closed loop driver error
		Error_Axis_Drive_Fault_U = 96, //Closed loop driver error
		Error_Axis_Drive_Fault_V = 97, //Closed loop driver error
		Error_Axis_Drive_Reserved = 98, //Closed loop driver error
		Error_Setting_Max_Rate_Exceeded = 100, //Setting error for max rate
		Error_Spindle_Synch_Failed = 110 //Spindle failed to synch before timeout

	};

#endif



#ifndef __C_BINARY_RESPONSES
#define __C_BINARY_RESPONSES
	enum class e_binary_responses : uint8_t
	{
		Ok = 240,
		Data_Error = 239,
		Response_Time_Out = 238,
		//Jog_Complete =		237,
		Data_Rx_Wait = 236,
		Check_Sum_Failure = 235
	};
#endif

#ifndef __C_PERIPHERAL_PANEL_PROCESSING
#define __C_PERIPHERAL_PANEL_PROCESSING
	enum class e_peripheral_panel_processing : uint16_t
	{
		Block_Skip = 1, //when on any lines beginning with the skip '/' char are not executed
		Single_block = 2, // only one line at a time is processed.
		Spindle_On_Off = 3, // spindle manual on or off
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

	template <typename TN>
	struct s_bit_flag_controller
	{
		bool get(int get_value)
		{
			return (BitGet(_flag, get_value));
		};

		bool get_clr(int get_value)
		{
			bool ret = (BitGet(_flag, get_value));
			clear(get_value);
			return ret;
		};

		void flip(int flip_value)
		{
			BitFlp(_flag, flip_value);
		};

		void set(int set_value)
		{
			BitSet_(_flag, set_value);
		};

		void set(bool bit_value, int bit_num)
		{
			if (bit_value)
				BitSet_(_flag, bit_num);
			else
				BitClr_(_flag, bit_num);
		};

		void clear(int clear_value)
		{
			BitClr_(_flag, clear_value);
		};

		void reset()
		{
			_flag = 0;
		};

		volatile TN _flag;//because this can be accessed by interrupts, i am making it volatile
	};

	struct s_ngc_block
	{
		float word_values[26]; //<--hard code to 26, cuz there are always 26 letters in the alphabet
		s_bit_flag_controller<uint32_t> word_flags;
		s_bit_flag_controller<uint32_t> block_events;
		uint16_t g_group[COUNT_OF_G_CODE_GROUPS_ARRAY]; //There are 14 groups of gcodes (0-13)
		s_bit_flag_controller<uint32_t> g_code_defined_in_block;
		uint16_t m_group[COUNT_OF_M_CODE_GROUPS_ARRAY]; //There are 5 groups of mcodes (0-4)
		s_bit_flag_controller<uint32_t> m_code_defined_in_block;
		float target_motion_position[INTERNAL_AXIS_COUNT];  //Positions the last block of motion left us at
														    //when reading these we must always assume the plane
														    //values were h,v,n,hr,hv,hn,ih,iv,in because do not
															//have the plane data to review after the block is
															//processed and cached.
		uint32_t __station__;

		//char comment[256];

		//double a_number;
		//double b_number;
		//double c_number;
		//char comment[256];
		//double d_number_float;
		//double e_number;
		//double f_number;
		//int g_modes[16];
		//int h_number;
		//double i_number;
		//double j_number;
		//double k_number;
		//int l_number;
		//int line_number;
		//int n_number;
		//int motion_to_be;
		//int m_count;
		//int m_modes[11];
		//int user_m;
		//double p_number;
		//double q_number;
		//double r_number;
		//double s_number;
		//int t_number;
		//double u_number;
		//double v_number;
		//double w_number;
		//double x_number;
		//double y_number;
		//double z_number;
		//
		//int radius_flag;
		//double radius;
		//int theta_flag;
		//double theta;
		//
		//// control (o-word) stuff
		//long offset;                 // start of line in file
		//int o_type;
		//int o_number;
		//char *o_name;                // !!!KL be sure to free this
		////double params[INTERP_SUB_PARAMS];
	};

	//****************************************************************************************
	//structs in here are packed and byte aligned on 1. This is due to sending serial data in
	//binary format from one mcu to another and the mcu's have different architectures.
	//Some are 8 bit, some are 32 bit. When/if all mcus are using the same architecture this
	//struct packing can be removed. I am aware that there is a processor cost for this.
	//****************************************************************************************
	struct s_motion_arc_values
	{
		float horizontal_offset = 0;
		float vertical_offset = 0;
		float Radius = 0;
	};

	struct s_motion_data_block
	{
		s_motion_data_block()
		{
			s_motion_data_block::flag.reset();
			memset(s_motion_data_block::axis_values, 0, sizeof(s_motion_data_block::axis_values));
		}
		const BinaryRecords::e_binary_record_types record_type = BinaryRecords::e_binary_record_types::Motion;
		BinaryRecords::e_motion_type motion_type = BinaryRecords::e_motion_type::rapid_linear;
		float feed_rate = 0;
		float spindle_speed = 0;
		uint8_t spindle_state = 0;
		BinaryRecords::e_feed_modes feed_rate_mode = BinaryRecords::e_feed_modes::FEED_RATE_UNITS_PER_MINUTE_MODE;
		//float word_values[26];
		uint32_t line_number = 0;
		float axis_values[MACHINE_AXIS_COUNT];
		s_motion_arc_values arc_values;
		s_bit_flag_controller<uint32_t> flag;
		uint32_t sequence = 0; //system set value, used to track when a block of code as completed.
		uint32_t _check_sum = 0;

	};


	struct s_peripheral_group_processing
	{
		e_peripheral_panel_processing Toggles; //Bit values set for on/off panel options
	};

	struct s_peripheral_group_overrides
	{
		e_peripheral_panel_override_rapids RapidFeed;
		uint8_t TravelFeed; //0-150
		uint8_t SpindleSpeed; //0-150
	};

	struct s_peripheral_group_jogging
	{
		uint8_t Axis;
		float Scale;
	};


	struct s_peripheral_panel
	{
		const BinaryRecords::e_binary_record_types record_type = BinaryRecords::e_binary_record_types::Peripheral_Control_Setting;
		s_peripheral_group_processing Processing;
		s_peripheral_group_overrides OverRides;
		s_peripheral_group_jogging Jogging;
		uint32_t _check_sum = 0;

	};

	struct s_jog_data_block
	{
		const BinaryRecords::e_binary_record_types record_type = BinaryRecords::e_binary_record_types::Jog;
		float axis_value;
		uint8_t axis;
		uint32_t flag = (int)BinaryRecords::e_block_flag::block_state_normal;
		uint32_t _check_sum = 0;
	};

	struct s_encoder_meta
	{
		int32_t reg_tc0_cv1 = 0;
		int32_t reg_tc0_ra0 = 0;
		float speed_rps = 0;
		float speed_rpm = 0;
	};
	struct s_encoders
	{
		s_encoder_meta meta_data;
		uint32_t feedrate_delay;
		uint16_t samples_per_second; //How frequently are we going to check the encoder signal
		uint16_t ticks_per_revolution;//How many ticks are in a rev
		int32_t current_rpm;//what is the current spindle rpm
		int32_t target_rpm;//what is the target spindle rpm
		uint8_t variable_percent;//how far from the target can it be?
		uint8_t wait_spindle_at_speed;//do we wait for the spindle to get to speed before we start interpolation? 1 = yes, 0 = no
		uint8_t lock_to_axis;//if spindle lock_to_axis is set ('X','Y','Z','A','B','C','U','V','W') moving that axis will cause the spindle to rotate
		int32_t spindle_synch_wait_time_ms;//when we are waiting for spindle at speed, how many milliseconds do we wait for synch to occur.

		//function to determine if we are near enough to the rpm to start interpolation
		uint8_t near(uint32_t current_rpm, uint32_t target_rpm, uint8_t variable_percent)
		{
			if (current_rpm == 0 || target_rpm == 0)
			{
				return 0;
			}
			//5% = 0.05
			//1000rpm * .05 = 50
			uint8_t target_percent = (float)target_rpm * (float)variable_percent / 100.0;
			//return 1 if the current RPM is within the % range of the target. otherwise 0
			return (current_rpm >= (target_rpm - target_percent)) && (current_rpm <= (target_rpm + target_percent));
		};
	};


	struct s_hardware
	{
		s_encoders spindle_encoder;
		float back_lash_comp_distance[MACHINE_AXIS_COUNT];//53
		float distance_per_rotation[MACHINE_AXIS_COUNT];//53
		float interpolation_error_distance;//57
		float acceleration[MACHINE_AXIS_COUNT]; //13
		float max_rate[MACHINE_AXIS_COUNT];//25
		uint16_t steps_per_mm[MACHINE_AXIS_COUNT];//31
		uint16_t pulse_length = 0;//49
	};

	struct s_motion_control_settings
	{
		const BinaryRecords::e_binary_record_types record_type = BinaryRecords::e_binary_record_types::Motion_Control_Setting;
		float junction_deviation = 0;//43
		float arc_tolerance = 0;//47
		uint16_t arc_angular_correction = 12;//59
		uint8_t invert_mpg_directions = 0;//60
		BinaryRecords::e_unit_types machine_units = BinaryRecords::e_unit_types::MM;//61
		s_hardware Hardware_Settings;
		uint32_t _check_sum = 0;//65
	};

	struct s_spindle_control_settings
	{
		uint32_t _check_sum = 0;
	};

	struct s_status_message
	{
		const BinaryRecords::e_binary_record_types record_type = BinaryRecords::e_binary_record_types::Status;
		BinaryRecords::e_system_state_record_types system_state;
		BinaryRecords::e_system_sub_state_record_types system_sub_state;
		float position[MACHINE_AXIS_COUNT];
		float num_message = 0.0;
		char * chr_message;
		uint32_t _check_sum = 0;
	};

};
#endif /* RECORDS_DEF_H */