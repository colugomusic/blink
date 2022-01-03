#pragma once

#include <blink/envelope_spec.hpp>

namespace blink {

struct EnvelopeManipulatorTargetSpec
{
	EnvelopeSpec offset_envelope;
	EnvelopeSpec override_envelope;
};

}
