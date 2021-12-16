#pragma once 

#include "blink.h"

#define BLINK_SYNTH

typedef struct
{
	uint64_t buffer_id;
	int instance_group;
	blink_SR song_rate;
	int data_offset;

	blink_Position* positions;
	blink_ParameterData* parameter_data;
} blink_SynthBuffer;

typedef blink_Error(*blink_Synth_Process)(void* proc_data, const blink_SynthBuffer* buffer, float* out);

typedef struct
{
	void* proc_data;

	blink_Synth_Process process;
} blink_SynthUnit;

typedef blink_SynthUnit(*blink_SynthInstance_AddUnit)(void* proc_data);

typedef struct
{
	void* proc_data;

	blink_Instance_StreamInit stream_init;

	// Blockhead will call add_unit() four times per effect block to create a set
	// synchronized synths for the purposes of crossfading between them to
	// avoid clicks.
	//
	// A crossfade between one or more units occurs whenever block data changes
	// or the song loops back to an earlier position. These two sitations may
	// occur simulataneously therefore Blockhead requires four units in total.
	blink_SynthInstance_AddUnit add_unit;
} blink_SynthInstance;

#ifdef BLINK_EXPORT
extern "C"
{
	EXPORTED blink_SynthInstance blink_make_synth_instance();
	EXPORTED blink_Error blink_destroy_synth_instance(blink_SynthInstance instance);
}
#endif