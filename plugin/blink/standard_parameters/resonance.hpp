#pragma once

#include <tweak/tweak.hpp>
#include <tweak/std.hpp>
#include <blink/math.hpp>
#include <blink/envelope_parameter_spec.hpp>
#include "percentage.hpp"

namespace blink {
namespace std_params {
namespace resonance {

inline EnvelopeParameterSpec envelope_parameter()
{
	EnvelopeParameterSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_FILTER_RESONANCE;
	out.name = "Resonance";
	out.envelope = percentage::envelope();

	return out;
}

} // resonance
} // blink
} // std_params