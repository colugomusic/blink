#pragma once

#include <blink/parameters/option_spec.hpp>
#include <blink/parameters/toggle_spec.hpp>
#include "search.hpp"

namespace blink {
namespace std_params {
namespace reverse {

static constexpr auto UUID { BLINK_STD_UUID_REVERSE };

inline auto option()
{
	blink::OptionSpec out;

	out.uuid = UUID;
	out.name = "Reverse";
	
	out.flags =
		blink_OptionFlags_IsManipulatorTarget |
		blink_OptionFlags_MovesDisplay |
		blink_OptionFlags_Hidden;

	out.default_index = -1;

	out.options = {
		"Mirror",
		"Tape",
		"Slip",
	};

	return out;
}

inline auto toggle()
{
	ToggleSpec out;

	out.uuid = UUID;
	out.name = "Reverse";

	out.flags =
		blink_ToggleFlags_CanManipulate |
		blink_ToggleFlags_ShowInContextMenu |
		blink_ToggleFlags_ShowButton |
		blink_ToggleFlags_MovesDisplay |
		blink_ToggleFlags_IconOnly;

	out.icon = blink_StdIcon_Reverse;
	out.default_value = false;
	out.searcher.binary = search::toggle_binary;
	out.searcher.forward = search::toggle_forward;

	return out;
}

} // reverse
} // blink
} // std_params