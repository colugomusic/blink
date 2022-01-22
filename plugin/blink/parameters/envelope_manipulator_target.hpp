#pragma once

#include <blink/envelope.hpp>
#include "envelope_manipulator_target_spec.hpp"

namespace blink {

class EnvelopeManipulatorTarget
{
public:

	EnvelopeManipulatorTarget(EnvelopeManipulatorTargetSpec spec)
	{
		api_.proc_data = this;
		api_.offset_envelope = nullptr;
		api_.override_envelope = nullptr;
		api_.apply_offset = nullptr;

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

		if (spec.apply_offset)
		{
			apply_offset_ = spec.apply_offset;

			api_.apply_offset = [](void* proc_data, float value, float offset)
			{
				auto self { (EnvelopeManipulatorTarget*)(proc_data) };

				return self->apply_offset_(value, offset);
			};
		}
	}

	auto api() const { return &api_; }

private:


	std::optional<Envelope> offset_;
	std::optional<Envelope> override_;
	std::function<float(float, float)> apply_offset_;
	blink_Envelope offset_api_ { 0 };
	blink_Envelope override_api_ { 0 };
	blink_EnvelopeManipulatorTarget api_;
};

}