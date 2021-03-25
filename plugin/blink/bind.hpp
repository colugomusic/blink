#pragma once

#include "chord_parameter.hpp"
#include "envelope_parameter.hpp"
#include "envelope_range.hpp"
#include "envelope_range_attribute.hpp"
#include "option_parameter.hpp"
#include "slider_parameter.hpp"
#include "slider_parameter_spec.hpp"
#include "toggle_parameter.hpp"
#include "generator.hpp"
#include "sampler.hpp"
#include "group.hpp"

namespace blink {
namespace bind {

inline blink_IntRange range(const Range<int>& range)
{
	blink_IntRange out;

	out.min = range.min;
	out.max = range.max;

	return out;
}

inline blink_Slider slider(const Slider<float>& slider)
{
	blink_Slider out;

	out.proc_data = (void*)(&slider);
	out.default_value = slider.spec().default_value;

	out.constrain = [](void* proc_data, float value)
	{
		auto slider = (Slider<float>*)(proc_data);

		return slider->spec().constrain(value);
	};

	out.increment = [](void* proc_data, float value, bool precise)
	{
		auto slider = (Slider<float>*)(proc_data);

		return slider->spec().increment(value, precise);
	};

	out.decrement = [](void* proc_data, float value, bool precise)
	{
		auto slider = (Slider<float>*)(proc_data);

		return slider->spec().decrement(value, precise);
	};

	out.drag = [](void* proc_data, float value, int amount, bool precise)
	{
		auto slider = (Slider<float>*)(proc_data);

		return slider->spec().drag(value, amount, precise);
	};

	out.display_value = [](void* proc_data, float value)
	{
		auto slider = (Slider<float>*)(proc_data);

		return slider->display_value(value);
	};

	out.from_string = [](void* proc_data, const char* str, float* value)
	{
		auto slider = (Slider<float>*)(proc_data);

		auto result = slider->spec().from_string(str);

		if (result)
		{
			*value = *result;
			return true;
		}

		return false;
	};

	return out;
}

inline blink_IntSlider slider(const Slider<int>& slider)
{
	blink_IntSlider out;

	out.proc_data = (void*)(&slider);
	out.default_value = slider.spec().default_value;

	out.constrain = [](void* proc_data, int value)
	{
		auto slider = (Slider<int>*)(proc_data);

		return slider->spec().constrain(value);
	};

	out.increment = [](void* proc_data, int value, bool precise)
	{
		auto slider = (Slider<int>*)(proc_data);

		return slider->spec().increment(value, precise);
	};

	out.decrement = [](void* proc_data, int value, bool precise)
	{
		auto slider = (Slider<int>*)(proc_data);

		return slider->spec().decrement(value, precise);
	};

	out.drag = [](void* proc_data, int value, int amount, bool precise)
	{
		auto slider = (Slider<int>*)(proc_data);

		return slider->spec().drag(value, amount, precise);
	};

	out.display_value = [](void* proc_data, int value)
	{
		auto slider = (Slider<int>*)(proc_data);

		return slider->display_value(value);
	};

	out.from_string = [](void* proc_data, const char* str, int* value)
	{
		auto slider = (Slider<int>*)(proc_data);

		auto result = slider->spec().from_string(str);

		if (result)
		{
			*value = *result;
			return true;
		}

		return false;
	};

	return out;
}

inline blink_EnvelopeRange envelope_range(EnvelopeRange& range)
{
	blink_EnvelopeRange out;

	out.min = slider(range.min());
	out.min = slider(range.max());

	return out;
}

inline blink_EnvelopeSnapSettings envelope_snap_settings(const EnvelopeSnapSettings& snap_settings)
{
	blink_EnvelopeSnapSettings out;

	out.step_size = slider(snap_settings.step_size);
	out.default_snap_amount = snap_settings.default_snap_amount;

	return out;
}

inline blink_Group group(const Group& group)
{
	blink_Group out;

	out.name = group.name.c_str();

	return out;
}

inline blink_Chord chord(ChordParameter& chord)
{
	blink_Chord out;

	out.parameter_type = blink_ParameterType_Chord;

	return out;
}

inline blink_Envelope envelope(EnvelopeParameter& envelope)
{
	blink_Envelope out;

	out.parameter_type = blink_ParameterType_Envelope;
	out.default_value = envelope.get_default_value();
	out.flags = envelope.get_flags();
	out.proc_data = &envelope;
	out.snap_settings = envelope_snap_settings(envelope.snap_settings());

	out.display_value = [](void* proc_data, float value)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope->display_value(value);
	};

	return out;
}

inline blink_Option option(OptionParameter& option)
{
	blink_Option out;

	out.parameter_type = blink_ParameterType_Option;
	out.default_value = option.get_default_value();
	out.proc_data = &option;

	out.get_text = [](void* proc_data, blink_Index index)
	{
		auto option = (OptionParameter*)(proc_data);

		return option->get_text(index);
	};

	return out;
}


inline blink_SliderParameter slider_parameter(SliderParameter<float>& slider_parameter)
{
	blink_SliderParameter out;

	out.parameter_type = blink_ParameterType_Slider;

	out.slider = slider(slider_parameter.slider());
	out.icon = slider_parameter.spec().icon;

	return out;
}

inline blink_IntSliderParameter slider_parameter(SliderParameter<int>& slider_parameter)
{
	blink_IntSliderParameter out;

	out.parameter_type = blink_ParameterType_IntSlider;
	out.slider = slider(slider_parameter.slider());
	out.icon = slider_parameter.spec().icon;

	return out;
}

inline blink_Toggle toggle(ToggleParameter& toggle)
{
	blink_Toggle out;

	out.parameter_type = blink_ParameterType_Toggle;
	out.default_value = toggle.get_default_value() ? BLINK_TRUE : BLINK_FALSE;
	out.icon = toggle.get_icon();
	out.flags = toggle.get_flags();

	return out;
}

inline blink_Parameter parameter(Parameter& parameter)
{
	blink_Parameter out;

	out.uuid = parameter.get_uuid();
	out.group_index = parameter.get_group_index();
	out.name = parameter.get_name();

	const auto type = parameter.get_type();

	switch (type)
	{
		case blink_ParameterType_Chord:
		{
			out.parameter.chord = chord(*static_cast<ChordParameter*>(&parameter));
			break;
		}

		case blink_ParameterType_Envelope:
		{
			out.parameter.envelope = envelope(*static_cast<EnvelopeParameter*>(&parameter));
			break;
		}

		case blink_ParameterType_Option:
		{
			out.parameter.option = option(*static_cast<OptionParameter*>(&parameter));
			break;
		}

		case blink_ParameterType_Slider:
		{
			out.parameter.slider = slider_parameter(*static_cast<SliderParameter<float>*>(&parameter));
			break;
		}

		case blink_ParameterType_IntSlider:
		{
			out.parameter.int_slider = slider_parameter(*static_cast<SliderParameter<int>*>(&parameter));
			break;
		}

		case blink_ParameterType_Toggle:
		{
			out.parameter.toggle = toggle(*static_cast<ToggleParameter*>(&parameter));
			break;
		}
	}

	return out;
}

#ifdef BLINK_GENERATOR
inline blink_Generator generator(Generator* generator, const char* name, bool requires_preprocess)
{
	blink_Generator out;

	out.proc_data = generator;
	out.name = name;
	out.num_channels = generator->get_num_channels();
	out.num_groups = generator->get_num_groups();
	out.num_parameters = generator->get_num_parameters();
	out.requires_preprocess = requires_preprocess;

	out.set_get_warp_point_data_cb = [](void* proc_data, void* host, blink_GetWarpPointDataCB cb)
	{
		auto generator = (Generator*)(proc_data);

		return generator->set_get_warp_point_data_cb(host, cb);
	};

	//out.set_get_manipulator_data_cb = [](void* proc_data, void* host, blink_GetManipulatorDataCB cb)
	//{
	//	auto generator = (Generator*)(proc_data);

	//	return generator->set_get_manipulator_data_cb(host, cb);
	//};

	out.get_group = [](void* proc_data, blink_Index index)
	{
		auto generator = (Generator*)(proc_data);

		return group(generator->get_group(index));
	};

	out.get_parameter = [](void* proc_data, blink_Index index)
	{
		auto generator = (Generator*)(proc_data);

		return parameter(generator->get_parameter(index));
	};

	out.get_parameter_by_id = [](void* proc_data, blink_UUID uuid)
	{
		auto generator = (Generator*)(proc_data);

		return parameter(generator->get_parameter_by_id(uuid));
	};

	out.set_data_offset = [](void* proc_data, int offset)
	{
		auto generator = (Generator*)(proc_data);

		generator->set_data_offset(offset);

		return BLINK_OK;
	};

	out.get_error_string = [](void* proc_data, blink_Error error)
	{
		auto generator = (Generator*)(proc_data);

		return generator->get_error_string(error);
	};

	out.process = [](void* proc_data, blink_SR song_rate, blink_SR sample_rate, const blink_Position* pos, float** out)
	{
		auto generator = (Generator*)(proc_data);

		return generator->process(song_rate, sample_rate, pos, out);
	};

	out.get_waveform_positions = [](void* proc_data, const blink_Position* block_positions, float* out, float* derivatives)
	{
		auto generator = (Generator*)(proc_data);

		return generator->get_waveform_positions(block_positions, out, derivatives);
	};

	out.set_get_sample_data_cb = [](void* proc_data, void* host, blink_GetSampleDataCB cb)
	{
		auto generator = (Generator*)(proc_data);

		return generator->set_get_sample_data_cb(host, cb);
	};

	out.set_get_sample_info_cb = [](void* proc_data, void* host, blink_GetSampleInfoCB cb)
	{
		auto generator = (Generator*)(proc_data);

		return generator->set_get_sample_info_cb(host, cb);
	};

	out.preprocess_sample = [](void* proc_data, void* host, blink_PreprocessCallbacks callbacks)
	{
		auto generator = (Generator*)(proc_data);

		return generator->preprocess_sample(host, callbacks);
	};

	return out;
}

template <class GeneratorType>
blink_Generator make_generator()
{
	return bind::generator(new GeneratorType(), GeneratorType::NAME, GeneratorType::REQUIRES_PREPROCESS);
}

blink_Error destroy_generator(blink_Generator generator)
{
	delete (Generator*)(generator.proc_data);

	return BLINK_OK;
}
#endif


#ifdef BLINK_SAMPLER
inline blink_Sampler sampler(Sampler* sampler, bool requires_preprocess)
{
	blink_Sampler out;

	out.proc_data = sampler;

	out.process = [](void* proc_data, const blink_SamplerBuffer* buffer, float* out)
	{
		auto sampler = (Sampler*)(proc_data);

		return sampler->process(buffer, out);
	};

	return out;
}

template <class SamplerType, class ...Args>
blink_Sampler make_sampler(Args... args)
{
	return bind::sampler(new SamplerType(args...), SamplerType::REQUIRES_PREPROCESS);
}

blink_Error destroy_sampler(blink_Sampler sampler)
{
	delete (Sampler*)(sampler.proc_data);

	return BLINK_OK;
}
#endif

}}