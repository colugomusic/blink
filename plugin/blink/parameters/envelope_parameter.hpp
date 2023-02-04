#pragma once

#include <cassert>
#include <memory>
#include <optional>
#include "parameter.hpp"
#include <blink/envelope.hpp>
#include "envelope_parameter_spec.hpp"

namespace blink {

class EnvelopeParameter : public Parameter
{
public:

	const EnvelopeParameterSpec spec;
	const Envelope envelope;
	const blink_Range clamp_range;
	const std::vector<blink_Index> options;
	const std::vector<blink_Index> sliders;
	const int flags;

	EnvelopeParameter(EnvelopeParameterSpec spec_);

	blink_ParameterType get_type() const override { return blink_ParameterType_Envelope; }

	auto manipulator_settings() const { return manipulator_settings_; }

private:

	std::optional<Envelope> offset_envelope_{};
	std::optional<Envelope> override_envelope_{};
	std::function<float(float, float)> apply_offset_;

	blink_Envelope offset_api_{0};
	blink_Envelope override_api_{0};
	blink_ManipulatorSettings manipulator_settings_{0};
};

inline EnvelopeParameter::EnvelopeParameter(EnvelopeParameterSpec spec_)
	: Parameter(spec_)
	, spec(spec_)
	, envelope(spec.envelope)
	, offset_envelope_(spec.offset_envelope)
	, override_envelope_(spec.override_envelope)
	, apply_offset_(spec.apply_offset)
	, clamp_range(spec.clamp_range)
	, options(spec.options)
	, sliders(spec.sliders)
	, flags(spec.flags)
{
	if (offset_envelope_)
	{
		offset_api_ = offset_envelope_->bind();
		manipulator_settings_.offset_envelope = &offset_api_;
	}

	if (override_envelope_)
	{
		override_api_ = override_envelope_->bind();
		manipulator_settings_.override_envelope = &override_api_;
	}

	if (apply_offset_)
	{
		manipulator_settings_.apply_offset = [](void* proc_data, float value, float offset)
		{
			auto self = (EnvelopeParameter*)(proc_data);

			return self->apply_offset_(value, offset);
		};
	}
}

}