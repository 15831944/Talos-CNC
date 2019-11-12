/*
* Main_Process.h
*
* Created: 7/12/2019 6:26:46 PM
* Author: Family
*/


#ifndef __MAIN_PROCESS_H__
#define __MAIN_PROCESS_H__

#include "../../coordinator_hardware_def.h"
#include "../../Serial/c_Serial.h"


//remove after testing

namespace Talos
{
	namespace Coordinator
	{
		class Main_Process
		{
			//variables
			public:
			static c_Serial host_serial;
			protected:
			private:

			//functions
			public:
			static void initialize();
			
			static void run();


			//Main_Process();
			//~Main_Process();
			protected:
			private:
			typedef uint8_t (*init_function)(void); // type for conciseness
			static void __initialization_start(const char * message, init_function initialization_pointer);
			static void __initialization_response(uint8_t response_code);
			//Main_Process( const Main_Process &c );
			//Main_Process& operator=( const Main_Process &c );

		}; //Main_Process
	};
};
#endif //__MAIN_PROCESS_H__


