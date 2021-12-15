#pragma once

#include "manipulator_target.hpp"
#include <blink/envelope.hpp>

namespace blink {

class ManipulatorSliderTarget : public ManipulatorTarget
{
public:

	const Envelope& offset_envelope() const { return offset_envelope_; }
	const Envelope& override_envelope() const { return override_envelope_; }

private:

	Envelope offset_envelope_;
	Envelope override_envelope_;
};

} // blink