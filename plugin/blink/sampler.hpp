#pragma once

#include <map>
#include <blink_sampler.h>
#include "envelope_spec.hpp"
#include "group.hpp"
#include "parameter.hpp"
#include "sample_data.hpp"
#include "slider_spec.hpp"
#include "generator_base.hpp"
#include "plugin.hpp"

namespace blink {

class Sampler : public GeneratorBase
{

public:

	Sampler(Plugin* plugin, int instance_group)
		: GeneratorBase(instance_group)
		, plugin_(plugin)
	{
	}

	virtual ~Sampler() {}

	blink_Error sampler_process(const blink_SamplerBuffer* buffer, float* out)
	{
		plugin_->begin_process(buffer->buffer_id, get_instance_group());

		GeneratorBase::begin_process(buffer->buffer_id, buffer->positions, buffer->data_offset);

		return process(buffer, out);
	}

protected:

	virtual blink_Error process(const blink_SamplerBuffer* buffer, float* out) = 0;

private:

	std::function<blink_WarpPoints*()> get_warp_point_data_;
	Plugin* plugin_;
};

}