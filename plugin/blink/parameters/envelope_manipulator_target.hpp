#pragma once

#include <blink/envelope.hpp>
#include "envelope_manipulator_target_spec.hpp"

namespace blink {

class EnvelopeManipulatorTarget
{
public:

	EnvelopeManipulatorTarget(EnvelopeManipulatorTargetSpec spec)
	{
		api_.offset_envelope = nullptr;
		api_.override_envelope = nullptr;

		if (spec.offset_envelope)
		{
			offset_ = Envelope { *spec.offset_envelope };
			offset_api_ = offset_->bind();
			api_.offset_envelope = &offset_api_;
		}

		if (spec.override_envelope)
		{
			override_ = Envelope { *spec.override_envelope };
			override_api_ = override_->bind();
			api_.override_envelope = &override_api_;
		}
	}

	auto api() const { return &api_; }

private:


	std::optional<Envelope> offset_;
	std::optional<Envelope> override_;
	blink_Envelope offset_api_ { 0 };
	blink_Envelope override_api_ { 0 };
	blink_EnvelopeManipulatorTarget api_;
};

}