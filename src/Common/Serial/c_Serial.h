/*
*  c_Serial.h - NGC_RS274 controller.
*  A component of Talos
*
*  Copyright (c) 2016-2018 Jeff Dill
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


#ifndef c_Serial_h
#define c_Serial_h
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "s_Buffer.h"
#include "../../std_types.h"

class c_Serial
{
	//variables
	public:
	uint8_t EOL;
	
	protected:
	
	private:
	uint8_t _check_tx_port_data(char );
	uint8_t _check_rx_port_data();
	uint8_t _port;

	//functions
	public:
	c_Serial();
	c_Serial(uint8_t Port, uint32_t BaudRate);
	
	uint8_t Available();
	void ClearBuffer(void);
	void InitBuffer(void);
	char Get();
	char Peek();
	char Peek(uint8_t LookAhead);
	uint8_t HasEOL();
	void Write(const char*);
	void Write(char);
	void Write_ni(int16_t);
	void Write_ni(int16_t,uint8_t);
	void Write_nf(float);
	void print_int32(long n);
	void print_float(float n, uint8_t decimal_places);
	void print_float(float n);
	void print_string(const char *s);
	void SkipToEOL();
	uint8_t WaitFOrEOL(uint32_t max_timeout);
	uint16_t DataSize ();
	
	
	//~c_Seriali();
	protected:
	private:
	void print_uint32_base10(uint32_t n);
	
};

#endif //__C_SERIALI_H__
