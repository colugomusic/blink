#pragma once

#include "range_spec.hpp"

namespace blkhdgen {

class EnvelopeRange
{
public:

	EnvelopeRange(RangeSpec<float> spec)
		: spec_(spec)
	{
	}

	auto& min() { return spec_.min; }
	auto& max() { return spec_.max; }

	const auto& min() const { return spec_.min;	}
	const auto& max() const { return spec_.max; }

private:

	RangeSpec<float> spec_;
};

}