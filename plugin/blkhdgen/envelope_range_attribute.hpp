#pragma once

#include <atomic>
#include "range_value.hpp"

namespace blkhdgen {

class EnvelopeRangeAttribute
{
public:

	EnvelopeRangeAttribute(RangeValue<float> range);

	void set(float value)
	{
		current_value_ = value;
	}

	float get() const
	{
		return current_value_;
	}
	{
		return range_;
	}

private:

	RangeValue<float> range_;
	std::atomic<float> current_value_;
};

EnvelopeRangeAttribute::EnvelopeRangeAttribute(RangeValue<float> range)
	: range_(range)
	, current_value_(range_.value)
{
}

}