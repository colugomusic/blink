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
};

inline EnvelopeParameter::EnvelopeParameter(EnvelopeParameterSpec spec_)
	: Parameter(spec_)
	, spec(spec_)
	, envelope(spec.envelope)
	, clamp_range(spec.clamp_range)
	, options(spec.options)
	, sliders(spec.sliders)
	, flags(spec.flags)
{
}

}