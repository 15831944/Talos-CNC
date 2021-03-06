#ifndef __C_KERNEL_ERROR_STRUCT
#define __C_KERNEL_ERROR_STRUCT
#include <stdint.h>


enum class e_error_behavior :uint8_t
{
	Critical = 0,
	Recoverable = 1,
	Informal = 2
};

struct s_error_stack
{
	uint16_t base;
	uint16_t method;
	uint16_t method_or_line;
};

struct s_kernel_error
{
	uint8_t sys_message;
	uint8_t sys_type;
	uint16_t user_code1;
	uint16_t user_code2;
	uint16_t read_crc;
	uint16_t new_crc;
	e_error_behavior behavior;
	uint8_t origin;
	s_error_stack stack;
	volatile char * data;
	uint16_t buffer_head;
	uint16_t buffer_tail;

};
#endif