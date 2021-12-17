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

	blink_ParameterType get_type() const override { return blink_ParameterType_Chord; }

	ChordParameter(const ChordSpec& spec)
		: Parameter(spec)
		, spec_(spec)
		, searcher_(spec.searcher)
	{
	}

	const auto& search() const { return searcher_; }

	blink_StdIcon icon() const { return spec_.icon; }
	int flags() const { return spec_.flags; }

private:

	ChordSpec spec_;
	ChordSearcher searcher_;
};

}