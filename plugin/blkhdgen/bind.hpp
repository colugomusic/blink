#pragma once

#include "chord_parameter.hpp"
#include "envelope_parameter.hpp"
#include "envelope_range.hpp"
#include "envelope_range_attribute.hpp"
#include "option_parameter.hpp"
#include "slider_parameter.hpp"
#include "toggle_parameter.hpp"
#include "generator.hpp"
#include "group.hpp"

namespace blkhdgen {
namespace bind {

inline blkhdgen_Range range(const Range<float>& range)
{
	blkhdgen_Range out;

	out.min = range.min;
	out.max = range.max;

	return out;
}

inline blkhdgen_IntRange range(const Range<int>& range)
{
	blkhdgen_IntRange out;

	out.min = range.min;
	out.max = range.max;

	return out;
}

inline blkhdgen_RangeValue range_value(const RangeValue<float>& range_value)
{
	blkhdgen_RangeValue out;

	out.range = range(range_value.range);
	out.default_value = range_value.value;
	out.step_size = range_value.step_size;

	return out;
}

inline blkhdgen_IntRangeValue range_value(const RangeValue<int>& range_value)
{
	blkhdgen_IntRangeValue out;

	out.range = range(range_value.range);
	out.default_value = range_value.value;
	out.step_size = range_value.step_size;

	return out;
}

inline blkhdgen_EnvelopeRangeAttribute envelope_range_attribute(EnvelopeRangeAttribute& attribute)
{
	blkhdgen_EnvelopeRangeAttribute out;

	out.proc_data = &attribute;
	out.range = range_value(attribute.get_range());

	out.set = [](void* proc_data, float value)
	{
		auto attribute = (EnvelopeRangeAttribute*)(proc_data);

		attribute->set(value);
	};

	out.get = [](void* proc_data)
	{
		auto attribute = (EnvelopeRangeAttribute*)(proc_data);

		return attribute->get();
	};

	return out;
}

inline blkhdgen_EnvelopeRange envelope_range(EnvelopeRange& range)
{
	blkhdgen_EnvelopeRange out;

	out.min = envelope_range_attribute(range.min());
	out.min = envelope_range_attribute(range.max());

	return out;
}

inline blkhdgen_EnvelopeSnapSettings envelope_snap_settings(const EnvelopeSnapSettings& snap_settings)
{
	blkhdgen_EnvelopeSnapSettings out;

	out.step_size = range_value(snap_settings.step_size);
	out.default_snap_amount = snap_settings.default_snap_amount;

	return out;
}

inline blkhdgen_Group group(const Group& group)
{
	blkhdgen_Group out;

	out.id = group.id;
	out.name = group.name.c_str();

	return out;
}

inline blkhdgen_Chord chord(ChordParameter& chord)
{
	blkhdgen_Chord out;

	out.parameter_type = blkhdgen_ParameterType_Chord;
	out.proc_data = &chord;

	out.set_get_chord_data_cb = [](void* proc_data, void* user, blkhdgen_GetChordDataCB cb) -> blkhdgen_Error
	{
		auto chord = (ChordParameter*)(proc_data);

		return chord->set_get_chord_data_cb(user, cb);
	};

	return out;
}

inline blkhdgen_Envelope envelope(EnvelopeParameter& envelope)
{
	blkhdgen_Envelope out;

	out.parameter_type = blkhdgen_ParameterType_Envelope;
	out.default_value = envelope.get_default_value();
	out.flags = envelope.get_flags();
	out.proc_data = &envelope;

	out.get_range = [](void* proc_data)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope_range(envelope->range());
	};

	out.get_snap_settings = [](void* proc_data)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope_snap_settings(envelope->snap_settings());
	};

	out.curve = [](void* proc_data, float value)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope->curve(value);
	};

	out.inverse_curve = [](void* proc_data, float value)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope->inverse_curve(value);
	};

	out.display_value = [](void* proc_data, float value)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope->display_value(value);
	};

	out.set_get_point_data_cb = [](void* proc_data, void* user, blkhdgen_GetPointDataCB cb)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope->set_get_point_data_cb(user, cb);
	};

	return out;
}

inline blkhdgen_Option option(OptionParameter& option)
{
	blkhdgen_Option out;

	out.parameter_type = blkhdgen_ParameterType_Option;
	out.default_value = option.get_default_value();
	out.proc_data = &option;

	out.set = [](void* proc_data, blkhdgen_Index index)
	{
		auto option = (OptionParameter*)(proc_data);

		return option->set(index);
	};

	out.get_text = [](void* proc_data, blkhdgen_Index index)
	{
		auto option = (OptionParameter*)(proc_data);

		return option->get_text(index);
	};

	return out;
}

inline blkhdgen_Slider slider(SliderParameter<float>& slider)
{
	blkhdgen_Slider out;

	out.parameter_type = blkhdgen_ParameterType_Slider;
	out.proc_data = &slider;
	out.range = range_value(slider.range());

	out.curve = [](void* proc_data, float value)
	{
		auto slider = (SliderParameter<float>*)(proc_data);

		return slider->curve(value);
	};

	out.inverse_curve = [](void* proc_data, float value)
	{
		auto slider = (SliderParameter<float>*)(proc_data);

		return slider->inverse_curve(value);
	};

	out.display_value = [](void* proc_data, float value)
	{
		auto slider = (SliderParameter<float>*)(proc_data);

		return slider->display_value(value);
	};

	out.set = [](void* proc_data, float value)
	{
		auto slider = (SliderParameter<float>*)(proc_data);

		return slider->set(value);
	};

	return out;
}

inline blkhdgen_IntSlider slider(SliderParameter<int>& slider)
{
	blkhdgen_IntSlider out;

	out.parameter_type = blkhdgen_ParameterType_Slider;
	out.proc_data = &slider;
	out.range = range_value(slider.range());

	out.display_value = [](void* proc_data, int value)
	{
		auto slider = (SliderParameter<int>*)(proc_data);

		return slider->display_value(value);
	};

	out.set = [](void* proc_data, int value)
	{
		auto slider = (SliderParameter<int>*)(proc_data);

		return slider->set(value);
	};

	return out;
}

inline blkhdgen_Toggle toggle(ToggleParameter& toggle)
{
	blkhdgen_Toggle out;

	out.parameter_type = blkhdgen_ParameterType_Toggle;
	out.default_value = toggle.get_default_value() ? BLKHDGEN_TRUE : BLKHDGEN_FALSE;
	out.flags = toggle.get_flags();
	out.proc_data = &toggle;

	out.set = [](void* proc_data, blkhdgen_Bool on)
	{
		auto toggle = (ToggleParameter*)(proc_data);

		return toggle->set(on == BLKHDGEN_TRUE);
	};

	return out;
}

inline blkhdgen_Parameter parameter(Parameter& parameter)
{
	blkhdgen_Parameter out;

	out.uuid = parameter.get_uuid();
	out.group_id = parameter.get_group_id();
	out.name = parameter.get_name();

	const auto type = parameter.get_type();

	switch (type)
	{
		case blkhdgen_ParameterType_Chord:
		{
			out.parameter.chord = chord(*static_cast<ChordParameter*>(&parameter));
			break;
		}

		case blkhdgen_ParameterType_Envelope:
		{
			out.parameter.envelope = envelope(*static_cast<EnvelopeParameter*>(&parameter));
			break;
		}

		case blkhdgen_ParameterType_Option:
		{
			out.parameter.option = option(*static_cast<OptionParameter*>(&parameter));
			break;
		}

		case blkhdgen_ParameterType_Slider:
		{
			out.parameter.slider = slider(*static_cast<SliderParameter<float>*>(&parameter));
			break;
		}

		case blkhdgen_ParameterType_IntSlider:
		{
			out.parameter.int_slider = slider(*static_cast<SliderParameter<int>*>(&parameter));
			break;
		}

		case blkhdgen_ParameterType_Toggle:
		{
			out.parameter.toggle = toggle(*static_cast<ToggleParameter*>(&parameter));
			break;
		}
	}

	return out;
}

inline blkhdgen_Generator generator(Generator* generator)
{
	blkhdgen_Generator out;

	out.proc_data = generator;
	out.name = generator->get_name();
	out.num_channels = generator->get_num_channels();
	out.num_groups = generator->get_num_groups();
	out.num_parameters = generator->get_num_parameters();

	out.set_get_warp_point_data_cb = [](void* proc_data, void* user, blkhdgen_GetWarpPointDataCB cb)
	{
		auto generator = (Generator*)(proc_data);

		return generator->set_get_warp_point_data_cb(user, cb);
	};

	out.set_get_manipulator_data_cb = [](void* proc_data, void* user, blkhdgen_GetManipulatorDataCB cb)
	{
		auto generator = (Generator*)(proc_data);

		return generator->set_get_manipulator_data_cb(user, cb);
	};

	out.get_group = [](void* proc_data, blkhdgen_Index index)
	{
		auto generator = (Generator*)(proc_data);

		return group(generator->get_group(index));
	};

	out.get_group_by_id = [](void* proc_data, blkhdgen_ID id)
	{
		auto generator = (Generator*)(proc_data);

		return group(generator->get_group_by_id(id));
	};

	out.get_parameter = [](void* proc_data, blkhdgen_Index index)
	{
		auto generator = (Generator*)(proc_data);

		return parameter(generator->get_parameter(index));
	};

	out.get_parameter_by_id = [](void* proc_data, blkhdgen_UUID uuid)
	{
		auto generator = (Generator*)(proc_data);

		return parameter(generator->get_parameter_by_id(uuid));
	};

	out.set_data_offset = [](void* proc_data, int offset)
	{
		auto generator = (Generator*)(proc_data);

		generator->set_data_offset(offset);

		return BLKHDGEN_OK;
	};

	out.get_error_string = [](void* proc_data, blkhdgen_Error error)
	{
		auto generator = (Generator*)(proc_data);

		return generator->get_error_string(error);
	};

	out.process = [](void* proc_data, blkhdgen_SR song_rate, blkhdgen_SR sample_rate, const blkhdgen_Position* pos, float** out)
	{
		auto generator = (Generator*)(proc_data);

		return generator->process(song_rate, sample_rate, pos, out);
	};

	out.get_waveform_positions = [](void* proc_data, const blkhdgen_Position* block_positions, float* out, float* derivatives)
	{
		auto generator = (Generator*)(proc_data);

		return generator->get_waveform_positions(block_positions, out, derivatives);
	};

	out.set_get_sample_data_cb = [](void* proc_data, void* user, blkhdgen_GetSampleDataCB cb)
	{
		auto generator = (Generator*)(proc_data);

		return generator->set_get_sample_data_cb(user, cb);
	};

	out.set_get_sample_info_cb = [](void* proc_data, void* user, blkhdgen_GetSampleInfoCB cb)
	{
		auto generator = (Generator*)(proc_data);

		return generator->set_get_sample_info_cb(user, cb);
	};

	return out;
}

template <class GeneratorType>
blkhdgen_Generator make_generator()
{
	return bind::generator(new GeneratorType());
}

blkhdgen_Error destroy_generator(blkhdgen_Generator generator)
{
	delete (Generator*)(generator.proc_data);

	return BLKHDGEN_OK;
}

}}