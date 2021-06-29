#pragma once

#include <blink_synth.h>
#include "block_positions.hpp"
#include "generator_base.hpp"
#include "plugin.hpp"

namespace blink {

class Synth : public GeneratorBase
{
public:

	Synth(Plugin* plugin, int instance_group)
		: GeneratorBase(instance_group)
		, plugin_(plugin)
	{
	}

	virtual ~Synth() {}

	blink_Error synth_process(const blink_SynthBuffer* buffer, float* out)
	{
		plugin_->begin_process(buffer->buffer_id, get_instance_group());

		GeneratorBase::begin_process(buffer->buffer_id, buffer->positions, buffer->data_offset);

		return process(buffer, out);
	}

private:

	virtual blink_Error process(const blink_SynthBuffer* buffer, float* out) = 0;

	Plugin* plugin_;
};

}