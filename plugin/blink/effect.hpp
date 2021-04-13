#pragma once

#include <map>
#include <blink_effect.h>
#include "envelope_spec.hpp"
#include "group.hpp"
#include "parameter.hpp"
#include "sample_data.hpp"
#include "slider_spec.hpp"
#include "generator_base.hpp"

namespace blink {

class Effect : public GeneratorBase
{

public:

	virtual ~Effect() {}

	virtual blink_Error process(const blink_EffectBuffer* buffer, const float* in, float* out) = 0;
	virtual blink_Error reset() = 0;

protected:

	void begin_process(const blink_EffectBuffer* buffer)
	{
		GeneratorBase::begin_process(buffer->positions, buffer->data_offset);
	}
};

}