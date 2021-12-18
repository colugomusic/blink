#pragma once

#include <blink/envelope_spec.hpp>

namespace blink {

struct ManipulatorEnvelopeTargetSpec
{
	EnvelopeSpec offset_envelope;
	EnvelopeSpec override_envelope;
};

}
