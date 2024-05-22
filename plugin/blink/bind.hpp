/* TODELETE:
#pragma once

#include <blink/parameters/chord_parameter.hpp>
#include <blink/parameters/envelope_parameter.hpp>
#include <blink/envelope_range.hpp>
#include "envelope_range.hpp"
#include <blink/parameters/option_parameter.hpp>
#include <blink/parameters/slider_parameter.hpp>
#include <blink/parameters/slider_parameter_spec.hpp>
#include <blink/parameters/group.hpp>
#include <blink/parameters/envelope_manipulator_target.hpp>

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

inline blink_ChordParameter chord(const ChordParameter& chord)
{
	blink_ChordParameter out;

	out.parameter_type = blink_ParameterType_Chord;
	out.icon = chord.icon;
	out.flags = chord.flags;

	return out;
}

inline blink_OptionParameter option(const OptionParameter& option)
{
	blink_OptionParameter out;

	out.parameter_type = option.get_type();
	out.max_index = option.get_max_index();
	out.default_index = option.default_index;
	out.icon = option.icon;
	out.flags = option.flags;
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
	out.flags = envelope_parameter.flags;
	out.clamp_range = envelope_parameter.clamp_range;
	out.icon = blink_StdIcon_None; // TODO

	out.options_count = blink_Index(envelope_parameter.options.size());
	out.sliders_count = blink_Index(envelope_parameter.sliders.size());

	out.get_option = [](void* proc_data, blink_Index index)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope->options[index];
	};

	out.get_slider = [](void* proc_data, blink_Index index)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope->sliders[index];
	};

	out.envelope = envelope_parameter.envelope.bind();
	out.manipulator_settings = envelope_parameter.manipulator_settings();

	return out;
}

inline blink_SliderParameter slider_parameter(const SliderParameter<float>& slider_parameter)
{
	blink_SliderParameter out;

	out.parameter_type = blink_ParameterType_Slider;

	out.proc_data = (void*)(&slider_parameter);
	out.slider = Slider<float>::bind(slider_parameter.slider);
	out.icon = slider_parameter.spec.icon;
	out.flags = slider_parameter.spec.flags;
	out.clamp_range = slider_parameter.clamp_range;
	out.manipulator_settings = slider_parameter.manipulator_settings();

	return out;
}

inline blink_IntSliderParameter slider_parameter(const SliderParameter<int>& slider_parameter)
{
	blink_IntSliderParameter out;

	out.parameter_type = blink_ParameterType_IntSlider;
	out.slider = Slider<float>::bind(slider_parameter.slider);
	out.icon = slider_parameter.spec.icon;
	out.flags = slider_parameter.spec.flags;

	return out;
}

inline blink_Parameter parameter(const Parameter& parameter)
{
	blink_Parameter out;

	out.uuid = parameter.uuid;
	out.group_name = parameter.get_group_name();
	out.name = parameter.get_name();
	out.short_name = parameter.get_short_name();
	out.long_desc = parameter.get_long_desc();
	out.manipulation_delegate = parameter.manipulation_delegate;

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
	}

	return out;
}

}}
*/
