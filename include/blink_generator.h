#pragma once 

#include "blink.h"

typedef struct
{
	//blink_GeneratorBase generator;

	void* proc_data;

	//blink_Generator_Process process;
} blink_Generator;

#ifdef BLINK_EXPORT
extern "C"
{
	EXPORTED blink_Generator blink_make_generator();
	EXPORTED blink_Error blink_destroy_generator(blink_Generator generator);
}
#endif
