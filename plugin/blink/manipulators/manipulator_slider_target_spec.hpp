#pragma once

#include <tweak/tweak.hpp>
#include <blink/envelope_spec.hpp>
#include "manipulator_target_spec.hpp"

namespace blink {

struct ManipulatorSliderTargetSpec : public ManipulatorTargetSpec
{
	float default_value;
	EnvelopeSpec offset_envelope;
	EnvelopeSpec override_envelope;
};

}
