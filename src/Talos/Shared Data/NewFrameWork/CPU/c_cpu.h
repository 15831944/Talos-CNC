#ifndef _C_FRAMEWORK_CPU
#define _C_FRAMEWORK_CPU

#include <stdint.h>
#include "../../../_bit_flag_control.h"
#include "../../../c_ring_template.h"
#include "../_s_system_record.h"
#include "../_e_system_messages.h"
#include "../framework_configuration.h"


//namespace Talos
//{
//	namespace NewFrameWork
//	{
		class c_cpu
		{
		public:
			uint8_t ID;
			void initialize(uint8_t id, volatile uint32_t * tick_timer_ms);
			void Synch(
				e_system_message::messages::e_data init_message
				, e_system_message::e_status_type init_request_type
				, uint8_t init_ready_wait_id
				, e_system_message::e_status_type init_response_type
				, bool is_master_cpus);

			void service_events(int32_t * position, uint16_t rpm);
			bool send_message(uint8_t message
				, uint8_t type
				, uint8_t origin
				, uint8_t target
				, uint8_t state
				, uint8_t sub_state
				, int32_t * position_data);

			

			s_control_message sys_message;
			enum class e_event_type : uint8_t
			{
				NoState = 0,
				OnLine = 1,
				Error = 2,
				SystemRecord = 3,
				AddendumRecord = 4,
				ReBoot = 5,
				UnHealthy = 6,
			};
			s_bit_flag_controller<uint32_t> system_events;

			struct s_event_class
			{
				s_bit_flag_controller<uint32_t> Critical;
				s_bit_flag_controller<uint32_t> Warning;
				s_bit_flag_controller<uint32_t> Informal;
				s_bit_flag_controller<uint32_t> Data;
				s_bit_flag_controller<uint32_t> Inquiry;
			};

			s_event_class host_events;
			
			volatile char hw_data_container[RING_BUFFER_SIZE];
			c_ring_buffer <char> hw_data_buffer;

			volatile uint32_t *pntr_cycle_count_ms;
			uint32_t message_lag_cycles = 0;
			uint32_t next_cycle_check_time = 0;
			uint32_t last_time_code = 0;

			void(*pntr_data_read_handler)(c_ring_buffer<char> * buffer, uint8_t cpu_id);
			void(*pntr_data_write_handler)();

		private:
			

			void __check_health();
			void __send_formatted_message(uint8_t init_message, uint8_t init_type);
			void __wait_formatted_message(uint8_t init_message, uint8_t init_type);

			void __check_data();
			void __assign_handler();
		};
//	};
//};
#endif