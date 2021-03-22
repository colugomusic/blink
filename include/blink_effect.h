#pragma once 

#include "blink.h"

// pos is a buffer of length blink_VECTOR_SIZE containing block positions.
//
// Positions will usually be increasing linearly but may jump back in the case of
// loop events.
//
// Be aware that Blockhead supports looping over extremely small regions (less
// than BLINK_VECTOR_SIZE)
//
// The is the only function that is called in the audio thread
//
// input and output pointers are aligned on 16-byte boundaries
typedef blink_Error (*blink_Effect_Process)(void* proc_data, blink_SR song_rate, blink_SR sample_rate, const blink_Position* pos, const float** in, float** out);

typedef struct
{
	//blink_GeneratorBase generator;

	void* proc_data;

	blink_Effect_Process process;
} blink_Effect;

#ifdef BLINK_EXPORT
extern "C"
{
	EXPORTED blink_Effect blink_make_effect();
	EXPORTED blink_Error blink_destroy_effect(blink_Effect effect);
}
#endif
