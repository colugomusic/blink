#pragma once

#include <blink.h>

namespace blink {

class SliderManipulator
{
public:

	SliderManipulator(const blink_ManipulatorData& data)
		: data_(&data)
	{}

	ml::DSPVector operator()(const ml::DSPVector& in)
	{
		switch (data_->type)
		{
			case blink_ManipulatorType_Envelope:
			{
				return manipulate(data_->envelope, in);
			}

			case blink_ManipulatorType_Slider:
			{
				return manipulate(data_->slider, in);
			}

			default:
			{
				// Slider targets can only be manipulated by envelopes or sliders.
				// If we get here I guess it's a host bug
				assert(false);
				return in;
			}
		}
	}

private:

	template <class ManipulatorData>
	ml::DSPVector manipulate(const ManipulatorData& data, const ml::DSPVector& in)
	{
		switch (data.mode)
		{
			case blink_ManipulatorMode_Offset: return offset(data, in);
			case blink_ManipulatorMode_Override: return override_(data, in);
			default:
			{
				assert(false);
				return in;
			}
		}
	}

	ml::DSPVector offset(const blink_EnvelopeManipulatorData& data, const ml::DSPVector& in)
	{
		// TODO:
		return in;
	}

	ml::DSPVector offset(const blink_SliderManipulatorData& data, const ml::DSPVector& in)
	{
		// TODO:
		return in;
	}

	ml::DSPVector override_(const blink_EnvelopeManipulatorData& data, const ml::DSPVector& in)
	{
		// TODO:
		return in;
	}

	ml::DSPVector override_(const blink_SliderManipulatorData& data, const ml::DSPVector& in)
	{
		// TODO:
		return in;
	}

	const blink_ManipulatorData* data_;
};

class ManipulatorSliderTargetData
{
public:

	ManipulatorSliderTargetData(const blink_ManipulatorTargetData& data)
		: data_(&data)
	{
	}

	ml::DSPVector operator()(ml::DSPVector in)
	{
		for (blink_Index i = 0; i < data_->manipulator_count; i++)
		{
			in = SliderManipulator(data_->manipulators[i])(in);
		}

		return in;
	}

private:

	const blink_ManipulatorTargetData* data_;
};

} // blink