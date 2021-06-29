#pragma once

#include <map>
#include <blink_effect.h>
#include "envelope_spec.hpp"
#include "group.hpp"
#include "parameter.hpp"
#include "sample_data.hpp"
#include "slider_spec.hpp"
#include "generator_base.hpp"
#include "plugin.hpp"

namespace blink {

class Effect : public GeneratorBase
{

public:

	Effect(Plugin* plugin, int instance_group)
		: GeneratorBase(instance_group)
		, plugin_(plugin)
	{
	}

	virtual ~Effect() {}

	blink_Error effect_process(const blink_EffectBuffer* buffer, const float* in, float* out)
	{
		plugin_->begin_process(buffer->buffer_id, get_instance_group());

		GeneratorBase::begin_process(buffer->buffer_id, buffer->positions, buffer->data_offset);

		return process(buffer, in, out);
	}

private:

	virtual blink_Error process(const blink_EffectBuffer* buffer, const float* in, float* out) = 0;

	Plugin* plugin_;
};

}