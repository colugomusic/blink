#pragma once

#include <tweak/tweak.hpp>
#include <tweak/std.hpp>
#include <blink/math.hpp>
#include <blink/parameters/toggle_spec.hpp>

namespace blink {
namespace std_params {
namespace loop {

inline ToggleSpec toggle()
{
	ToggleSpec out;

	out.uuid = BLINK_STD_UUID_LOOP;
	out.name = "Loop";
	out.flags = blink_ToggleFlags_ShowInContextMenu | blink_ToggleFlags_ShowButton | blink_ToggleFlags_MovesDisplay | blink_ToggleFlags_IconOnly;
	out.icon = blink_StdIcon_Loop;
	out.default_value = false;

	return out;
}

} // loop
} // blink
} // std_params