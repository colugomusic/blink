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
		blink_ParamFlags_CanManipulate |
		blink_ParamFlags_MovesDisplay |
		blink_ParamFlags_Hidden;

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
		blink_ParamFlags_CanManipulate |
		blink_ParamFlags_IsToggle |
		blink_ParamFlags_ShowInContextMenu |
		blink_ParamFlags_ShowButton |
		blink_ParamFlags_MovesDisplay |
		blink_ParamFlags_IconOnly;

	out.icon = blink_StdIcon_Reverse;
	out.default_index = 0;
	out.searcher.binary = search::step_binary;
	out.searcher.forward = search::step_forward;

	return out;
}

} // reverse
} // blink
} // std_params