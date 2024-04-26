#pragma once

#include <tweak/tweak.hpp>
#include <tweak/std/percentage.hpp>
#include <blink/math.hpp>
#include <blink/parameters/envelope_parameter_spec.hpp>

namespace blink {
namespace std_params {
namespace noise_amount {

static constexpr auto UUID { BLINK_STD_UUID_NOISE_AMOUNT };

inline EnvelopeParameterSpec envelope_parameter()
{
	EnvelopeParameterSpec out;

	out.uuid = BLINK_STD_UUID_NOISE_AMOUNT;
	out.name = "Noise Amount";
	out.short_name = "Amount";
	out.envelope = percentage::envelope();
	out.clamp_range = { 0.0f, 1.0f };

	out.flags |= blink_ParamFlags_HostClamp;
	out.flags |= blink_ParamFlags_CanManipulate;
	out.offset_envelope = percentage::bipolar::envelope();
	out.override_envelope = percentage::envelope();

	return out;
}

} // noise_amount
} // blink
} // std_params