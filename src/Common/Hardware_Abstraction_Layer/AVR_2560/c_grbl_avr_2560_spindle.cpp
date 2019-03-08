/*
* c_grbl_win_limits.cpp
*
* Created: 3/7/2019 10:22:11 AM
* Author: jeff_d
*/

//#include <avr/io.h>
//#include <avr/interrupt.h>
#include "c_grbl_avr_2560_spindle.h"
#include "../../../helpers.h"
#include "../../../GRBL/c_settings.h"

float Hardware_Abstraction_Layer::Grbl::Spindle::pwm_gradient = 0; // Precalulated value to speed up rpm to PWM conversions.

void Hardware_Abstraction_Layer::Grbl::Spindle::initialize()
{
	// Configure variable spindle PWM and enable pin, if required.
	SPINDLE_PWM_DDR |= (1<<SPINDLE_PWM_BIT); // Configure as PWM output pin.
	SPINDLE_TCCRA_REGISTER = SPINDLE_TCCRA_INIT_MASK; // Configure PWM output compare timer
	SPINDLE_TCCRB_REGISTER = SPINDLE_TCCRB_INIT_MASK;
	SPINDLE_OCRA_REGISTER = SPINDLE_OCRA_TOP_VALUE; // Set the top value for 16-bit fast PWM mode
	SPINDLE_ENABLE_DDR |= (1<<SPINDLE_ENABLE_BIT); // Configure as output pin.
	SPINDLE_DIRECTION_DDR |= (1<<SPINDLE_DIRECTION_BIT); // Configure as output pin.

	Hardware_Abstraction_Layer::Grbl::Spindle::pwm_gradient = SPINDLE_PWM_RANGE/(c_settings::settings.rpm_max-c_settings::settings.rpm_min);
	Hardware_Abstraction_Layer::Grbl::Spindle::stop();
}

	// Disables the spindle and sets PWM output to zero when PWM variable spindle speed is enabled.
	// Called by various main program and ISR routines. Keep routine small, fast, and efficient.
	// Called by spindle_init(), spindle_set_speed(), spindle_set_state(), and mc_reset().
	void Hardware_Abstraction_Layer::Grbl::Spindle::stop()
	{
		SPINDLE_TCCRA_REGISTER &= ~(1<<SPINDLE_COMB_BIT); // Disable PWM. Output voltage is zero.
		#ifdef INVERT_SPINDLE_ENABLE_PIN
		SPINDLE_ENABLE_PORT |= (1<<SPINDLE_ENABLE_BIT);  // Set pin to high
		#else
		SPINDLE_ENABLE_PORT &= ~(1<<SPINDLE_ENABLE_BIT); // Set pin to low
		#endif
	}