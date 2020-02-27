#ifndef _C_FRAMEWORK_ERROR_STRUCT
#define _C_FRAMEWORK_ERROR_STRUCT
#include <stdint.h>
#include "_e_record_types.h"

enum class e_error_group :uint8_t
{
	EventHandler = 0,
	DataHandler = 1
};

enum class e_error_source :uint8_t
{
	Serial = 0,
	Disk = 1,
	Network = 2
};

enum class e_error_process :uint8_t
{
	EventHandle = 0,
	EventAssign = 1,
	Write = 2,
	Process = 3,
	Store = 4,
	Delete = 5,
	Read = 6
};

enum class e_error_behavior :uint8_t
{
	Critical = 0,
	Recoverable = 1,
	Informal = 2
};

enum class e_error_code :uint8_t
{
	AttemptToHandleNewEventWhilePreviousIsPending = 0,
	UnExpectedDataTypeForRecord = 1,
	UnHandledRecordType = 2,
	
};

struct s_framework_error
{
	e_error_group group;
	e_error_source source;
	e_error_process process;
	e_record_types record_type;
	e_error_behavior behavior;
	e_error_code code;
	uint8_t data_size;

};
#endif