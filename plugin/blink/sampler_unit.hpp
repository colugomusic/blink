#pragma once

#include <map>
#include <blink_sampler.h>
#include "instance.hpp"
#include "unit.hpp"

namespace blink {

class SamplerUnit : public Unit
{

public:

	SamplerUnit(Instance* instance)
		: Unit(instance)
	{
	}

	virtual ~SamplerUnit() {}

	blink_Error sampler_process(const blink_SamplerBuffer* buffer, const blink_ParameterData* parameter_data, float* out)
	{
		get_instance()->begin_process(buffer->buffer_id);

		Unit::begin_process(buffer->buffer_id, buffer->positions, buffer->data_offset);

		return process(buffer, parameter_data, out);
	}

protected:

	virtual blink_Error process(const blink_SamplerBuffer* buffer, const blink_ParameterData* parameter_data, float* out) = 0;
};

}