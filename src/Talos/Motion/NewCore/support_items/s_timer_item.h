#ifndef __C_MOTION_CORE_TIMER_ITEM_H
#define __C_MOTION_CORE_TIMER_ITEM_H

#include <stdint.h>
#include "s_common_objects.h"
#include "../../_bit_flag_control.h"

struct s_timer_item
{
	uint32_t steps_to_execute_in_this_segment;           // Number of step events to be executed for this segment
	uint32_t timer_delay_value;  // Step distance traveled per ISR tick, aka step rate.
	s_common_timer_object common;
	uint32_t sequence;
};
#endif