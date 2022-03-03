#pragma once

namespace blink {
namespace bind {

inline blink_SynthUnit synth_unit(SynthUnit* synth)
{
	blink_SynthUnit out;

	out.proc_data = synth;

	out.process = [](void* proc_data, const blink_SynthBuffer* buffer, const blink_SynthUnitState* unit_state, float* out)
	{
		auto synth = (SynthUnit*)(proc_data);

		return synth->synth_process(*buffer, *unit_state, out);
	};

	return out;
}

inline blink_SynthInstance synth_instance(SynthInstance* instance)
{
	blink_SynthInstance out;

	out.proc_data = instance;

	out.add_unit = [](void* proc_data)
	{
		auto instance = (SynthInstance*)(proc_data);

		return synth_unit(instance->add_unit());
	};
	
	out.stream_init = [](void* proc_data, blink_SR SR)
	{
		auto instance = (SynthInstance*)(proc_data);

		instance->stream_init(SR);

		return BLINK_OK;
	};

	return out;
}

}}
