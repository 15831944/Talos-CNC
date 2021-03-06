/*
* c_grbl_avr_2560_limits.h
*
* Created: 3/7/2019 10:22:11 AM
* Author: jeff_d
*/
#include "../../../Talos.h"

#ifdef __AVR_ATmega2560__
#ifndef __C_GRBL_AVR_2560_COOLANT_H__
#define __C_GRBL_AVR_2560_COOLANT_H__
#include <avr/io.h>

// Define flood and mist coolant enable output pins.
#define COOLANT_FLOOD_DDR  DDRH
#define COOLANT_FLOOD_PORT PORTH
#define COOLANT_FLOOD_BIT  5 // MEGA2560 Digital Pin 8
#define COOLANT_MIST_DDR   DDRH
#define COOLANT_MIST_PORT  PORTH
#define COOLANT_MIST_BIT   6 // MEGA2560 Digital Pin 9

#define COOLANT_STATE_DISABLE   0  // Must be zero
#define COOLANT_STATE_FLOOD     bit(0)
#define COOLANT_STATE_MIST      bit(1)

namespace Hardware_Abstraction_Layer
{
	namespace Grbl
	{
		class Coolant
		{
			//variables
			public:
			protected:
			private:

			//functions
			public:
			static void initialize();
			static void stop();
			static void set_state(uint8_t mode);
			protected:
			private:

		};
	};
};
#endif
#endif