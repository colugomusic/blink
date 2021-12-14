#pragma once

#include "envelope_spec.hpp"

namespace blink {

struct EnvelopeParameterSpec : public ParameterSpec
{
	EnvelopeSpec envelope;
	std::vector<blink_Index> options;
	std::vector<blink_Index> sliders;
	int flags = 0;
};

}
