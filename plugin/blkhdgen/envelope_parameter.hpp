#pragma once

#include "envelope_range.hpp"
#include "parameter.hpp"

namespace blkhdgen {

class EnvelopeParameter : public Parameter
{
public:
	blkhdgen_ParameterType get_type() const override { return blkhdgen_ParameterType_Envelope; }

	float normalize(float value) const;
	float inverse_normalize(float value) const;
	const char* display_value(float value) const;

	EnvelopeRange& range();
};

}