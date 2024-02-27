#pragma once

#include <map>
#include <blink_sampler.h>
#include "instance.hpp"
#include "unit.hpp"

namespace blink {

class SamplerUnit : public Unit {
public:
	SamplerUnit(Instance* instance)
		: Unit(instance)
	{
	}
	virtual ~SamplerUnit() {}
	blink_Error sampler_process(const blink_SamplerBuffer& buffer, const blink_SamplerUnitState& unit_state, float* out) {
		get_instance()->begin_process(buffer.buffer_id); 
		Unit::begin_process(buffer.buffer_id, buffer.positions);
		return process(buffer, unit_state, out);
	}
protected:
	virtual blink_Error process(const blink_SamplerBuffer& buffer, const blink_SamplerUnitState& unit_state, float* out) = 0;
};

}