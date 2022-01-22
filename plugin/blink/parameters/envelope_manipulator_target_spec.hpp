#pragma once

#include <blink/envelope_spec.hpp>

namespace blink {

struct EnvelopeManipulatorTargetSpec
{
	// At least one must be given
	std::optional<EnvelopeSpec> offset_envelope;
	std::optional<EnvelopeSpec> override_envelope;

	// If not set, host will just do (value + offset)
	std::function<float(float value, float offset)> apply_offset;
};

}
