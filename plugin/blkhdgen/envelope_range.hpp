#pragma once

#include "envelope_range_attribute.hpp"
#include "range_spec.hpp"

namespace blkhdgen {

class EnvelopeRange
{
public:

	EnvelopeRange(RangeSpec<float> spec)
		: min_(spec.min_range)
		, max_(spec.max_range)
	{
	}

	EnvelopeRangeAttribute& min() { return min_; }
	EnvelopeRangeAttribute& max() { return max_; }

	const EnvelopeRangeAttribute& min() const { return min_; }
	const EnvelopeRangeAttribute& max() const { return max_; }

private:

	EnvelopeRangeAttribute min_;
	EnvelopeRangeAttribute max_;
};

}