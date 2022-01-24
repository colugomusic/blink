#pragma once

#include <tweak/tweak.hpp>
#include <blink/slider_spec.hpp>
#include <blink/searcher.hpp>

namespace blink {

template <class T>
class Slider
{
public:

	const SliderSpec<T> spec;
	const tweak::Tweaker<T> tweaker;
	const FloatSearcher searcher;

	Slider(SliderSpec<T> spec_);

	const char* display_value(T value) const;

	static blink_Slider bind(const Slider<float>& slider);
	static blink_IntSlider bind(const Slider<int>& slider);

private:

	mutable std::string display_value_buffer_;
};

template <class T>
Slider<T>::Slider(SliderSpec<T> spec_)
	: spec(spec_)
	, tweaker(tweak::Spec<T>(spec))
	, searcher(spec.searcher, float(spec.default_value))
{
}

template <class T>
const char* Slider<T>::display_value(T value) const
{
	display_value_buffer_ = tweaker.to_string(value);

	return display_value_buffer_.c_str();
}

template <class T>
inline blink_Slider Slider<T>::bind(const Slider<float>& slider)
{
	blink_Slider out = { 0 };

	const auto& spec { slider.spec };

	out.default_value = spec.default_value;

	const auto no_control = !spec.constrain || !spec.decrement || !spec.to_string || !spec.drag || !spec.from_string || !spec.increment;

	if (!no_control)
	{
		out.proc_data = (void*)(&slider);

		out.constrain = [](void* proc_data, float value)
		{
			auto slider = (Slider<float>*)(proc_data);

			return slider->tweaker.constrain(value);
		};

		out.increment = [](void* proc_data, float value, blink_Bool precise)
		{
			auto slider = (Slider<float>*)(proc_data);

			return slider->tweaker.increment(value, precise);
		};

		out.decrement = [](void* proc_data, float value, blink_Bool precise)
		{
			auto slider = (Slider<float>*)(proc_data);

			return slider->tweaker.decrement(value, precise);
		};

		out.drag = [](void* proc_data, float value, int amount, blink_Bool precise)
		{
			auto slider = (Slider<float>*)(proc_data);

			return slider->tweaker.drag(value, amount, precise);
		};

		out.display_value = [](void* proc_data, float value)
		{
			auto slider = (Slider<float>*)(proc_data);

			return slider->display_value(value);
		};

		out.from_string = [](void* proc_data, const char* str, float* value)
		{
			auto slider = (Slider<float>*)(proc_data);

			auto result = slider->tweaker.from_string(str);

			if (result)
			{
				*value = *result;
				return blink_Bool(BLINK_TRUE);
			}

			return blink_Bool(BLINK_FALSE);
		};

		out.stepify = [](void* proc_data, float value)
		{
			auto slider = (Slider<float>*)(proc_data);

			return slider->tweaker.stepify(value);
		};
	}

	return out;
}

template <class T>
inline blink_IntSlider Slider<T>::bind(const Slider<int>& slider)
{
	blink_IntSlider out;

	out.proc_data = (void*)(&slider);
	out.default_value = slider.spec.default_value;

	out.constrain = [](void* proc_data, int value)
	{
		auto slider = (Slider<int>*)(proc_data);

		return slider->tweaker.constrain(value);
	};

	out.increment = [](void* proc_data, int value, blink_Bool precise)
	{
		auto slider = (Slider<int>*)(proc_data);

		return slider->tweaker.increment(value, precise);
	};

	out.decrement = [](void* proc_data, int value, blink_Bool precise)
	{
		auto slider = (Slider<int>*)(proc_data);

		return slider->tweaker.decrement(value, precise);
	};

	out.drag = [](void* proc_data, int value, int amount, blink_Bool precise)
	{
		auto slider = (Slider<int>*)(proc_data);

		return slider->tweaker.drag(value, amount, precise);
	};

	out.display_value = [](void* proc_data, int value)
	{
		auto slider = (Slider<int>*)(proc_data);

		return slider->display_value(value);
	};

	out.from_string = [](void* proc_data, const char* str, int* value)
	{
		auto slider = (Slider<int>*)(proc_data);

		auto result = slider->tweaker.from_string(str);

		if (result)
		{
			*value = *result;
			return blink_Bool(BLINK_TRUE);
		}

		return blink_Bool(BLINK_FALSE);
	};

	return out;
}

}
