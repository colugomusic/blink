#pragma once

#include <blink/parameters/chord_parameter.hpp>
#include <blink/parameters/envelope_parameter.hpp>
#include <blink/envelope_range.hpp>
#include "envelope_range.hpp"
#include <blink/parameters/option_parameter.hpp>
#include <blink/parameters/slider_parameter.hpp>
#include <blink/parameters/slider_parameter_spec.hpp>
#include <blink/parameters/toggle_parameter.hpp>
#include <blink/parameters/group.hpp>
#include <blink/parameters/manipulator_envelope_target.hpp>
#include <blink/parameters/manipulator_slider_target.hpp>

namespace blink {
namespace bind {

blink_Parameter parameter(const Parameter& parameter);

inline blink_IntRange range(const Range<int>& range)
{
	blink_IntRange out;

	out.min = range.min;
	out.max = range.max;

	return out;
}

inline blink_Group group(const Group& group)
{
	blink_Group out;

	out.name = group.name.c_str();

	return out;
}

inline blink_Chord chord(const ChordParameter& chord)
{
	blink_Chord out;

	out.parameter_type = blink_ParameterType_Chord;
	out.icon = chord.icon();
	out.flags = chord.flags();

	return out;
}

inline blink_Option option(const OptionParameter& option)
{
	blink_Option out;

	out.parameter_type = option.get_type();
	out.max_index = option.get_max_index();
	out.default_index = option.get_default_index();
	out.proc_data = (void*)(&option);

	out.get_text = [](void* proc_data, blink_Index index)
	{
		auto option = (const OptionParameter*)(proc_data);

		return option->get_text(index);
	};

	return out;
}

inline blink_EnvelopeParameter envelope_parameter(const EnvelopeParameter& envelope_parameter)
{
	blink_EnvelopeParameter out;

	out.proc_data = (void*)(&envelope_parameter);
	out.parameter_type = blink_ParameterType_Envelope;
	out.flags = envelope_parameter.get_flags();

	out.manipulator_target = const_cast<blink_ManipulatorEnvelopeTarget*>(envelope_parameter.get_manipulator_target_api());
	out.options_count = envelope_parameter.get_options_count();
	out.sliders_count = envelope_parameter.get_sliders_count();

	out.get_option = [](void* proc_data, blink_Index index)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope->get_option(index);
	};

	out.get_slider = [](void* proc_data, blink_Index index)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope->get_slider(index);
	};

	out.envelope = envelope_parameter.envelope().bind();

	return out;
}

inline blink_SliderParameter slider_parameter(const SliderParameter<float>& slider_parameter)
{
	blink_SliderParameter out;

	out.parameter_type = blink_ParameterType_Slider;

	out.manipulator_target = const_cast<blink_ManipulatorSliderTarget*>(slider_parameter.get_manipulator_target_api());
	out.slider = Slider<float>::bind(slider_parameter.slider());
	out.icon = slider_parameter.spec().icon;
	out.flags = slider_parameter.spec().flags;

	return out;
}

inline blink_IntSliderParameter slider_parameter(const SliderParameter<int>& slider_parameter)
{
	blink_IntSliderParameter out;

	out.parameter_type = blink_ParameterType_IntSlider;
	out.slider = Slider<float>::bind(slider_parameter.slider());
	out.icon = slider_parameter.spec().icon;
	out.flags = slider_parameter.spec().flags;

	return out;
}

inline blink_Toggle toggle(const ToggleParameter& toggle)
{
	blink_Toggle out;

	out.parameter_type = blink_ParameterType_Toggle;
	out.default_value = toggle.get_default_value() ? BLINK_TRUE : BLINK_FALSE;
	out.icon = toggle.get_icon();
	out.flags = toggle.get_flags();

	return out;
}

inline blink_Parameter parameter(const Parameter& parameter)
{
	blink_Parameter out;

	out.uuid = parameter.get_uuid();
	out.group_index = parameter.get_group_index();
	out.name = parameter.get_name();
	out.short_name = parameter.get_short_name();
	out.long_desc = parameter.get_long_desc();

	const auto type = parameter.get_type();

	switch (type)
	{
		case blink_ParameterType_Chord:
		{
			out.parameter.chord = chord(*static_cast<const ChordParameter*>(&parameter));
			break;
		}

		case blink_ParameterType_Envelope:
		{
			out.parameter.envelope = envelope_parameter(*static_cast<const EnvelopeParameter*>(&parameter));
			break;
		}

		case blink_ParameterType_Option:
		{
			out.parameter.option = option(*static_cast<const OptionParameter*>(&parameter));
			break;
		}

		case blink_ParameterType_Slider:
		{
			out.parameter.slider = slider_parameter(*static_cast<const SliderParameter<float>*>(&parameter));
			break;
		}

		case blink_ParameterType_IntSlider:
		{
			out.parameter.int_slider = slider_parameter(*static_cast<const SliderParameter<int>*>(&parameter));
			break;
		}

		case blink_ParameterType_Toggle:
		{
			out.parameter.toggle = toggle(*static_cast<const ToggleParameter*>(&parameter));
			break;
		}
	}

	return out;
}

#ifdef BLINK_SAMPLER
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

#endif

#ifdef BLINK_EFFECT
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
#endif

#ifdef BLINK_SYNTH
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
#endif

}}
