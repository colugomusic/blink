#pragma once

#include "block_positions.hpp"
#include "envelope_spec.hpp"
#include "envelope_range.hpp"
#include "envelope_snap_settings.hpp"
#include <blink/math.hpp>
#include <blink/searcher.hpp>

namespace blink {

class Envelope
{
public:

	const EnvelopeSpec spec;
	const EnvelopeRange range;
	const Slider<float> value_slider;
	const float default_value;
	const EnvelopeSnapSettings snap_settings;
	const RealSearcher searcher;

	Envelope(EnvelopeSpec spec);

	const char* display_value(float value) const;
	float stepify(float value) const { return spec.stepify ? spec.stepify(value) : value; }

	float snap_value(float value, float step_size, float snap_amount) const
	{
		return spec.snap_value ? spec.snap_value(value, step_size, snap_amount) : stepify(value);
	}

	std::optional<float> get_gridline(int index) const;
	std::optional<float> get_stepline(int index, float step_size) const;

	blink_Envelope bind() const;

private:

	mutable std::string display_value_buffer_;
};

inline std::optional<float> Envelope::get_gridline(int index) const
{
	if (!spec.get_gridline) return std::optional<float>();

	return spec.get_gridline(index);
}

inline std::optional<float> Envelope::get_stepline(int index, float step_size) const
{
	if (!spec.get_stepline) return std::optional<float>();

	return spec.get_stepline(index, step_size);
}

inline Envelope::Envelope(EnvelopeSpec spec_)
	: spec { spec_ }
	, range { spec.range }
	, snap_settings { spec.step_size, spec.default_snap_amount  }
	, value_slider { spec.value_slider }
	, default_value { spec.default_value }
	, searcher { spec.searcher, spec.default_value }
{
}

inline const char* Envelope::display_value(float value) const
{
	return (display_value_buffer_ = spec.to_string(value)).c_str();
}

inline blink_Envelope Envelope::bind() const
{
	blink_Envelope out;

	out.default_value = default_value;
	out.show_grid_labels = spec.show_grid_labels;
	out.proc_data = (void*)(this);
	out.snap_settings = snap_settings.bind();
	out.value_slider = Slider<float>::bind(value_slider);
	out.min_slider = Slider<float>::bind(range.min());
	out.max_slider = Slider<float>::bind(range.max());

	out.get_gridline = [](void* proc_data, int index, float* out)
	{
		auto envelope = (Envelope*)(proc_data);

		auto result = envelope->get_gridline(index);

		if (result)
		{
			*out = *result;
			return blink_Bool(BLINK_TRUE);
		}

		return blink_Bool(BLINK_FALSE);
	};

	out.get_stepline = [](void* proc_data, int index, float step_size, float* out)
	{
		auto envelope = (Envelope*)(proc_data);

		auto result = envelope->get_stepline(index, step_size);

		if (result)
		{
			*out = *result;
			return blink_Bool(BLINK_TRUE);
		}

		return blink_Bool(BLINK_FALSE);
	};

	out.search = [](void* proc_data, const blink_EnvelopeData* data, float block_position)
	{
		auto envelope = (Envelope*)(proc_data);

		return envelope->searcher.search(data->points, block_position);
	};

	out.display_value = [](void* proc_data, float value)
	{
		auto envelope = (Envelope*)(proc_data);

		return envelope->display_value(value);
	};

	out.stepify = [](void* proc_data, float value)
	{
		auto envelope = (Envelope*)(proc_data);

		return envelope->stepify(value);
	};

	out.snap_value = [](void* proc_data, float value, float step_size, float snap_amount)
	{
		auto envelope = (Envelope*)(proc_data);

		return envelope->snap_value(value, step_size, snap_amount);
	};

	return out;
}

}