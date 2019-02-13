/*
*  c_hal.h - NGC_RS274 controller.
*  A component of Talos
*
*  Copyright (c) 2016-2019 Jeff Dill
*
*  Talos is free software: you can redistribute it and/or modify
*  it under the terms of the GNU LPLPv3 License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Talos is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with Talos.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef __C_HAL_H__
#define __C_HAL_H__
#include "../../std_types.h"
#include "../../talos.h"
#include "../Serial/s_Buffer.h"
#ifdef __AVR_ATmega2560__
#include "AVR_2560\c_cpu_AVR_2560.h"
#endif
#ifdef __AVR_ATmega328P__
#include "AVR_328\c_cpu_AVR_328.h"
#endif
#ifdef MSVC
#include "Virtual/c_cpu_VIRTUAL.h"
#endif
#ifdef __SAM3X8E__
#include "ARM_SAM3X8E/c_cpu_ARM_SAM3X8E.h"
#endif
#include "../../Coordinator/Shared/Settings/c_general.h"

class c_hal
{
	////variables
	//public:
	//// Step and direction port invert masks.
	
	//functions
	
	public:
	static void initialize();

	typedef struct
	{
		void (*TIMER1_COMPA_vect)(void);
		void (*TIMER1_CAPT_vect)(uint16_t,uint8_t);
		void (*TIMER0_OVF_vect)(void);
		void (*PCINT0_vect)(void);
		void (*PCINT2_vect)(void);
		void (*INT0_vect)(uint16_t,uint8_t);
		void (*INT1_vect)(uint16_t,uint8_t);
	}s_isr_pointers;
	static s_isr_pointers ISR_Pointers;

	//core struct refers to actions that give basic function
	typedef struct
	{
		void (*PNTR_INITIALIZE)(void);
		void (*PNTR_START_INTERRUPTS)(void);
		void (*PNTR_STOP_INTERRUPTS)(void);
		void (*PNTR_BASE)(void);		
		uint32_t (*PNTR_GET_CPU_SPEED)(void);
	}s_core_function_pointers;
	static s_core_function_pointers core;
	
	typedef struct
	{
		void (*PNTR_INITIALIZE)(uint8_t, uint32_t);
		void (*PNTR_BASE)(void);
		void (*PNTR_VIRTUAL_BUFFER_WRITE)(uint8_t, const char*);
		void (*PNTR_SERIAL_RX)(void);
		void (*PNTR_SERIAL_TX)(uint8_t , char);
		void (*PNTR_I2C_RX)(void);
		void (*PNTR_I2C_TX)(void);
		s_Buffer *PNTR_SERIAL_RX_BUFFER;
		
	}s_comm_function_pointers;
	static s_comm_function_pointers comm;	
	
	////driver struct refers to actions to run stepper or servo drivers
	//typedef struct
	//{
	//	void(*PNTR_INITIALIZE)(void);
	//	void(*PNTR_ENABLE)(void);
	//	void(*PNTR_DISABLE)(void);
	//	void(*PNTR_DRIVE)(void);
	//	void(*PNTR_RESET)(void);
	//	void(*PNTR_SET_PRESCALER)(uint16_t);
	//	void(*PNTR_SET_TIMER_RATE)(uint16_t);

	//}s_driver_function_pointers;
	//static s_driver_function_pointers driver;

	//driver struct refers to actions to run stepper or servo drivers
	typedef struct
	{
		void (*PNTR_INITIALIZE)(void);
		void (*PNTR_ENABLE)(void);
		void (*PNTR_DISABLE)(void);
		void (*PNTR_DRIVE)(void);
		void (*PNTR_BRAKE)(void);
		void (*PNTR_FORWARD)(void);
		void (*PNTR_REVERSE)(void);
		void (*PNTR_RELEASE)(void);
		void (*PNTR_DRIVE_ANALOG)(uint16_t);
		void (*PNTR_RESET)(void);
		void(*PNTR_SET_PRESCALER)(uint16_t);
		void(*PNTR_SET_TIMER_RATE)(uint16_t);
		uint8_t(*PNTR_AUX_DRIVE_CONTROLER)(uint8_t);
		void (*PNTR_CONFIGURE_STEPPER)(uint8_t,uint8_t,uint8_t,uint32_t);
	}s_driver_function_pointers;
	static s_driver_function_pointers driver;

	//feedback struct refers to actions to implement closed loop control
	typedef struct
	{
		void (*PNTR_INITIALIZE)(void);
		void (*PNTR_ENABLE)(void);
		void (*PNTR_DISABLE)(void);
		bool (*PNTR_IS_DIRTY)(void);
		void (*PNTR_PULSE_ISR)(void);
		int32_t *PNTR_POSITION_DATA;
		//void (*PNTR_RESET)(void);
		
	}s_feedback_function_pointers;
	static s_feedback_function_pointers feedback;
	static void (*PNTR_INTERNAL_PCINT0)(void);
	static void (*PNTR_INTERNAL_PCINT2)(void);
	
	//lcd struct refers to anything related to lcd display
	typedef struct
	{
		void (*PNTR_INITIALIZE)(void);
		void (*PNTR_ENABLE)(void);
		void (*PNTR_DISABLE)(void);
		void (*PNTR_UPDATE_AXIS_DISPLAY)(uint8_t,float);
		void (*PNTR_UPDATE_EDM_DISPLAY)(uint8_t,float);
	}s_lcd_function_pointers;
	static s_lcd_function_pointers lcd;

	//edm struct refers to anything related to edm control.
	typedef struct
	{
		void (*PNTR_INITIALIZE)(void);
		float (*PNTR_GET_ARC_VOLTAGE)(void);
		void (*PNTR_SET_ARC_DRIVE_FREQUENCY)(void);
	}s_edm_function_pointers;
	static s_edm_function_pointers edm;

	//io struct refers to anything related to reading input and output
	typedef struct
	{
		void(*PNTR_INITIALIZE)(void);
		int16_t(*PNTR_GET_ANALOG)(int16_t);
		uint8_t(*PNTR_GET_DIGITAL)(uint8_t);
		void(*PNTR_SET_ANALOG)(int16_t);
		void(*PNTR_SET_DIGITAL)(uint8_t);
	}s_input_function_pointers;
	static s_input_function_pointers io;

	//io struct refers to anything related to reading input and output
	typedef struct
	{
		void(*PNTR_INITIALIZE)(void);
		void(*PNTR_ENCODER_TIME_CAPTURE)();
		void(*PNTR_ENCODER_QUADRATURE)();
	}s_spindle_function_pointers;
	static s_spindle_function_pointers spindle;

	typedef struct
	{
		void(*PNTR_INITIALIZE)(void);
		void(*PNTR_GET_BYTE)(uint8_t,uint8_t*);
		void(*PNTR_GET_WORD)(uint16_t,uint16_t*);
		void(*PNTR_GET_DWORD)(uint32_t,uint32_t*);
		void(*PNTR_GET_FLOAT)(float,float*);
		void(*PNTR_SET_BYTE)(uint8_t*,uint8_t*);
		void(*PNTR_SET_WORD)(uint16_t*,uint16_t*);
		void(*PNTR_SET_DWORD)(uint32_t*,uint32_t*);
		void(*PNTR_SET_FLOAT)(float*,float);
	}s_storage_function_pointers;
	static s_storage_function_pointers storage;
	
	public:
	protected:
	private:
	//c_hal( const c_hal &c );
	//c_hal& operator=( const c_hal &c );
	//c_hal();
	//~c_hal();

}; //c_hal

#endif //__C_HAL_H__
