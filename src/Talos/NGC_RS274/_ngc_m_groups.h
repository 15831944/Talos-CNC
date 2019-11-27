/*
*  NGC_Groups.h - NGC_RS274 controller.
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


#ifndef NGC_Mcode_Groups_h
#define NGC_Mcode_Groups_h

#include <stdint.h>
namespace NGC_RS274
{
	namespace Groups
	{
		class M
		{
			public:
			static const uint8_t UNUSED_0 = 0;
			static const uint8_t UNUSED_1 = 1;
			static const uint8_t UNUSED_2 = 2;
			static const uint8_t UNUSED_3 = 3;
			static const uint8_t STOPPING = 4;
			static const uint8_t UNUSED_4 = 5;
			static const uint8_t TOOL_CHANGE = 6;
			static const uint8_t SPINDLE = 7;
			static const uint8_t COOLANT = 8;
			static const uint8_t OVERRIDE = 9;
			static const uint8_t USER_DEFINED = 10;
		};
	};
}

#endif
