#pragma once 

#include "blink.h"

#define BLINK_EFFECT

typedef struct {
	blink_BufferID buffer_id;
	blink_SR song_rate;
	blink_Position* positions;
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

struct blink_EffectUnitState
{
	uint64_t id; // Increments by at least one every time the unit state changes.
	             // The same unit state might be passed in for multiple audio buffers, i.e.
	             // this id might not change from one buffer to the next.
	             // Could be used to optimize things like envelope traversals, for example
	             // if the parameter state has not changed since the last audio buffer then
	             // a plugin could continue searching from the previously hit envelope
	             // point instead of searching from the beginning

	float scale;
	int64_t data_offset;
	// May be NULL, in which case plugins should act as if all parameters are default.
	const blink_ParamData* param_data;
};

#ifdef BLINK_EXPORT
extern "C"
{
	EXPORTED blink_Error              blink_effect_process(blink_UnitIdx unit_idx, const blink_EffectBuffer* buffer, const blink_EffectUnitState* unit_state, const float* in, float* out);
	EXPORTED blink_EffectInstanceInfo blink_effect_get_info(blink_InstanceIdx instance_idx);
}
#endif
