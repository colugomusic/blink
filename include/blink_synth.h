#pragma once 

#include "blink.h"

#define BLINK_SYNTH

typedef struct
{
	blink_SR song_rate;
	blink_SR sample_rate;
	int data_offset;

	blink_Position* positions;
	blink_WarpPoints* warp_points;
	blink_ParameterData* parameter_data;
} blink_SynthBuffer;

typedef blink_Error(*blink_Synth_Process)(void* proc_data, const blink_SynthBuffer* buffer, float* out);
typedef blink_Error(*blink_Synth_Reset)(void* proc_data);

typedef struct
{
	void* proc_data;

	blink_Synth_Process process;
	blink_Synth_Reset reset;
} blink_Synth;

#ifdef BLINK_EXPORT
extern "C"
{
	EXPORTED blink_Synth blink_make_synth();
	EXPORTED blink_Error blink_destroy_synth(blink_Synth synth);
}
#endif