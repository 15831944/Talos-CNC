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


#ifndef NGC_INTERPTER_ERRORS_H
#define NGC_INTERPTER_ERRORS_H

#define ALLOC_BASE_ERROR_SIZE 50
#define ALLOC_G_ERROR_SIZE 50
#define ALLOC_M_ERROR_SIZE 50
#define ALLOC_WORD_ERROR_SIZE 50
#define ALLOC_CANNED_CYCLE_ERROR_SIZE 50
#define ALLOC_COORDINATE_ERROR_SIZE 50
#define ALLOC_TOOL_OFFSET_ERROR_SIZE 50
#define ALLOC_CUTTER_COMP_ERROR_SIZE 50
#define ALLOC_FEED_MODE_ERROR_SIZE 50
#define ALLOC_PLANE_ROTATION_ERROR_SIZE 50

#define BASE_GENERAL_ERROR         (0) //0-50
#define  BASE_ERROR_G_ERROR        (BASE_GENERAL_ERROR + ALLOC_BASE_ERROR_SIZE) //50-99
#define  BASE_ERROR_M_ERROR        (BASE_ERROR_G_ERROR + ALLOC_G_ERROR_SIZE) //100-149
#define  BASE_ERROR_WORD_ERROR     (BASE_ERROR_M_ERROR + ALLOC_M_ERROR_SIZE) //150-199
#define  BASE_CAN_CYCLE_ERROR      (BASE_ERROR_WORD_ERROR + ALLOC_WORD_ERROR_SIZE)//200-249
#define  BASE_COORDINATE_ERROR     (BASE_CAN_CYCLE_ERROR + ALLOC_CANNED_CYCLE_ERROR_SIZE)//250-299
#define  BASE_TOOL_OFFSET_ERROR    (BASE_COORDINATE_ERROR + ALLOC_COORDINATE_ERROR_SIZE)//300-349
#define  BASE_CUTTER_COMP_ERROR    (BASE_TOOL_OFFSET_ERROR + ALLOC_TOOL_OFFSET_ERROR_SIZE)//350-399
#define  BASE_PLANE_ROTATION_ERROR (BASE_CUTTER_COMP_ERROR + ALLOC_PLANE_ROTATION_ERROR_SIZE)//400-449
#define  BASE_PARSER_ERROR         (BASE_PLANE_ROTATION_ERROR + ALLOC_FEED_MODE_ERROR_SIZE)//450-499
#define  BASE_FEED_MODE_ERROR      (BASE_PARSER_ERROR + ALLOC_FEED_MODE_ERROR_SIZE)//500-549
#define  BASE_PLANE_SELECT_ERROR   (BASE_FEED_MODE_ERROR + ALLOC_FEED_MODE_ERROR_SIZE)//550-600
#define  BASE_NOT_UNDERSTOOD       1000 //1000+

enum class e_parsing_errors :uint16_t
{
	/*
	Errors for interpreter failure are all negative values, and are not used more than once!
	*/
	//1000 errors
	INTERPRETER_DOES_NOT_UNDERSTAND_G_WORD_VALUE = BASE_NOT_UNDERSTOOD + 1,
	INTERPRETER_DOES_NOT_UNDERSTAND_M_WORD_VALUE = BASE_NOT_UNDERSTOOD + 2,
	INTERPRETER_DOES_NOT_UNDERSTAND_CHARACTER_IN_BLOCK = BASE_NOT_UNDERSTOOD + 3,
	INTERPRETER_HAS_NO_DATA = BASE_NOT_UNDERSTOOD + 4,

	//0 errors
	OK = BASE_GENERAL_ERROR + 0, //<--Everything parsed fine
	MOTION_CANCELED_AXIS_VALUES_INVALID = BASE_GENERAL_ERROR + 1,
	NO_AXIS_DEFINED_FOR_MOTION = BASE_GENERAL_ERROR + 2,
	
	MISSING_CIRCLE_OFFSET_IJ = BASE_GENERAL_ERROR + 3,
	MISSING_CIRCLE_OFFSET_IK = BASE_GENERAL_ERROR + 4,
	MISSING_CIRCLE_OFFSET_JK = BASE_GENERAL_ERROR + 5,
	RADIUS_FORMAT_ARC_RADIUS_LESS_THAN_ZERO = BASE_GENERAL_ERROR + 6,
	CENTER_FORMAT_ARC_RADIUS_ERROR_EXCEEDS_005 = BASE_GENERAL_ERROR + 7,
	CENTER_FORMAT_ARC_RADIUS_ERROR_EXCEEDS_PERCENTAGE = BASE_GENERAL_ERROR + 8,

	MISSING_CIRCLE_AXIS_XY = BASE_GENERAL_ERROR + 9,
	MISSING_CIRCLE_AXIS_XZ = BASE_GENERAL_ERROR + 10,
	MISSING_CIRCLE_AXIS_YZ = BASE_GENERAL_ERROR + 11,
	MISSING_CIRCLE_AXIS_UV = BASE_GENERAL_ERROR + 12,
	MISSING_CIRCLE_AXIS_UW = BASE_GENERAL_ERROR + 13,
	MISSING_CIRCLE_AXIS_VW = BASE_GENERAL_ERROR + 14,
	ACTIVE_PLANE_UNSPECIFIED = BASE_GENERAL_ERROR + 15,
	
	NO_FEED_RATE_SPECIFIED = BASE_FEED_MODE_ERROR + 0,
	INVALID_FEED_RATE_SPECIFIED = BASE_FEED_MODE_ERROR + 1,
	NO_SPINDLE_VALUE_FOR_UNIT_PER_ROTATION = BASE_FEED_MODE_ERROR + 2,
	NO_SPINDLE_MODE_FOR_UNIT_PER_ROTATION = BASE_FEED_MODE_ERROR + 3,
	NO_FEED_RATE_MODE_SET = BASE_FEED_MODE_ERROR + 4,

	//40 errors
	G_CODE_GROUP_NON_MODAL_ALREADY_SPECIFIED = 1 + BASE_ERROR_G_ERROR,
	G_CODE_GROUP_NON_MODAL_AXIS_CANNOT_BE_SPECIFIED_WITH_MOTION = 2 + BASE_ERROR_G_ERROR,
	G_CODE_GROUP_MOTION_GROUP_ALREADY_SPECIFIED = 3 + BASE_ERROR_G_ERROR,
	G_CODE_GROUP_PLANE_SELECTION_ALREADY_SPECIFIED = 4 + BASE_ERROR_G_ERROR,
	G_CODE_GROUP_DISTANCE_MODE_ALREADY_SPECIFIED = 5 + BASE_ERROR_G_ERROR,
	G_CODE_GROUP_FEED_RATE_MODE_ALREADY_SPECIFIED = 6 + BASE_ERROR_G_ERROR,
	G_CODE_GROUP_UNITS_ALREADY_SPECIFIED = 7 + BASE_ERROR_G_ERROR,
	G_CODE_GROUP_CUTTER_RADIUS_COMPENSATION_ALREADY_SPECIFIED = 8 + BASE_ERROR_G_ERROR,
	G_CODE_GROUP_TOOL_LENGTH_OFFSET_ALREADY_SPECIFIED = 9 + BASE_ERROR_G_ERROR,
	G_CODE_GROUP_RETURN_MODE_CANNED_CYCLE_ALREADY_SPECIFIED = 10 + BASE_ERROR_G_ERROR,
	G_CODE_GROUP_COORDINATE_SYSTEM_SELECTION_ALREADY_SPECIFIED = 11 + BASE_ERROR_G_ERROR,
	G_CODE_GROUP_PATH_CONTROL_MODE_ALREADY_SPECIFIED = 12 + BASE_ERROR_G_ERROR,
	G_CODE_GROUP_NON_MODAL_MISSING_P_VALUE = 13 + BASE_ERROR_G_ERROR,
	G_CODE_GROUP_NON_MODAL_MISSING_L_VALUE = 14 + BASE_ERROR_G_ERROR,
	

	PLANE_ROTATION_CENTER_MISSING_FIRST_AXIS_VALUE = 0 + BASE_PLANE_ROTATION_ERROR,
	PLANE_ROTATION_CENTER_MISSING_SECOND_AXIS_VALUE = 1 + BASE_PLANE_ROTATION_ERROR,
	PLANE_ROTATION_MISSING_R_VALUE = 2 + BASE_PLANE_ROTATION_ERROR,
	PLANE_ROTATION_ANGLE_INVALID = 3 + BASE_PLANE_ROTATION_ERROR,
	PLANE_ROTATION_MISSING_I_VALUE = 4 + BASE_PLANE_ROTATION_ERROR,

	PLANE_SELECT_ILLEGAL_DURING_ACTIVE_ROTATION = 0 + BASE_PLANE_SELECT_ERROR,

	//60 errors	
	M_CODE_GROUP_STOPPING_ALREADY_SPECIFIED = 1 + BASE_ERROR_M_ERROR,
	M_CODE_GROUP_TOOL_CHANGE_ALREADY_SPECIFIED = 2 + BASE_ERROR_M_ERROR,
	M_CODE_GROUP_SPINDLE_ALREADY_SPECIFIED = 3 + BASE_ERROR_M_ERROR,
	M_CODE_GROUP_COOLANT_ALREADY_SPECIFIED = 4 + BASE_ERROR_M_ERROR,
	M_CODE_GROUP_OVERRIDE_ALREADY_SPECIFIED = 5 + BASE_ERROR_M_ERROR,
	M_CODE_GROUP_USER_DEFINED_ALREADY_SPECIFIED = 6 + BASE_ERROR_M_ERROR,

	WORD_A_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 0,
	WORD_B_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 1,
	WORD_C_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 2,
	WORD_D_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 3,
	WORD_E_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 4,
	WORD_F_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 5,
	WORD_G_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 6,
	WORD_H_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 7,
	WORD_I_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 8,
	WORD_J_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 9,
	WORD_K_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 10,
	WORD_L_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 11,
	WORD_M_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 12,
	WORD_N_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 13,
	WORD_O_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 14,
	WORD_P_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 15,
	WORD_Q_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 16,
	WORD_R_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 17,
	WORD_S_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 18,
	WORD_T_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 19,
	WORD_U_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 20,
	WORD_V_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 21,
	WORD_W_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 22,
	WORD_X_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 23,
	WORD_Y_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 24,
	WORD_Z_ALREADY_DEFINED = BASE_ERROR_WORD_ERROR + 25,

	//200 errors
	CAN_CYCLE_MISSING_R_VALUE = BASE_CAN_CYCLE_ERROR + 0,
	CAN_CYCLE_WORD_L_NOT_POS_INTEGER = BASE_CAN_CYCLE_ERROR + 1,
	CAN_CYCLE_WORD_L_LESS_THAN_2 = BASE_CAN_CYCLE_ERROR + 2,
	CAN_CYCLE_MISSING_X_VALUE = BASE_CAN_CYCLE_ERROR + 3,
	CAN_CYCLE_MISSING_Y_VALUE = BASE_CAN_CYCLE_ERROR + 4,
	CAN_CYCLE_MISSING_Z_VALUE = BASE_CAN_CYCLE_ERROR + 5,
	CAN_CYLCE_ROTATIONAL_AXIS_A_DEFINED = BASE_CAN_CYCLE_ERROR + 6,
	CAN_CYLCE_ROTATIONAL_AXIS_B_DEFINED = BASE_CAN_CYCLE_ERROR + 7,
	CAN_CYLCE_ROTATIONAL_AXIS_C_DEFINED = BASE_CAN_CYCLE_ERROR + 8,
	CAN_CYLCE_CUTTER_RADIUS_COMPENSATION_ACTIVE = BASE_CAN_CYCLE_ERROR + 9,
	CAN_CYCLE_LINEAR_AXIS_UNDEFINED = BASE_CAN_CYCLE_ERROR + 10,
	CAN_CYCLE_RETURN_MODE_UNDEFINED = BASE_CAN_CYCLE_ERROR + 11,
	CAN_CYCLE_MISSING_P_VALUE = BASE_CAN_CYCLE_ERROR + 12,

	//250 errors
	COORDINATE_SETTING_MISSING_L_VALUE = BASE_COORDINATE_ERROR + 0,
	COORDINATE_SETTING_MISSING_P_VALUE = BASE_COORDINATE_ERROR + 1,
	COORDINATE_SETTING_MISSING_AXIS_VALUE = BASE_COORDINATE_ERROR + 2,
	COORDINATE_SETTING_PARAM_VALUE_OUT_OF_RANGE = BASE_COORDINATE_ERROR + 3,
	COORDINATE_SETTING_L_VALUE_OUT_OF_RANGE = BASE_COORDINATE_ERROR + 4,
	COORDINATE_SETTING_INVALID_DURING_COMPENSATION = BASE_COORDINATE_ERROR + 5,

	//300 errors
	TOOL_OFFSET_MISSING_H_VALUE = BASE_TOOL_OFFSET_ERROR + 0,
	TOOL_OFFSET_H_VALUE_OUT_OF_RANGE = BASE_TOOL_OFFSET_ERROR + 1,
	TOOL_OFFSET_SETTING_PARAM_VALUE_OUT_OF_RANGE = BASE_TOOL_OFFSET_ERROR + 2,
	TOOL_OFFSET_SETTING_MISSING_PARAM_VALUE = BASE_TOOL_OFFSET_ERROR + 3,
	TOOL_OFFSET_SETTING_Q_VALUE_OUT_OF_RANGE = BASE_TOOL_OFFSET_ERROR + 4,

	//350 errors
	CUTTER_RADIUS_COMP_NOT_ALLOWED_XZ_PLANE = BASE_CUTTER_COMP_ERROR + 0,
	CUTTER_RADIUS_COMP_ALREADY_ACTIVE = BASE_CUTTER_COMP_ERROR + 1,
	CUTTER_RADIUS_COMP_MISSING_D_P_VALUE = BASE_CUTTER_COMP_ERROR + 2,
	CUTTER_RADIUS_COMP_D_VALUE_OUT_OF_RANGE = BASE_CUTTER_COMP_ERROR + 3,
	CUTTER_RADIUS_COMP_D_P_BOTH_ASSIGNED = BASE_CUTTER_COMP_ERROR + 4,
	CUTTER_RADIUS_COMP_WHEN_MOTION_CANCELED = BASE_CUTTER_COMP_ERROR + 5,
	CUTTER_RADIUS_COMP_SWITCH_SIDES_WHILE_ACTIVE = BASE_CUTTER_COMP_ERROR + 6,
	CUTTER_RADIUS_COMP_MISSING_P_VALUE = BASE_CUTTER_COMP_ERROR + 7,

	//400 errors
	NumericValueMissing = BASE_PARSER_ERROR + 0,
	DivideByZero = BASE_PARSER_ERROR + 1,
	ExponentOnNegativeNonInteger = BASE_PARSER_ERROR + 2,
	Unknown_Op_Name_A = BASE_PARSER_ERROR + 3,
	Unknown_Op_Name_C = BASE_PARSER_ERROR + 4,
	Unknown_Op_Name_M = BASE_PARSER_ERROR + 5,
	Unknown_Op_Name_O = BASE_PARSER_ERROR + 6,
	Unknown_Op_Name_X = BASE_PARSER_ERROR + 7,
	Unknown_Op_Name_E = BASE_PARSER_ERROR + 8,
	Unknown_Op_Name_F = BASE_PARSER_ERROR + 9,
	Unknown_Op_Name_N = BASE_PARSER_ERROR + 10,
	Unknown_Op_Name_G = BASE_PARSER_ERROR + 11,
	Unknown_Op_Name_L = BASE_PARSER_ERROR + 12,
	Unknown_Op_Name_R = BASE_PARSER_ERROR + 13,
	Unknown_Op_Name_S = BASE_PARSER_ERROR + 14,
	Unknown_Op_Name_T = BASE_PARSER_ERROR + 15,

	ExpressionNotClosed = BASE_PARSER_ERROR + 16,
	UnknownOperationClass = BASE_PARSER_ERROR + 17,
	IllegalCaller = BASE_PARSER_ERROR + 18,
	ParameterNameNotClosed = BASE_PARSER_ERROR + 19,
	IntExpectedAtValue = BASE_PARSER_ERROR + 20,
	ParamaterValueOutOfRange = BASE_PARSER_ERROR + 21,
	UnaryMissingOpen = BASE_PARSER_ERROR + 22,
	MissingSlashIn_ATAN = BASE_PARSER_ERROR + 23,
	MissingBracketAfterSlash = BASE_PARSER_ERROR + 24,
	ArcCosValueOutOfRange = BASE_PARSER_ERROR + 25,
	ArcSinValueOutOfRange = BASE_PARSER_ERROR + 26,
	LogValueNegative = BASE_PARSER_ERROR + 27,
	SqrtValueNegative = BASE_PARSER_ERROR + 28,
	UnHandledValueClass = BASE_PARSER_ERROR + 29,
	NoNamedParametersAvailable = BASE_PARSER_ERROR + 30,
	LocalNamedParametersNotAvailable = BASE_PARSER_ERROR + 31,
	GlobalNamedParametersNotAvailable = BASE_PARSER_ERROR + 32,
	NumericParametersNotAvailable = BASE_PARSER_ERROR + 33,
	NumericParametersMaxNotAvailable = BASE_PARSER_ERROR + 34,
	BadDataInBuffer = BASE_PARSER_ERROR + 35,
	OCodesNotImplimented = BASE_PARSER_ERROR + 36,
	NumericParamaterUpdateFailure = BASE_PARSER_ERROR + 37,
	NumericParamaterUpdateUnavailable = BASE_PARSER_ERROR + 38,
	NamedParamaterUpdateFailure = BASE_PARSER_ERROR + 39,
	NamedParamaterUpdateUnavailable = BASE_PARSER_ERROR + 40,


};
#endif 