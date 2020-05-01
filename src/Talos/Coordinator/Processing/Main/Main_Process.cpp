/*
* Main_Process.cpp
*
* Created: 7/12/2019 6:26:46 PM
* Author: Family
This is the main entry point for the coordinator. All things pass through here and
then move to their respective modules.

*/



#include "Main_Process.h"
#include "../../../NGC_RS274/NGC_Line_Processor.h"
#include "../../../NGC_RS274/NGC_Tool.h"
#include "../../../NGC_RS274/NGC_Coordinates.h"
#include "../../../NGC_RS274/NGC_System.h"
#include "../../../Configuration/c_configuration.h"
#include "../Events/EventHandlers/c_ngc_data_events.h"
#include "../Events/EventHandlers/c_system_event_handler.h"
#include "../../../Shared Data/FrameWork/Data/cache_data.h"
#include "../../../Shared Data/FrameWork/Startup/c_framework_start.h"
#include "../../../Shared Data/FrameWork/Error/c_framework_error.h"
#include "../../../Shared Data/FrameWork/Event/Serial/c_new_serial_event_handler.h"
#include "../../../Shared Data/FrameWork/Data/c_framework_system_data_handler.h"
#include "../Data/DataHandlers/c_system_data_handler.h"
#include "../../../Shared Data/_s_status_record.h"
#include "../../../Shared Data/Settings/Motion/_s_motion_control_settings_encapsulation.h"
//
//#include <avr/io.h>
//#include <avr/interrupt.h>


#ifdef MSVC
static char test_line[256] = "?G1\r\n";
static int test_byte = 0;
#endif

c_Serial Talos::Coordinator::Main_Process::host_serial;
c_Serial Talos::Coordinator::Main_Process::motion_serial;

void Talos::Coordinator::Main_Process::initialize()
{
	//Talos::Coordinator::Main_Process::__configure_ports();

	//Setup the ports and function pointers so we know which cpu is talking and we can report back debug data
	Talos::Shared::FrameWork::StartUp::initialize(0, 0, 1, 2, 3
		, Talos::Coordinator::Main_Process::debug_string
		, Talos::Coordinator::Main_Process::debug_byte
		, Talos::Coordinator::Main_Process::debug_int
		, Talos::Coordinator::Main_Process::debug_float
		, Talos::Coordinator::Main_Process::debug_float_dec);
	

	Talos::Coordinator::Main_Process::host_serial = c_Serial(Talos::Shared::FrameWork::StartUp::cpu_type.Host, 250000); //<--Connect to host
	Talos::Coordinator::Main_Process::motion_serial = c_Serial(Talos::Shared::FrameWork::StartUp::cpu_type.Motion, 250000); //<--Connect to host
	Talos::Coordinator::Main_Process::host_serial.print_string("Coordinator initializing\r\n");

	__critical_initialization("Core", Hardware_Abstraction_Layer::Core::initialize, STARTUP_CLASS_CRITICAL);//<--core start up
	__critical_initialization("Interrupts", Hardware_Abstraction_Layer::Core::start_interrupts, STARTUP_CLASS_CRITICAL);//<--start interrupts on hardware
	__critical_initialization("Disk", Hardware_Abstraction_Layer::Disk::initialize, STARTUP_CLASS_CRITICAL);//<--drive/eprom storage
	__critical_initialization("\tSettings", Hardware_Abstraction_Layer::Disk::load_configuration, STARTUP_CLASS_WARNING);//<--drive/eprom storage
	__critical_initialization("\tConfiguration", Talos::Confguration::initialize, STARTUP_CLASS_CRITICAL);//<--g code buffer
	
		
	//__critical_initialization("Events", Talos::Shared::Events::initialize, STARTUP_CLASS_CRITICAL);//<--init system_events
	//__critical_initialization("Data Buffer", Talos::Motion::NgcBuffer::initialize,STARTUP_CLASS_CRITICAL);//<--g code buffer
	//__critical_initialization("Data Startup", c_ngc_data_handler::initialize, STARTUP_CLASS_CRITICAL);//<--g code buffer
	//__critical_initialization("Data Line", NGC_RS274::LineProcessor::initialize,STARTUP_CLASS_CRITICAL);//<--g code interpreter

	//Load the motion control settings from disk
	Hardware_Abstraction_Layer::Disk::load_motion_control_settings(&Talos::Shared::c_cache_data::motion_configuration_record);
	//Now try to comm with the motion controller so we can give it operatng parameters.
	//__critical_initialization("Motion Control Comms", Talos::Coordinator::Main_Process::motion_initialize, STARTUP_CLASS_WARNING);//<--motion controller card

	//__critical_initialization("Spindle Control Comms", NULL, STARTUP_CLASS_WARNING);//<--spindle controller card

	
	Talos::Shared::FrameWork::StartUp::CpuCluster[Talos::Shared::FrameWork::StartUp::cpu_type.Motion].Synch(
		e_system_message::messages::e_data::MotionConfiguration
		, e_system_message::e_status_type::Inquiry
		, (int)e_system_message::messages::e_data::SystemRecord
		, e_system_message::e_status_type::Data, true);
	

	//Load the initialize block from settings. These values are the 'initial' values of the gcode blocks
	//that are processed.
	Hardware_Abstraction_Layer::Disk::load_initialize_block(&Talos::Shared::c_cache_data::ngc_block_record);


	//Assign the read,write function pointers. These assignments must take place outside the
	//block buffer control. The block buffer control system must not know anything about the HAL it
	//is servicing.
	Talos::Shared::c_cache_data::pntr_write_ngc_block_record = Hardware_Abstraction_Layer::Disk::put_block;
	Talos::Shared::c_cache_data::pntr_read_ngc_block_record = Hardware_Abstraction_Layer::Disk::get_block;
	//Write the start up block to cache
	Talos::Shared::c_cache_data::pntr_write_ngc_block_record(&Talos::Shared::c_cache_data::ngc_block_record);

	//setup the tool table controller
	NGC_RS274::Tool_Control::Table::pntr_tool_table_read = Hardware_Abstraction_Layer::Disk::get_tool;
	NGC_RS274::Tool_Control::Table::pntr_tool_table_write = Hardware_Abstraction_Layer::Disk::put_tool;

	//setup the wcs controller
	NGC_RS274::Coordinate_Control::WCS::pntr_wcs_read = Hardware_Abstraction_Layer::Disk::get_wcs;
	NGC_RS274::Coordinate_Control::WCS::pntr_wcs_write = Hardware_Abstraction_Layer::Disk::put_wcs;

#ifdef MSVC
	//Hardware_Abstraction_Layer::Serial::add_to_buffer(0, "g68x5.y5.R28.\r\ng0x6\r\n f4g1y1.\r\nx5\r\n");
	//cutter comp line 1 left comp test
	//Hardware_Abstraction_Layer::Serial::add_to_buffer(0, "p.25 g1 f1 g41 x1 y0\r\n g2 x1.5y0.5 i1.5 j0\r\n g1 x1.5y1.5\r\ng1 x3.5 y1.5\r\n");
	//cutter comp line 2 right comp test
	//Hardware_Abstraction_Layer::Serial::add_to_buffer(0, "p.25 g1 f1 g42 x1 y0\r\n g2 x2y0 i1.5 j0\r\n g1 x3y0\r\ng1 x4 y0\r\n");
	//cutter comp line 3 left comp start comp with arc
	//Hardware_Abstraction_Layer::Serial::add_to_buffer(0, "p.25 f1 g41 g2 x0.5 y0.5 i0.5 j0.0\r\n g1 x0.5y1.5\r\ng1 x0.5 y2.5\r\n");

	//simple gcode line
	//Hardware_Abstraction_Layer::Serial::add_to_buffer(0, "g01y5x5g91g20\r\n");
	//Hardware_Abstraction_Layer::Serial::add_to_buffer(0, "g01y10x10g91g20\r\n");
	//Hardware_Abstraction_Layer::Serial::add_to_buffer(0, "g01y0x0g91g20\r\n");
	//Hardware_Abstraction_Layer::Serial::add_to_buffer(0, "g01y0x0g90g20\r\n");
	//purposely bad g code line
	//Hardware_Abstraction_Layer::Serial::add_to_buffer(0, "g01 y7 g10x3 \r\n");//here axis words are used for motion and non modal. Thats an error
	//Hardware_Abstraction_Layer::Serial::add_to_buffer(0, "g0y#525r#<test>[1.0-[5.0+10]]\r\ng1x3\r\n");
	//Hardware_Abstraction_Layer::Serial::add_to_buffer(0, "g99 y [ #777 - [#<test> + #<_glob> +-sqrt[2]] ] \r\n\r\n\r\n\r\n");// /n/ng1x3\r\n");
	//Hardware_Abstraction_Layer::Serial::add_to_buffer(0, "#<tool>=10\r\n");
#endif


}

void Talos::Coordinator::Main_Process::__critical_initialization(const char * message, init_function initialization_pointer, uint8_t critical)
{
	Talos::Coordinator::Main_Process::host_serial.print_string(message);
	Talos::Coordinator::Main_Process::host_serial.print_string("...");
	if (initialization_pointer)
	{
		__initialization_response(initialization_pointer(), critical);
	}
	else
	{
		Talos::Coordinator::Main_Process::host_serial.print_string("Not Available\r\n");
	}

}

void Talos::Coordinator::Main_Process::__initialization_response(uint8_t response_code, uint8_t critical)
{
	//response codes that are not 0, are fatal at start up
	if (response_code)
	{
		Talos::Coordinator::Main_Process::host_serial.print_string("FAILED! Err Cd:");
		Talos::Coordinator::Main_Process::host_serial.print_int32(response_code);
		Talos::Coordinator::Main_Process::host_serial.print_string("\r\n");

		if (critical)
		{
			Talos::Coordinator::Main_Process::host_serial.print_string("** System halted **");
			while (1) {}
		}
		Talos::Coordinator::Main_Process::host_serial.print_string("\t** System warning **\r\n");
	}
	else
	{
		Talos::Coordinator::Main_Process::host_serial.print_string("OK.\r\n");
	}
}
volatile uint32_t tick_at_time = 0;

//ISR (TIMER5_COMPA_vect)
//{
//UDR0='A';
//Talos::Shared::FrameWork::Events::Router.ready.event_manager.set((int)c_event_router::ss_ready_data::e_event_type::Testsignal);
//}

void Talos::Coordinator::Main_Process::run()
{
	Talos::Coordinator::Main_Process::host_serial.print_string("\r\n** System ready **\r\n");

	while (Talos::Shared::FrameWork::StartUp::CpuCluster[Talos::Shared::FrameWork::StartUp::cpu_type.Host].system_events.get((int)c_cpu::e_event_type::OnLine))
	{
		if (tick_at_time > 0)
		{
			Talos::Coordinator::Main_Process::host_serial.print_int32(tick_at_time);
			Talos::Coordinator::Main_Process::host_serial.print_string("\r\n");
			tick_at_time = 0;
		}

		//0: Handle system system_events
		//Talos::Coordinator::Events::system_event_handler.process();

		//1: Handle hardware system_events
		//Talos::Coordinator::Events::hardware_event_handler.process();

		//2: System event handler (should always follow the router system_events)
		Talos::Coordinator::Events::System::process();

		//3: Handle ancillary system_events
		//Talos::Coordinator::Events::ancillary_event_handler.process();

		//4:: Handle ngc processing system_events
		Talos::Coordinator::Events::Data::process();

		//Handle cpu events
		Talos::Shared::FrameWork::StartUp::run_events();
				

		/*if (extern_data_events.inquire.event_manager.get((int)s_inquiry_data::e_event_type::IntialBlockStatus))
		ngc_block_report(Talos::Shared::c_cache_data::ngc_block_record);*/
	}

	Talos::Coordinator::Main_Process::host_serial.print_string("\r\n** System halted **");
	while (1) {}
}

void Talos::Coordinator::Main_Process::debug_string(const char * data)
{
	Talos::Coordinator::Main_Process::host_serial.print_string(data);
}
void Talos::Coordinator::Main_Process::debug_int(long data)
{
	Talos::Coordinator::Main_Process::host_serial.print_int32(data);
}
void Talos::Coordinator::Main_Process::debug_byte(const char data)
{
	Talos::Coordinator::Main_Process::host_serial.Write(data);
}
void Talos::Coordinator::Main_Process::debug_float(float data)
{
	Talos::Coordinator::Main_Process::host_serial.print_float(data);
}
void Talos::Coordinator::Main_Process::debug_float_dec(float data, uint8_t decimals)
{
	Talos::Coordinator::Main_Process::host_serial.print_float(data, decimals);
}
