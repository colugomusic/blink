#pragma once

#include <blink/envelope_spec.hpp>

namespace blink {

struct EnvelopeManipulatorTargetSpec
{
	// At least one must be given
	std::optional<EnvelopeSpec> offset_envelope;
	std::optional<EnvelopeSpec> override_envelope;
};

}
