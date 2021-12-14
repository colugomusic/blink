#pragma once

#include "parameter.hpp"
#include "envelope.hpp"
#include "envelope_parameter_spec.hpp"

namespace blink {

class EnvelopeParameter : public Parameter
{
public:

	EnvelopeParameter(EnvelopeParameterSpec spec);

	blink_ParameterType get_type() const override { return blink_ParameterType_Envelope; }

	int get_flags() const { return spec_.flags; }

	int get_options_count() const { return int(options_.size()); }
	blink_Index get_option(blink_Index index) const { return options_[index]; }
	int get_sliders_count() const { return int(sliders_.size()); }
	blink_Index get_slider(blink_Index index) const { return sliders_[index]; }

	const Envelope& envelope() const { return envelope_; }

private:

	EnvelopeParameterSpec spec_;
	Envelope envelope_;
	std::vector<blink_Index> options_;
	std::vector<blink_Index> sliders_;
};

inline EnvelopeParameter::EnvelopeParameter(EnvelopeParameterSpec spec)
	: Parameter(spec)
	, spec_(spec)
	, envelope_(spec.envelope)
{
	for (const auto& option_spec : spec.options)
	{
		options_.push_back(option_spec);
	}

	for (const auto& slider_spec : spec.sliders)
	{
		sliders_.push_back(slider_spec);
	}
}

}