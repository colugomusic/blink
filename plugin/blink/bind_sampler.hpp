#pragma once

namespace blink {
namespace bind {

inline blink_SamplerUnit sampler_unit(SamplerUnit* sampler)
{
	blink_SamplerUnit out;

	out.proc_data = sampler;

	out.process = [](void* proc_data, const blink_SamplerBuffer* buffer, const blink_SamplerUnitState* unit_state, float* out)
	{
		auto sampler = (SamplerUnit*)(proc_data);

		return sampler->sampler_process(*buffer, *unit_state, out);
	};

	return out;
}

inline blink_SamplerInstance sampler_instance(SamplerInstance* instance)
{
	blink_SamplerInstance out;

	out.proc_data = instance;

	out.add_unit = [](void* proc_data)
	{
		auto instance = (SamplerInstance*)(proc_data);

		return sampler_unit(instance->add_unit());
	};

	out.stream_init = [](void* proc_data, blink_SR SR)
	{
		auto instance = (SamplerInstance*)(proc_data);

		instance->stream_init(SR);

		return BLINK_OK;
	};

	return out;
}

}}
