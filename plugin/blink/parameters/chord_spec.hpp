#pragma once

#include <functional>
#include "parameter_spec.hpp"
#include <blink/searcher_spec.hpp>

namespace blink {

struct ChordSpec : public ParameterSpec
{
	ChordSearcherSpec searcher;
	blink_StdIcon icon;
	int flags = 0;
};

}