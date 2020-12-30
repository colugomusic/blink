#pragma once

#include "range.hpp"

namespace blkhdgen {

class EnvelopeRangeAttribute
{
public:

	void set(float value);
	float get() const;

	const Range<float>& get_range() const;
};

}