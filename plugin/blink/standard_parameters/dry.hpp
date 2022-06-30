#pragma once

#include "mix.hpp"

namespace blink {
namespace std_params {
namespace dry {

static constexpr auto UUID { BLINK_STD_UUID_DRY };

inline auto envelope_parameter()
{
	auto out{ mix::envelope_parameter() };

	out.uuid = UUID;
	out.name = "Dry";

	return out;
}

inline auto slider_parameter()
{
	auto out{ mix::slider_parameter() };

	out.uuid = UUID;
	out.name = "Dry";

	return out;
}

inline auto envelope_manipulator_target()
{
	return mix::envelope_manipulator_target();
}

} // dry
} // blink
} // std_params