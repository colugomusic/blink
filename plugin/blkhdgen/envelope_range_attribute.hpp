#pragma once

#include "range_value.hpp"

namespace blkhdgen {

class EnvelopeRangeAttribute
{
public:

	void set(float value);
	float get() const;

	const RangeValue<float>& get_range() const;
};

}