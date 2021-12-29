#pragma once

#include <blink_synth.h>
#include "block_positions.hpp"
#include "unit.hpp"
#include "plugin.hpp"

namespace blink {

class SynthUnit : public Unit
{
public:

	SynthUnit(Instance* instance)
		: Unit(instance)
	{
	}

	virtual ~SynthUnit() {}

	blink_Error synth_process(const blink_SynthBuffer& buffer, const blink_SynthUnitState& unit_state, float* out)
	{
		get_instance()->begin_process(buffer.buffer_id);

		Unit::begin_process(buffer.buffer_id, buffer.positions, unit_state.data_offset);

		return process(buffer, unit_state, out);
	}

private:

	virtual blink_Error process(const blink_SynthBuffer& buffer, const blink_SynthUnitState& unit_state, float* out) = 0;
};

}