#pragma once

#include <blink/envelope_spec.hpp>
#include <blink/parameters/parameter_spec.hpp>

namespace blink {

struct EnvelopeParameterSpec : public ParameterSpec
{
	EnvelopeSpec envelope;
	std::vector<blink_Index> options;
	std::vector<blink_Index> sliders;
	int flags = 0;
};

}
