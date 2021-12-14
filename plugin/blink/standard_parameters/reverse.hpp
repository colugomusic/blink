#pragma once

#include <blink/toggle_spec.hpp>

namespace blink {
namespace std_params {
namespace reverse {

inline ToggleSpec toggle()
{
	ToggleSpec out;

	out.uuid = BLINK_STD_UUID_TOGGLE_REVERSE;
	out.name = "Reverse";
	out.flags = blink_ToggleFlags_ShowInContextMenu | blink_ToggleFlags_ShowButton | blink_ToggleFlags_MovesDisplay | blink_ToggleFlags_IconOnly;
	out.icon = blink_StdIcon_Reverse;
	out.default_value = false;

	return out;
}

} // reverse
} // blink
} // std_params