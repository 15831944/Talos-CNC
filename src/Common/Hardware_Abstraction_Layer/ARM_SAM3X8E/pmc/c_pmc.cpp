/*
*  c_pmc.cpp - NGC_RS274 controller.
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

#ifdef __SAM3X8E__//<--This will stop the multiple ISR definition error

#include "c_pmc.h"


void c_pmc::initialize()
{

}

void c_pmc::peripheral_clock_enable_register(Pmc *_pmc, uint32_t flags)
{
	_pmc->PMC_PCER0 |= flags;
}
#endif
// default constructor
//c_pmc::c_pmc()
//{
//} //c_pmc
//
//// default destructor
//c_pmc::~c_pmc()
//{
//} //~c_pmc
