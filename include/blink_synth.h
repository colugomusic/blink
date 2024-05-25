#pragma once 

#include "blink.h"

#define BLINK_SYNTH

typedef struct {
	blink_BufferID buffer_id;
	blink_SR SR;
	blink_SR song_rate;
	blink_Position* positions;
} blink_SynthBuffer;

struct blink_SynthUnitState
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
	EXPORTED blink_Error blink_synth_process(blink_UnitIdx unit_idx, const blink_SynthBuffer* buffer, const blink_SynthUnitState* unit_state, float* out);
}
#endif