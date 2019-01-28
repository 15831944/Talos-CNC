/*
* c_configuration.cpp
*
* Created: 1/26/2019 1:00:24 PM
* Author: jeff_d
*/


#include "c_configuration.h"
#include "..\Common\Hardware_Abstraction_Layer\c_hal.h"

Plasma_Control::c_configuration::s_address Plasma_Control::c_configuration::input;
Plasma_Control::c_configuration::s_address Plasma_Control::c_configuration::output;

void Plasma_Control::c_configuration::initialize()
{
	//load configuration settings from eeprom
	uint8_t value = 0;
	c_hal::storage.PNTR_GET_BYTE(1, &value);
	Plasma_Control::c_configuration::input.pin = value;

	c_hal::storage.PNTR_GET_BYTE(2, &value);
	Plasma_Control::c_configuration::input.port = value;

	uint8_t function = 0;
	c_hal::storage.PNTR_GET_BYTE(2, &function);
	//switch ((Plasma_Control::e_functions)function)
	//{
	//case Plasma_Control::e_functions::HIGH_LIMIT_SWITCH:
	//{
		////Assign a function to the condition for this configuration item.
//
		////Plasma_Control::c_configuration.input.PNTR_FUNC = 
		//break;
	//}
	//case Plasma_Control::e_functions::LOW_LIMIT_SWITCH:
	//{
		////Assign a function to the condition for this configuration item.
//
		////Plasma_Control::c_configuration.input.PNTR_FUNC = 
		//break;
	//}
	//case Plasma_Control::e_functions::ARC_GOOD:
	//{
		////Assign a function to the condition for this configuration item.
//
		////Plasma_Control::c_configuration.input.PNTR_FUNC = 
		//break;
	//}
	//default:
		///* Your code here */
		//break;
	//}

}
// default constructor
//Plasma_Control::c_configuration::c_configuration()
//{
//} //c_configuration
//
//// default destructor
//Plasma_Control::c_configuration::~c_configuration()
//{
//} //~c_configuration
