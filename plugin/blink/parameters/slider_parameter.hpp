#pragma once

#include "parameter.hpp"
#include <blink/parameters/slider_parameter_spec.hpp>

namespace blink {

template <class T>
class SliderParameter : public Parameter
{
public:

	const SliderParameterSpec<T> spec;
	const Slider<T> slider;
	const blink_Range clamp_range;

	SliderParameter(SliderParameterSpec<T> spec_);

	blink_ParameterType get_type() const override;

	const char* display_value(T value) const;
	auto manipulator_settings() const { return manipulator_settings_; }

private:

	mutable std::string display_value_buffer_;
	std::optional<Envelope> offset_envelope_{};
	std::optional<Envelope> override_envelope_{};
	std::function<float(float, float)> apply_offset_;

	blink_Envelope offset_api_{0};
	blink_Envelope override_api_{0};
	blink_ManipulatorSettings manipulator_settings_{0};
};

template <class T>
SliderParameter<T>::SliderParameter(SliderParameterSpec<T> spec_)
	: Parameter(spec_)
	, spec(spec_)
	, slider(spec.slider)
	, clamp_range(spec.clamp_range)
	, offset_envelope_(spec.offset_envelope)
	, override_envelope_(spec.override_envelope)
	, apply_offset_(spec.apply_offset)
{
	if (offset_envelope_)
	{
		offset_api_ = offset_envelope_->bind();
		manipulator_settings_.offset_envelope = &offset_api_;
	}

	if (override_envelope_)
	{
		override_api_ = override_envelope_->bind();
		manipulator_settings_.override_envelope = &override_api_;
	}

	if (apply_offset_ && (offset_envelope_ || override_envelope_))
	{
		manipulator_settings_.apply_offset = [](void* proc_data, float value, float offset)
		{
			auto self = (SliderParameter*)(proc_data);

			return self->apply_offset_(value, offset);
		};
	}
}

template <>
inline blink_ParameterType SliderParameter<float>::get_type() const
{
	return blink_ParameterType_Slider;
}

template <>
inline blink_ParameterType SliderParameter<int>::get_type() const
{
	return blink_ParameterType_IntSlider;
}

template <class T>
const char* SliderParameter<T>::display_value(T value) const
{
	return (display_value_buffer_ = spec.slider.display_value(value)).c_str();
}

}
