#pragma once 

#include "blink.h"

#define BLINK_EFFECT

typedef struct
{
	int instance_group;
	blink_SR song_rate;
	blink_SR sample_rate;
	int data_offset;

	blink_Position* positions;
	blink_WarpPoints* warp_points;
	blink_ParameterData* parameter_data;
} blink_EffectBuffer;

typedef blink_Error(*blink_Effect_Process)(void* proc_data, const blink_EffectBuffer* buffer, const float* in, float* out);
typedef blink_Error(*blink_Effect_Reset)(void* proc_data);

typedef struct
{
	void* proc_data;

	blink_Effect_Process process;
	blink_Effect_Reset reset;
} blink_Effect;

#ifdef BLINK_EXPORT
extern "C"
{
	// Blockhead will call this four times per effect block to create a set
	// synchronized effects for the purposes of crossfading between them to
	// avoid clicks.
	//
	// A crossfade between one or more effects occurs whenever block data changes
	// or the song loops back to an earlier position. These two sitations may
	// occur simulataneously therefore Blockhead requires four instances in total.
	//
	// Blockhead passes in a different value for instance_group for each set of
	// four synchronized effects it creates. Plugins can use this id to share
	// data between related instances if they need to.
	EXPORTED blink_Effect blink_make_effect(int instance_group);

	// Free all memory associated with this effect instance
	EXPORTED blink_Error blink_destroy_effect(blink_Effect effect);
}
#endif
