#pragma once

#include "manipulator_target.hpp"
#include "manipulator_slider_target_spec.hpp"
#include <blink/envelope.hpp>

namespace blink {

class ManipulatorSliderTarget : public ManipulatorTarget
{
public:

	blink_MT_Type get_type() const override { return blink_MT_Type_Slider; }

	ManipulatorSliderTarget(ManipulatorSliderTargetSpec spec)
		: ManipulatorTarget(spec)
		, offset_envelope_(spec.offset_envelope)
		, override_envelope_(spec.override_envelope)
	{
	}

	const Envelope& offset_envelope() const { return offset_envelope_; }
	const Envelope& override_envelope() const { return override_envelope_; }

private:

	Envelope offset_envelope_;
	Envelope override_envelope_;
};

} // blink