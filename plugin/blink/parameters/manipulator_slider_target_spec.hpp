#pragma once

#include <blink/envelope_spec.hpp>

namespace blink {

struct ManipulatorSliderTargetSpec
{
	EnvelopeSpec offset_envelope;
	EnvelopeSpec override_envelope;
};

}
