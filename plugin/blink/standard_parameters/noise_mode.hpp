#pragma once

#include <blink/parameters/option_spec.hpp>

namespace blink {
namespace std_params {
namespace noise_mode {

static constexpr auto UUID { BLINK_STD_UUID_NOISE_MODE };

inline OptionSpec option()
{
	OptionSpec out;

	out.uuid = BLINK_STD_UUID_NOISE_MODE;
	out.name = "Noise Mode";
	out.default_index = 0;
	out.options = {
		"Multiply",
		"Mix",
	};
	out.flags |= blink_ParamFlags_CanManipulate;

	return out;
}

} // noise_mode
} // blink
} // std_params