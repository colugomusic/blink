#pragma once 

#include "blkhdgen.h"

typedef struct
{
	blkhdgen_GeneratorInfo generator_info;

	blkhdgen_Generator_SetDataOffset set_data_offset;
	blkhdgen_Generator_Process process;
} blkhdgen_Generator;

#ifdef BLKHDGEN_EXPORT
extern "C"
{
	EXPORTED blkhdgen_Generator blkhdgen_make_generator();
	EXPORTED blkhdgen_Error blkhdgen_destroy_generator(blkhdgen_Generator generator);
}
#endif
