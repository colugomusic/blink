#pragma once 

#include "blink.h"

#define BLINK_SYNTH

typedef struct
{
	int instance_group;
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
	// Blockhead will call this four times per synthesizer block to create a set
	// synchronized synthesizers for the purposes of crossfading between them to
	// avoid clicks.
	//
	// A crossfade between one or more synthesizers occurs whenever block data changes
	// or the song loops back to an earlier position. These two sitations may
	// occur simulataneously therefore Blockhead requires four instances in total.
	//
	// Blockhead passes in a different value for instance_group for each set of
	// four synchronized synthesizers it creates. Plugins can use this id to share
	// data between related instances if they need to.
	EXPORTED blink_Synth blink_make_synth(int instance_group);

	// Free all memory associated with this synthesizer instance.
	// This will always be called four times per synthesizer block.
	//
	// process() will no longer be called for any other instances in the
	// instance_group so it is safe to free any shared instance data the first
	// time this is called for a any member of that instance_group.
	EXPORTED blink_Error blink_destroy_synth(blink_Synth synth);
}
#endif