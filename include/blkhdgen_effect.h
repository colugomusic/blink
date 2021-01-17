#pragma once 

#include "blkhdgen.h"

// pos is a buffer of length BLKHDGEN_VECTOR_SIZE containing block positions.
//
// Positions will usually be increasing linearly but may jump back in the case of
// loop events.
//
// Be aware that Blockhead supports looping over extremely small regions (less
// than BLKHDGEN_VECTOR_SIZE)
//
// The is the only function that is called in the audio thread
//
// input and output pointers are aligned on 16-byte boundaries
typedef blkhdgen_Error (*blkhdgen_Effect_Process)(void* proc_data, blkhdgen_SR song_rate, blkhdgen_SR sample_rate, const blkhdgen_Position* pos, const float** in, float** out);

typedef struct
{
	blkhdgen_GeneratorBase generator;

	void* proc_data;

	blkhdgen_Effect_Process process;
} blkhdgen_Effect;

#ifdef BLKHDGEN_EXPORT
extern "C"
{
	EXPORTED blkhdgen_Effect blkhdgen_make_effect();
	EXPORTED blkhdgen_Error blkhdgen_destroy_effect(blkhdgen_Effect effect);
}
#endif
