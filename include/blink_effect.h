#pragma once 

#include "blink.h"

typedef struct
{
	blink_SR song_rate;
	blink_SR sample_rate;
	int data_offset;

	blink_Position* positions;
	blink_ParameterData* parameter_data;
} blink_SamplerBuffer;

typedef blink_Error(*blink_Effect_Process)(void* proc_data, const blink_EffectBuffer* buffer, const float* in, float* out);

typedef struct
{
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
