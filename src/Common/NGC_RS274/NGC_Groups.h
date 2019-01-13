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


#ifndef NGC_Gcode_Groups_h
#define NGC_Gcode_Groups_h

#include <stdint.h>
class NGC_Gcode_Groups
{
public:
	static const uint8_t NON_MODAL = 0;
	static const uint8_t MOTION = 1;
	static const uint8_t PLANE_SELECTION = 2;
	static const uint8_t DISTANCE_MODE = 3;
	static const uint8_t UNASSIGNED_4 = 4;
	static const uint8_t FEED_RATE_MODE = 5;
	static const uint8_t UNITS = 6;
	static const uint8_t CUTTER_RADIUS_COMPENSATION = 7;
	static const uint8_t TOOL_LENGTH_OFFSET = 8;
	static const uint8_t UNASSIGNED_9 = 9;
	static const uint8_t RETURN_MODE_CANNED_CYCLE = 10;
	static const uint8_t UNASSIGNED_11 = 11;
	static const uint8_t COORDINATE_SYSTEM_SELECTION = 12;
	static const uint8_t PATH_CONTROL_MODE = 13;
	static const uint8_t UNASSIGNED_14 = 14;
	static const uint8_t RECTANGLAR_POLAR_COORDS_SELECTION = 15;
};
class NGC_Mcode_Groups
{
public:
	static const uint8_t STOPPING = 4;
	static const uint8_t TOOL_CHANGE = 6;
	static const uint8_t SPINDLE_TURNING = 7;
	static const uint8_t COOLANT = 8;
	static const uint8_t FEED_SPEED_OVERRIDE = 9;
};
#endif
