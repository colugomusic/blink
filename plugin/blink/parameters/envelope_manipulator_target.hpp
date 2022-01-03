#pragma once

#include <blink/envelope.hpp>
#include "envelope_manipulator_target_spec.hpp"

namespace blink {

class EnvelopeManipulatorTarget
{
public:

	EnvelopeManipulatorTarget(EnvelopeManipulatorTargetSpec spec)
		: offset_(spec.offset_envelope)
		, override_(spec.override_envelope)
	{
		api_.offset_envelope = offset_.bind();
		api_.override_envelope = override_.bind();
	}

	auto api() const { return &api_; }

private:


	Envelope offset_;
	Envelope override_;
	blink_EnvelopeManipulatorTarget api_;
};

}