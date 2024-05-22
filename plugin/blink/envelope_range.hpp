/*TODELETE:
#pragma once

#include "range_spec.hpp"
#include "slider.hpp"

namespace blink {

class EnvelopeRange
{
public:

	EnvelopeRange(RangeSpec<float> spec)
		:  min_(spec.min)
		, max_(spec.max)
	{
	}

	auto& min() { return min_; }
	auto& max() { return max_; }

	const auto& min() const { return min_;	}
	const auto& max() const { return max_; }

private:

	Slider<float> min_;
	Slider<float> max_;
};

}
*/