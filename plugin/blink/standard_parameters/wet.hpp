#pragma once

#include "mix.hpp"

namespace blink {
namespace std_params {
namespace wet {

static constexpr auto UUID { BLINK_STD_UUID_WET };

inline auto envelope_parameter()
{
	auto out{ mix::envelope_parameter() };

	out.uuid = UUID;
	out.name = "Wet";

	return out;
}

inline auto slider_parameter()
{
	auto out{ mix::slider_parameter() };

	out.uuid = UUID;
	out.name = "Wet";

	return out;
}

} // wet
} // blink
} // std_params