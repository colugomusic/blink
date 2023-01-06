#pragma once

#include <functional>
#include <blink/envelope_spec.hpp>
#include <blink/range_spec.hpp>
#include <blink/parameters/parameter_spec.hpp>

namespace blink {

struct EnvelopeParameterSpec : public ParameterSpec
{
	EnvelopeSpec envelope;
	blink_Range clamp_range;
	std::vector<blink_Index> options;
	std::vector<blink_Index> sliders;
	int flags = 0;
	std::optional<EnvelopeSpec> offset_envelope;
	std::optional<EnvelopeSpec> override_envelope;
	std::function<float(float, float)> apply_offset;

	EnvelopeParameterSpec& operator|(int add_flags)
	{
		flags |= add_flags;

		return *this;
	}
};

}
