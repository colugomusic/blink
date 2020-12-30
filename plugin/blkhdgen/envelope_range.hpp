#pragma once

#include "envelope_range_attribute.hpp"

namespace blkhdgen {

class EnvelopeRange
{
public:

	EnvelopeRangeAttribute& min();
	EnvelopeRangeAttribute& max();
};

}