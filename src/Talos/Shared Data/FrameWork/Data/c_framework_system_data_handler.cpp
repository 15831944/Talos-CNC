#include "c_framework_system_data_handler.h"
#include "../../_s_status_record.h"
#include "../Error/c_framework_error.h"
#include "cache_data.h"

#define BASE_ERROR 200
void(*Talos::Shared::FrameWork::Data::System::pntr_read_release)();
void(*Talos::Shared::FrameWork::Data::System::pntr_write_release)();

s_control_message * pntr_sys_wrk;

#define SYS_CONTROL_RECORD 0
#define SYS_ADDENDUM_RECORD 1

uint8_t crc_size = 2;

Talos::Shared::FrameWork::Data::s_packet read;
Talos::Shared::FrameWork::Data::s_packet write;

bool Talos::Shared::FrameWork::Data::System::send(uint8_t message
, uint8_t type
, uint8_t origin
, uint8_t target
, uint8_t state
, uint8_t sub_state
, int32_t * position_data
)
{
	//There are multiple system messages. Target determines which one is going to update.
	pntr_sys_wrk = &Talos::Shared::FrameWork::StartUp::CpuCluster[target].sys_message;
	//if the cache data system rec pointer is null we are free to use it. if its not, we must
	//leave the system_events set and keep checking on each loop. it should send after only 1 processor loop
	if (pntr_sys_wrk == NULL)
	return false;

	//set the pointer to the cache record
	//Talos::Shared::c_cache_data::pntr_system_record = &Talos::Shared::c_cache_data::system_record;

	pntr_sys_wrk->message = message;
	pntr_sys_wrk->origin = origin;
	pntr_sys_wrk->target = target;
	//copy position data from the interpolation hardware
	if (position_data != NULL)
	memcpy(pntr_sys_wrk->position
	, position_data
	, sizeof(int32_t)*MACHINE_AXIS_COUNT);
	pntr_sys_wrk->state = state;
	pntr_sys_wrk->sub_state = sub_state;
	pntr_sys_wrk->type = type;

	//make up some stuff for testing.
	pntr_sys_wrk->position[0] = 123;
	pntr_sys_wrk->position[1] = 456;
	pntr_sys_wrk->position[2] = 789;
	pntr_sys_wrk->position[3] = 102;
	pntr_sys_wrk->position[4] = 345;
	pntr_sys_wrk->position[5] = 678;

	Talos::Shared::FrameWork::Events::Router::outputs.event_manager.set((int)e_system_message::messages::e_data::SystemRecord);
	
	//Since a message is now in the queue, process system_events for it. This may also process other system_events that are pending in the framework
	Talos::Shared::FrameWork::Events::Router::process();
	return true;
}

#define ROUTE_READ_ERROR 1
#define SYSTEM_RECORD_POINTER_NULL 1
void Talos::Shared::FrameWork::Data::System::route_read(uint8_t event_id, s_bit_flag_controller<uint32_t> *event_object)
{
	read.cache[SYS_CONTROL_RECORD] = (char*)&Talos::Shared::FrameWork::StartUp::CpuCluster[event_id].sys_message;
	read.message_record = &Talos::Shared::FrameWork::StartUp::CpuCluster[event_id].sys_message;
	read.cache[SYS_ADDENDUM_RECORD] = NULL;
	read.counter = s_control_message::__size__;
	read.addendum_checked = false;
	read.event_id = event_id;
	read.event_object = event_object;
	read.target = 0;
	read.record_number = SYS_CONTROL_RECORD;
}

#define READER_ERROR 2
#define SYSTEM_CRC_FAILED 1
#define ADDENDUM_CRC_FAILED 2
void Talos::Shared::FrameWork::Data::System::reader()
{
	
	while ((Talos::Shared::FrameWork::Events::Router::inputs.pntr_ring_buffer + (int)read.event_id)->ring_buffer.has_data())
	{
		*read.cache[read.record_number] = (Talos::Shared::FrameWork::Events::Router::inputs.pntr_ring_buffer + (int)read.event_id)->ring_buffer.get();
		read.counter--;
		if (!read.counter)
		{
			
			//See if there is an addendum
			if (!read.addendum_checked)
			{
				__check_addendum(&read);
				
			}
			//if read counter is still zero then there is no addendum
			if (!read.counter)
			{
				//DO NOT CHANGE ANYTHING IN THE RECORD UNTIL WE CHECK CRC!!!
				uint16_t crc_check = __crc_compare(read.cache[SYS_CONTROL_RECORD], s_control_message::__size__);

				if (crc_check != 0)
				{
					Talos::Shared::FrameWork::Error::raise_error(BASE_ERROR, READER_ERROR, SYSTEM_CRC_FAILED,read.event_id);
				}
				
				read.message_record->rx_from = (int)read.event_id;

				//clear the read event. Thats the event that made us call into here and we are done
				read.event_object->clear(read.event_id);
				//A system record came in, so lets flag that event
				Talos::Shared::FrameWork::StartUp::CpuCluster[read.event_id].system_events.set((int)c_cpu::e_event_type::SystemRecord);
				
				//The reader that has been calling into here must now be released
				pntr_read_release();
				//Talos::Shared::c_cache_data::system_record.rx_from = read.event_id;
				//c_cache_data::temp_system_message.rx_from = read.event_id;
				
				//These need to be null again. If this code gets called by mistake we can check for nulls and throw an line.
				read.counter = 0;
				read.event_id = 0;
				read.event_object = NULL;
				pntr_read_release = NULL;
				//Shared::c_cache_data::pntr_system_record == NULL;

				////Copy the temp system record to the its final destination now that we know where it goes.
				//memcpy(&Talos::Shared::c_cache_data::sys_message[temp_system_message.rx_from]
				//	, &temp_system_message, s_control_message::__size__);

				//Do we have an andendum
				if (read.has_addendum)
				{
					//check addendum crc
					crc_check = __crc_compare(read.cache[SYS_ADDENDUM_RECORD], read.addendum_size);
					if (crc_check != 0)
					{
						Talos::Shared::FrameWork::Error::raise_error(BASE_ERROR, READER_ERROR, ADDENDUM_CRC_FAILED, read.event_id);
					}
					Talos::Shared::FrameWork::StartUp::CpuCluster[read.event_id].system_events.set((int)c_cpu::e_event_type::AddendumRecord);
					//__check_addendum function determined if an addendum was recieved with the data. If it was it gave us an event controller
					//and we need to set that event controllers flag so the rest of the application knows something came in, and what it was.
				}
				
				if (read.addendum_event_object != NULL)
				read.addendum_event_object->set(read.addendum_event_id);

				return;
			}
		}
		else
		{
			read.cache[read.record_number]++;
		}

	}
}

uint16_t Talos::Shared::FrameWork::Data::System::__crc_compare(char * source, uint16_t size)
{
	#define LAST_BYTE 0
	#define PREV_BYTE 1
	
	Talos::Shared::FrameWork::Error::framework_error.read_crc = ((uint8_t)*(source - LAST_BYTE) << 8) | (uint8_t)*(source - PREV_BYTE);
	
	*(source - LAST_BYTE) = 0;
	*(source - PREV_BYTE) = 0;
	
	Talos::Shared::FrameWork::Error::framework_error.new_crc = Talos::Shared::FrameWork::CRC::crc16(source - (size - 1), size - crc_size);
	return Talos::Shared::FrameWork::Error::framework_error.read_crc - Talos::Shared::FrameWork::Error::framework_error.new_crc;
}

#define ROUTE_WRITE_ERROR 4
void Talos::Shared::FrameWork::Data::System::route_write(uint8_t event_id, s_bit_flag_controller<uint32_t> *event_object)
{
	if (pntr_sys_wrk == NULL)
	Talos::Shared::FrameWork::Error::raise_error(BASE_ERROR, ROUTE_WRITE_ERROR, SYSTEM_RECORD_POINTER_NULL, event_id);

	write.counter = s_control_message::__size__;
	write.message_record = pntr_sys_wrk;
	write.event_id = event_id;
	write.event_object = event_object;
	write.target = pntr_sys_wrk->target;
	write.cache[SYS_CONTROL_RECORD] = (char*)pntr_sys_wrk;
	write.addendum_checked = false;
	//Clear the crc
	pntr_sys_wrk->crc = 0;
	//Get a crc value
	pntr_sys_wrk->crc = Talos::Shared::FrameWork::CRC::crc16(write.cache[SYS_CONTROL_RECORD], s_control_message::__size__ - crc_size);
	//System record is now ready to write out and has a crc on the end.
	write.record_number = SYS_CONTROL_RECORD;

}

#define WRITER_ERROR 5
void Talos::Shared::FrameWork::Data::System::writer()
{

	while (write.counter)
	{
		//target value <10 is a serial route
		if (write.target < 10)
		{
			Talos::Shared::FrameWork::Events::Router::outputs.pntr_serial_write(write.target, *(write.cache[write.record_number]));
		}

		write.cache[write.record_number]++;
		write.counter--;

		if (!write.counter)
		{
			//We now need to see if there is addendum data to send after that system record.
			if (!write.addendum_checked)
			__check_addendum(&write);

			//if write counter is still zero then there is no addendum
			if (!write.counter)
			{
				//The writer that has been calling into here must now be released
				pntr_write_release();
				//Clear the event so this will stop firing
				write.event_object->clear(write.event_id);

				//These need to be null again. If this code gets called by mistake we can check for nulls and throw an line.
				write.counter = 0;
				write.event_id = 0;
				write.event_object = NULL;
				write.cache[SYS_CONTROL_RECORD] = NULL;
				write.cache[SYS_ADDENDUM_RECORD] = NULL;
				write.target = 0;
				//release the system record
				pntr_sys_wrk = NULL;
			}
		}
	}
}

#define __CHECK_ADDENDUM 6
#define UNKNOWN_RECORD_TYPE 1
#define UNHANDLED_RECORD_TYPE 2
void Talos::Shared::FrameWork::Data::System::__check_addendum(s_packet *cache_object)
{
	cache_object->cache[SYS_ADDENDUM_RECORD] = NULL;
	cache_object->has_addendum = false;
	cache_object->addendum_checked = true;
	cache_object->addendum_size = 0;
	cache_object->addendum_event_object = NULL;
	cache_object->addendum_event_id = 0;

	switch ((e_system_message::e_status_type)cache_object->message_record->type)
	{
		case e_system_message::e_status_type::Data:
		{
			__classify_data_type_message(cache_object);
			break;
		}
		case e_system_message::e_status_type::Inquiry:
		{
			__classify_inquiry_type_message(cache_object);
			break;
		}
		case e_system_message::e_status_type::Critical:
		{
			Talos::Shared::FrameWork::Error::raise_error(BASE_ERROR, __CHECK_ADDENDUM, UNHANDLED_RECORD_TYPE, cache_object->event_id);
			break;
		}
		case e_system_message::e_status_type::Warning:
		{
			Talos::Shared::FrameWork::Error::raise_error(BASE_ERROR, __CHECK_ADDENDUM, UNHANDLED_RECORD_TYPE, cache_object->event_id);
			break;
		}
		case e_system_message::e_status_type::Informal:
		{
			//Informal messages shouldn't have an addendum. the 'information' is in the system record
			//__raise_error(BASE_ERROR, __CHECK_ADDENDUM, UNHANDLED_RECORD_TYPE, cache_object->event_id);
			break;
		}
		default:
		Talos::Shared::FrameWork::Error::raise_error(BASE_ERROR, __CHECK_ADDENDUM, UNKNOWN_RECORD_TYPE, cache_object->event_id);
		break;
	}
}


#define __CLASSIFY_DATA_TYPE_MESSAGE 7
#define UNKNOWN_MESSAGE_TYPE 1
#define UNCLASSED_MESSAGE_TYPE 2
void Talos::Shared::FrameWork::Data::System::__classify_data_type_message(s_packet *cache_object)
{
	switch (((e_system_message::messages::e_data)cache_object->message_record->message))
	{
		case e_system_message::messages::e_data::MotionConfiguration:
		{
			
			//Assign the second write cache object to the motion config record
			cache_object->cache[SYS_ADDENDUM_RECORD] = (char*)(&Talos::Shared::c_cache_data::motion_configuration_record);
			cache_object->has_addendum = true;
			cache_object->record_number = SYS_ADDENDUM_RECORD;
			//Clear the crc
			Talos::Shared::c_cache_data::motion_configuration_record.crc = 0;
			//Get a crc value
			Talos::Shared::c_cache_data::motion_configuration_record.crc =
			Talos::Shared::FrameWork::CRC::crc16(cache_object->cache[SYS_ADDENDUM_RECORD], s_motion_control_settings_encapsulation::__size__ - crc_size);
			cache_object->counter += s_motion_control_settings_encapsulation::__size__;
			cache_object->addendum_size = s_motion_control_settings_encapsulation::__size__;
			cache_object->addendum_event_object = &Talos::Shared::FrameWork::StartUp::CpuCluster[cache_object->event_id].host_events.Data;
			cache_object->addendum_event_id = (uint8_t)e_system_message::messages::e_data::MotionConfiguration;
			break;
		}
		default:
		Talos::Shared::FrameWork::Error::raise_error(BASE_ERROR, __CLASSIFY_DATA_TYPE_MESSAGE, UNCLASSED_MESSAGE_TYPE, cache_object->event_id);
		break;
	}
}

#define __CLASSIFY_INQUIRY_TYPE_MESSAGE 8
#define UNKNOWN_MESSAGE_TYPE 1
#define UNCLASSED_MESSAGE_TYPE 2
void Talos::Shared::FrameWork::Data::System::__classify_inquiry_type_message(s_packet *cache_object)
{
	switch (((e_system_message::messages::e_data)cache_object->message_record->message))
	{
		case e_system_message::messages::e_data::MotionConfiguration:
		{
			//This is an inquiry message to get motion configuration data. Construct an event to send
			//this data back
			
			
			//Talos::Shared::c_cache_data::pntr_motion_configuration_record = &Talos::Shared::c_cache_data::motion_configuration_record;
			////Assign the second write cache object to the motion config record
			//cache_object->cache[SYS_ADDENDUM_RECORD] = (char*)Talos::Shared::c_cache_data::pntr_motion_configuration_record;
			//cache_object->has_addendum = true;
			//cache_object->record_number = SYS_ADDENDUM_RECORD;
			////Clear the crc
			//Talos::Shared::c_cache_data::motion_configuration_record.crc = 0;
			////Get a crc value
			//Talos::Shared::c_cache_data::motion_configuration_record.crc =
			//Talos::Shared::FrameWork::CRC::crc16(cache_object->cache[SYS_ADDENDUM_RECORD], s_motion_control_settings_encapsulation::__size__ - crc_size);
			//cache_object->counter += s_motion_control_settings_encapsulation::__size__;
			//cache_object->addendum_size = s_motion_control_settings_encapsulation::__size__;
			cache_object->addendum_event_object = &Talos::Shared::FrameWork::StartUp::CpuCluster[cache_object->event_id].host_events.Inquiry;
			cache_object->addendum_event_id = (int)e_system_message::messages::e_data::MotionConfiguration;
			break;
		}
		default:
		Talos::Shared::FrameWork::Error::raise_error(BASE_ERROR, __CLASSIFY_DATA_TYPE_MESSAGE, UNCLASSED_MESSAGE_TYPE, cache_object->event_id);
		break;
	}
}