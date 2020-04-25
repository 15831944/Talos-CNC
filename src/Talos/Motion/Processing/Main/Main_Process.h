/*
* Main_Process.h
*
* Created: 7/12/2019 6:26:46 PM
* Author: Family
*/


#ifndef __MAIN_PROCESS_H__
#define __MAIN_PROCESS_H__

#include "../../motion_hardware_def.h"
#include "../../Serial/c_Serial.h"
#include "../../../Shared Data/FrameWork/extern_events_types.h"

namespace Talos
{
	namespace Motion
	{
		class Main_Process
		{
			//variables
			public:
			static c_Serial host_serial, coordinator_serial;
			protected:
			private:

			//functions
			public:
				static uint8_t coordinator_initialize();
				static void initialize();
			static void run();
			static void test_coord_msg();
			static void test_spindle_msg();
			static void test_motion_msg();
			static void test_ngc_block();
			static void error_handler();
			//Main_Process();
			//~Main_Process();
			protected:
			private:
				static void __configure_ports();
			typedef uint8_t (*init_function)(void); // type for conciseness
			static void __initialization_start(const char * message, init_function initialization_pointer, uint8_t critical);
			static void __initialization_response(uint8_t response_code, uint8_t critical);
			//Main_Process( const Main_Process &c );
			//Main_Process& operator=( const Main_Process &c );

		}; //Main_Process
	
	};
};
#endif //__MAIN_PROCESS_H__
