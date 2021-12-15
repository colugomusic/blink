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

	blink_Error sampler_process(const blink_SamplerBuffer* buffer, float* out)
	{
		get_instance()->begin_process(buffer->buffer_id);

		Unit::begin_process(buffer->buffer_id, buffer->positions, buffer->data_offset);

		return process(buffer, out);
	}

protected:

	virtual blink_Error process(const blink_SamplerBuffer* buffer, float* out) = 0;

private:

	std::function<blink_WarpPoints*()> get_warp_point_data_;
};

}