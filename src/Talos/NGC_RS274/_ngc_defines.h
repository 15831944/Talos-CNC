/*
*  ngc_defines.h - NGC_RS274 controller.
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


#ifndef NGC_DEFINES_H
#define NGC_DEFINES_H

#include "../physical_machine_parameters.h"

//How many words can a block contain
#define COUNT_OF_BLOCK_WORDS_ARRAY 26
//How many elements are in the G code group array
#define COUNT_OF_G_CODE_GROUPS_ARRAY 16
//How many elements are in the M code group array
#define COUNT_OF_M_CODE_GROUPS_ARRAY 11
//How many tool offset are in the tool table. (Note tool 0 is used when P value is sent)
#define COUNT_OF_TOOL_TABLE 10
#define INTERNAL_AXIS_COUNT 9 //x,y,z,a,b,c,u,v,w
#define G_CODE_MULTIPLIER 10
#define NEAR_ONE 0.9999
#define NEAR_ZERO 0.0001

#define A_WORD_BIT 0
#define B_WORD_BIT 1
#define C_WORD_BIT 2
#define D_WORD_BIT 3
#define E_WORD_BIT 4
#define F_WORD_BIT 5
#define G_WORD_BIT 6
#define H_WORD_BIT 7
#define I_WORD_BIT 8
#define J_WORD_BIT 9
#define K_WORD_BIT 10
#define L_WORD_BIT 11
#define M_WORD_BIT 12
#define N_WORD_BIT 13
#define O_WORD_BIT 14
#define P_WORD_BIT 15
#define Q_WORD_BIT 16
#define R_WORD_BIT 17
#define S_WORD_BIT 18
#define T_WORD_BIT 19
#define U_WORD_BIT 20
#define V_WORD_BIT 21
#define W_WORD_BIT 22
#define X_WORD_BIT 23
#define Y_WORD_BIT 24
#define Z_WORD_BIT 25



#define HORIZONTAL_MOTION_AXIS 0
#define VERTICAL_MOTION_AXIS 1
#define NORMAL_MOTION_AXIS 2
#define HORIZONTAL_ROTARY_AXIS 3
#define VERTICAL_ROTARY_AXIS 4
#define NORMAL_ROTARY_AXIS 5
#define HORIZONTAL_INCRIMENTAL_AXIS 6
#define VERTICAL_INCRIMENTAL_AXIS 7
#define NORMAL_INCRIMENTAL_AXIS 8

#define RS274NGC_MAX_PARAMETERS 5414
#define MAX_NAMED_PARAM_LENGTH 64


#endif /* NGC_DEFINES_H */

#ifdef MACHINE_TYPE_MILL
#include "Machine Specific/Mill/NGC_G_Codes.h"
#include "Machine Specific/Mill/NGC_M_Codes.h"
#endif

#ifdef MACHINE_TYPE_LATHE
#include "Machine Specific/Lathe/NGC_G_Codes.h"
#include "Machine Specific/Lathe/NGC_M_Codes.h"
#endif