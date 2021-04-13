#pragma once

#include <map>
#include <blink_sampler.h>
#include "envelope_spec.hpp"
#include "group.hpp"
#include "parameter.hpp"
#include "sample_data.hpp"
#include "slider_spec.hpp"
#include "generator_base.hpp"

namespace blink {

class Sampler : public GeneratorBase
{

public:

	virtual ~Sampler() {}

	virtual blink_Error process(const blink_SamplerBuffer* buffer, float* out) = 0;

protected:

	void begin_process(const blink_SamplerBuffer* buffer)
	{
		GeneratorBase::begin_process(buffer->positions, buffer->data_offset);
	}

private:

	std::function<blink_WarpPoints*()> get_warp_point_data_;
};

}