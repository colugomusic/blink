#pragma once

#include <blink/parameters/toggle_spec.hpp>
#include "search.hpp"

namespace blink {
namespace std_params {
namespace loop {

static constexpr auto UUID { BLINK_STD_UUID_LOOP };

inline ToggleSpec toggle()
{
	ToggleSpec out;

	out.uuid = BLINK_STD_UUID_LOOP;
	out.name = "Loop";
	out.flags = blink_ToggleFlags_ShowInContextMenu | blink_ToggleFlags_ShowButton | blink_ToggleFlags_MovesDisplay | blink_ToggleFlags_IconOnly;
	out.icon = blink_StdIcon_Loop;
	out.default_value = false;
	out.searcher.binary = search::toggle_binary;
	out.searcher.forward = search::toggle_forward;

	return out;
}

} // loop
} // blink
} // std_params