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

template <class T>
inline blkhdgen_Range range(const Range<T>& range)
{
	blkhdgen_Range out;

	out.min = range.min;
	out.max = range.max;

	return out;
}

template <class T>
inline blkhdgen_RangeValue range_value(const RangeValue<T>& range_value)
{
	blkhdgen_RangeValue out;

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

inline blkhdgen_Group group(const Group& group)
{
	blkhdgen_Group out;

	out.id = group.get_id();
	out.name = group.get_name();

	return out;
}

inline blkhdgen_Chord chord(ChordParameter& chord)
{
	blkhdgen_Chord out;

	out.parameter_type = blkhdgen_ParameterType_Chord;
	out.proc_data = &chord;

	out.set = [](void* proc_data, blkhdgen_Index note, blkhdgen_Index beg, blkhdgen_Index end, blkhdgen_Bool on) -> blkhdgen_Error
	{
		auto chord = (ChordParameter*)(proc_data);

		return chord->set(note, beg, end, on == BLKHDGEN_TRUE);
	};

	return out;
}

inline blkhdgen_Envelope envelope(EnvelopeParameter& envelope)
{
	blkhdgen_Envelope out;

	out.parameter_type = blkhdgen_ParameterType_Envelope;
	out.proc_data = &envelope;

	out.get_range = [](void* proc_data)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope_range(envelope->range());
	};

	out.normalize = [](void* proc_data, float value)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope->normalize(value);
	};

	out.inverse_normalize = [](void* proc_data, float value)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope->inverse_normalize(value);
	};

	out.display_value = [](void* proc_data, float value)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope->display_value(value);
	};

	return out;
}

inline blkhdgen_Option option(OptionParameter& option)
{
	blkhdgen_Option out;

	out.parameter_type = blkhdgen_ParameterType_Option;
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

inline blkhdgen_Slider slider(SliderParameter& slider)
{
	blkhdgen_Slider out;

	out.parameter_type = blkhdgen_ParameterType_Slider;
	out.proc_data = &slider;
	out.range = range_value(slider.range());

	out.normalize = [](void* proc_data, float value)
	{
		auto slider = (SliderParameter*)(proc_data);

		return slider->normalize(value);
	};

	out.inverse_normalize = [](void* proc_data, float value)
	{
		auto slider = (SliderParameter*)(proc_data);

		return slider->inverse_normalize(value);
	};

	out.display_value = [](void* proc_data, float value)
	{
		auto slider = (SliderParameter*)(proc_data);

		return slider->display_value(value);
	};

	out.set = [](void* proc_data, float value)
	{
		auto slider = (SliderParameter*)(proc_data);

		return slider->set(value);
	};

	return out;
}

inline blkhdgen_Toggle toggle(ToggleParameter& toggle)
{
	blkhdgen_Toggle out;

	out.parameter_type = blkhdgen_ParameterType_Toggle;
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
	out.flags = parameter.get_flags();

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
			out.parameter.slider = slider(*static_cast<SliderParameter*>(&parameter));
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

	out.add_warp_point = [](void* proc_data, blkhdgen_IntPosition block_position)
	{
		auto generator = (Generator*)(proc_data);

		return generator->add_warp_point(block_position);
	};

	out.remove_warp_point = [](void* proc_data, blkhdgen_Index index)
	{
		auto generator = (Generator*)(proc_data);

		return generator->remove_warp_point(index);
	};

	out.move_warp_point = [](void* proc_data, blkhdgen_Index index, blkhdgen_IntPosition new_position)
	{
		auto generator = (Generator*)(proc_data);

		return generator->move_warp_point(index, new_position);
	};

	out.clear_warp_points = [](void* proc_data)
	{
		auto generator = (Generator*)(proc_data);

		return generator->clear_warp_points();
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

	out.get_error_string = [](void* proc_data, blkhdgen_Error error)
	{
		auto generator = (Generator*)(proc_data);

		return generator->get_error_string(error);
	};

	out.process = [](void* proc_data, const blkhdgen_Position* pos, float** out)
	{
		auto generator = (Generator*)(proc_data);

		return generator->process(pos, out);
	};

	out.get_mod_value = [](void* proc_data, blkhdgen_Position block_position)
	{
		auto generator = (Generator*)(proc_data);

		return generator->get_mod_value(block_position);
	};

	out.get_waveform_position = [](void* proc_data, blkhdgen_Position block_position)
	{
		auto generator = (Generator*)(proc_data);

		return generator->get_waveform_position(block_position);
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