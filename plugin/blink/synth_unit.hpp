#pragma once

#include <blink_synth.h>
#include "block_positions.hpp"
#include "unit.hpp"
#include "plugin.hpp"

namespace blink {

class SynthUnit : public Unit
{
public:

	SynthUnit(Instance* instance)
		: Unit(instance)
	{
	}

	virtual ~SynthUnit() {}

	blink_Error synth_process(const blink_SynthBuffer* buffer, const blink_ParameterData* parameter_data, float* out)
	{
		get_instance()->begin_process(buffer->buffer_id);

		Unit::begin_process(buffer->buffer_id, buffer->positions, buffer->data_offset);

		return process(buffer, parameter_data, out);
	}

private:

	virtual blink_Error process(const blink_SynthBuffer* buffer, const blink_ParameterData* parameter_data, float* out) = 0;
};

}