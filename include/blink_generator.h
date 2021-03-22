#pragma once 

#include "blkhdgen.h"

typedef struct
{
	//blkhdgen_GeneratorBase generator;

	void* proc_data;

	//blkhdgen_Generator_Process process;
} blkhdgen_Generator;

#ifdef BLKHDGEN_EXPORT
extern "C"
{
	EXPORTED blkhdgen_Generator blkhdgen_make_generator();
	EXPORTED blkhdgen_Error blkhdgen_destroy_generator(blkhdgen_Generator generator);
}
#endif
