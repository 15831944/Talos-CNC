#ifndef __C_CACHE_DATA__
#define __C_CACHE_DATA__

#include "../_s_motion_data_block.h"
#include "../_s_status_record.h"
#include "../_s_ngc_line_record.h"

//typedef void(*ret_pointer)(c_ring_buffer <char> * buffer);
namespace Talos
{
	namespace Shared
	{
		class c_cache_data
		{
			//variables
		public:
			//static void(*pntr_data_handler_release)(c_ring_buffer<char> * buffer);
			static s_motion_data_block motion_block_record;
			static s_status_message status_record;
			static s_ngc_line_record ngc_line_record;
			static s_ngc_block ngc_block_record;
		protected:
		private:


			//functions
		public:
			//static ret_pointer assign_write_handler(c_ring_buffer <char> * buffer, s_data_events::e_event_type event_id);
			//static void ngc_write_handler(c_ring_buffer <char> * buffer);
			//static ret_pointer assign_read_handler(c_ring_buffer <char> * buffer, s_data_events::e_event_type event_id);
			//static void ngc_read_handler(c_ring_buffer <char> * buffer);
		protected:
		private:
			//static void __release(c_ring_buffer <char> * buffer_source, s_data_events::e_event_type event_id);
			//static void __assign_error_handler(c_ring_buffer <char> * buffer_source, uint16_t error_value);
		}; //c_serial_events
	};
};

#endif