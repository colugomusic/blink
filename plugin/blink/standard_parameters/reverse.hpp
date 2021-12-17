#pragma once

#include <blink/parameters/toggle_spec.hpp>
#include "search.hpp"

namespace blink {
namespace std_params {
namespace reverse {

inline ToggleSpec toggle()
{
	ToggleSpec out;

	out.uuid = BLINK_STD_UUID_REVERSE;
	out.name = "Reverse";
	out.flags = blink_ToggleFlags_ShowInContextMenu | blink_ToggleFlags_ShowButton | blink_ToggleFlags_MovesDisplay | blink_ToggleFlags_IconOnly;
	out.icon = blink_StdIcon_Reverse;
	out.default_value = false;
	out.searcher.binary = search::toggle_binary;
	out.searcher.forward = search::toggle_forward;

	return out;
}

} // reverse
} // blink
} // std_params