#pragma once

namespace blink {
namespace bind {

inline blink_EffectUnit effect_unit(EffectUnit* unit)
{
	blink_EffectUnit out;

	out.proc_data = unit;

	out.process = [](void* proc_data, const blink_EffectBuffer* buffer, const blink_EffectUnitState* unit_state, const float* in, float* out)
	{
		auto unit = (EffectUnit*)(proc_data);

		return unit->effect_process(*buffer, *unit_state, in, out);
	};

	return out;
}

inline blink_EffectInstance effect_instance(EffectInstance* instance)
{
	blink_EffectInstance out;

	out.proc_data = instance;

	out.get_info = [](void* proc_data)
	{
		auto instance = (EffectInstance*)(proc_data);

		return instance->get_info();
	};

	out.add_unit = [](void* proc_data)
	{
		auto instance = (EffectInstance*)(proc_data);

		return effect_unit(instance->add_unit());
	};
	
	out.stream_init = [](void* proc_data, blink_SR SR)
	{
		auto instance = (EffectInstance*)(proc_data);

		instance->stream_init(SR);

		return BLINK_OK;
	};

	return out;
}

}}
