#pragma once

#include <blink/envelope.hpp>
#include "manipulator_envelope_target_spec.hpp"

namespace blink {

class ManipulatorEnvelopeTarget
{
public:

	ManipulatorEnvelopeTarget(ManipulatorEnvelopeTargetSpec spec)
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
	blink_ManipulatorEnvelopeTarget api_;
};

}