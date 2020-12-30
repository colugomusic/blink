#pragma once

#include "envelope_range_attribute.hpp"
#include "range_spec.hpp"

namespace blkhdgen {

class EnvelopeRange
{
public:

	EnvelopeRange(RangeSpec<float> spec);

	EnvelopeRangeAttribute& min();
	EnvelopeRangeAttribute& max();

	const EnvelopeRangeAttribute& min() const;
	const EnvelopeRangeAttribute& max() const;

private:

	EnvelopeRangeAttribute min_;
	EnvelopeRangeAttribute max_;
};

EnvelopeRange::EnvelopeRange(RangeSpec<float> spec)
	: min_(spec.min_range)
	, max_(spec.max_range)
{

}

}