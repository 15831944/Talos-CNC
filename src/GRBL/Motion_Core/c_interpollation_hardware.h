#include <stdint.h>
#include "c_segment_timer_bresenham.h"
#include "c_segment_timer_item.h"
namespace Motion_Core
{
	namespace Hardware
	{
#ifndef __C_HARDWARE_INTERPOLLATION
#define __C_HARDWARE_INTERPOLLATION
		class Interpollation
		{
			static uint32_t counter[N_AXIS];
			
#ifdef STEP_PULSE_DELAY
			uint8_t step_bits;  // Stores out_bits output to complete the step pulse delay
#endif

			//static uint8_t execute_step;     // Flags step execution for each interrupt.
			static uint8_t step_pulse_time;  // Step pulse reset time after step rise
			static uint8_t step_outbits;         // The next stepping-bits to be output
			static uint8_t dir_outbits;
#ifdef ADAPTIVE_MULTI_AXIS_STEP_SMOOTHING
			uint32_t steps[N_AXIS];
#endif

			//uint16_t step_count;       // Steps remaining in line segment motion
			static Motion_Core::Segment::Bresenham::Bresenham_Item *Change_Check_Exec_Timer_Bresenham; // Tracks the current st_block index. Change indicates new block.
			static Motion_Core::Segment::Bresenham::Bresenham_Item *Exec_Timer_Bresenham;   // Pointer to the block data for the segment being executed
			static Motion_Core::Segment::Timer::Timer_Item *Exec_Timer_Item;  // Pointer to the segment being executed
			

			static uint8_t step_port_invert_mask;
			static uint8_t dir_port_invert_mask;

		public:
			static uint8_t Step_Active;
			static uint8_t Interpolation_Active;
			static uint32_t Current_Line;
			static void step_tick();
			static void Initialize();
			static void Shutdown();
			static uint32_t system_position[N_AXIS];

			Interpollation();
			~Interpollation();
		};
#endif
	};
};
