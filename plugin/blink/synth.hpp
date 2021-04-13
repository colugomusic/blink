#pragma once

#include <blink_synth.h>
#include "block_positions.hpp"
#include "generator_base.hpp"

namespace blink {

class Synth : public GeneratorBase
{
public:

	virtual ~Synth() {}

	void begin_process(const blink_SynthBuffer* buffer)
	{
		GeneratorBase::begin_process(buffer->positions, buffer->data_offset);
	}

	virtual blink_Error process(const blink_SynthBuffer* buffer, float* out) = 0;
	virtual blink_Error reset() = 0;
};

}