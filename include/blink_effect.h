#pragma once 

#include "blink.h"

#define BLINK_EFFECT

typedef struct
{
	uint64_t buffer_id;
	blink_SR song_rate;
	int data_offset;

	blink_Position* positions;
	blink_ParameterData* parameter_data;
	blink_ManipulatorTargetData* manipulator_target_data;
} blink_EffectBuffer;

typedef struct
{
	// Blockhead uses these values to inform the user of any latency introduced
	// by the block due to buffering or whatever. Latency compensation is
	// dealt with manually by the end user (for now)
	// 
	// A value of -1 means "unknown"
	// If all values are -1, it means the block does not introduce any latency
	// 
	// Not all of these need to be filled. Blockhead will use:
	//  - exact_delay if it is set, otherwise
	//  - the (min/max)_delay pair if they are both set, otherwise
	//  - min_delay, otherwise
	//  - max_delay, otherwise
	// 	- approximate_delay
	// 
	int exact_delay;       // Exact latency (in frames) introduced by the block
	int min_delay;         // Minimum latency (in frames)
	int max_delay;         // Maximum latency (in frames)
	int approximate_delay; // Approximate latency (in frames)
} blink_EffectInstanceInfo;

typedef blink_Error(*blink_Effect_Process)(void* proc_data, const blink_EffectBuffer* buffer, const float* in, float* out);

typedef struct
{
	void* proc_data;

	blink_Effect_Process process;
} blink_EffectUnit;

typedef blink_EffectInstanceInfo(*blink_EffectInstance_GetInfo)(void* proc_data);
typedef blink_EffectUnit(*blink_EffectInstance_AddUnit)(void* proc_data);

typedef struct
{
	void* proc_data;

	blink_EffectInstance_GetInfo get_info;
	blink_Instance_StreamInit stream_init;

	// Blockhead will call add_unit() four times per effect block to create a set
	// synchronized effects for the purposes of crossfading between them to
	// avoid clicks.
	//
	// A crossfade between one or more units occurs whenever block data changes
	// or the song loops back to an earlier position. These two sitations may
	// occur simulataneously therefore Blockhead requires four units in total.
	blink_EffectInstance_AddUnit add_unit;
} blink_EffectInstance;

#ifdef BLINK_EXPORT
extern "C"
{
	EXPORTED blink_EffectInstance blink_make_effect_instance();
	EXPORTED blink_Error blink_destroy_effect_instance(blink_EffectInstance instance);
}
#endif
