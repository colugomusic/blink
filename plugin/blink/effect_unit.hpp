#pragma once

#include <map>
#include <blink_effect.h>
#include "unit.hpp"
#include "instance.hpp"

namespace blink {

class EffectUnit : public Unit
{

public:

	EffectUnit(Instance* instance)
		: Unit(instance)
	{
	}

	virtual ~EffectUnit() {}

	blink_Error effect_process(const blink_EffectBuffer* buffer, const blink_ParameterData* parameter_data, const float* in, float* out)
	{
		get_instance()->begin_process(buffer->buffer_id);

		Unit::begin_process(buffer->buffer_id, buffer->positions, buffer->data_offset);

		return process(buffer, parameter_data, in, out);
	}

	// Called by UI thread
	// Can be overridden to inform Blockhead about latency introduced by buffering
	virtual blink_EffectInstanceInfo get_info() const
	{
		blink_EffectInstanceInfo out;

		// -1 means "unknown"
		out.approximate_delay = -1;
		out.exact_delay = -1;
		out.min_delay = -1;
		out.max_delay = -1;

		return out;
	}

private:

	virtual blink_Error process(const blink_EffectBuffer* buffer, const blink_ParameterData* parameter_data, const float* in, float* out) = 0;
};

}