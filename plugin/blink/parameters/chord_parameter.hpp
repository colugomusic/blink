#pragma once

#include <blink/bits.hpp>
#include <blink/block_positions.hpp>
#include <blink/math.hpp>
#include <blink/searcher.hpp>
#include "parameter.hpp"
#include "chord_spec.hpp"

#pragma warning(push, 0)
#include <DSP/MLDSPOps.h>
#pragma warning(pop)

namespace blink {

class ChordParameter : public Parameter
{
public:

	const ChordSpec spec;
	const ChordSearcher searcher;
	const blink_StdIcon icon;
	const int flags;

	blink_ParameterType get_type() const override { return blink_ParameterType_Chord; }

	ChordParameter(const ChordSpec& spec_)
		: Parameter(spec_)
		, spec(spec_)
		, searcher(spec.searcher)
		, icon(spec.icon)
		, flags(spec.flags)
	{
	}
};

}