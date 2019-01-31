
/*
*  c_edm.h - NGC_RS274 controller.
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



#ifndef C_EDM_H_
#define C_EDM_H_

#include "..\Talos.h"

class c_edm_driver
{
	public:
	static void initialize();
	static float Get_ArcVoltage();
	static void Find_Part_Zero();
	static void Dither();
};

#endif /* C_EDM_H_ */
