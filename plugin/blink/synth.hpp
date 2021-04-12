#pragma once

#include <blink_synth.h>
#include "generator_base.hpp"

namespace blink {

class Synth : public GeneratorBase
{

public:

	virtual ~Synth() {}

	virtual blink_Error process(const blink_SynthBuffer* buffer, float* out) = 0;
	virtual blink_Error reset() = 0;
};

}