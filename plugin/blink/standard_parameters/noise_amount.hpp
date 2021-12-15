#pragma once

#include <tweak/tweak.hpp>
#include <tweak/std.hpp>
#include <blink/math.hpp>
#include <blink/parameters/envelope_parameter_spec.hpp>

namespace blink {
namespace std_params {
namespace noise_amount {

inline EnvelopeParameterSpec envelope_parameter()
{
	EnvelopeParameterSpec out;

	out.uuid = BLINK_STD_UUID_NOISE_AMOUNT;
	out.name = "Noise Amount";
	out.short_name = "Amount";
	out.envelope = percentage::envelope();

	return out;
}

} // noise_amount
} // blink
} // std_params