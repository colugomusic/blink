#pragma once

#include <blink/parameters/option_spec.hpp>
#include "search.hpp"

namespace blink {
namespace std_params {
namespace reverse {

inline auto option()
{
	blink::OptionSpec out;

	out.uuid = BLINK_STD_UUID_REVERSE_MODE;
	out.name = "Reverse";
	
	out.flags =
		blink_OptionFlags_CanManipulate |
		blink_OptionFlags_MovesDisplay |
		blink_OptionFlags_Hidden;

	out.default_index = -1;
	out.searcher.binary = search::step_binary;
	out.searcher.forward = search::step_forward;

	out.options = {
		"Mirror",
		"Tape",
		"Slip",
	};

	return out;
}

inline auto toggle()
{
	OptionSpec out;

	out.uuid = BLINK_STD_UUID_REVERSE_TOGGLE;
	out.name = "Reverse";

	out.flags =
		blink_OptionFlags_IsToggle |
		blink_OptionFlags_ShowInContextMenu |
		blink_OptionFlags_ShowButton |
		blink_OptionFlags_MovesDisplay |
		blink_OptionFlags_IconOnly;

	out.icon = blink_StdIcon_Reverse;
	out.default_index = 0;
	out.searcher.binary = search::step_binary;
	out.searcher.forward = search::step_forward;

	// When the user tries to create a manipulator for this parameter,
	// create a manipulator for this other parameter instead
	out.manipulation_delegate = BLINK_STD_UUID_REVERSE_MODE;

	return out;
}

} // reverse
} // blink
} // std_params