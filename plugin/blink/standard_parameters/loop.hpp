#pragma once

#include <blink/parameters/option_spec.hpp>
#include "search.hpp"

namespace blink {
namespace std_params {
namespace loop {

static constexpr auto UUID { BLINK_STD_UUID_LOOP };

inline OptionSpec toggle()
{
	OptionSpec out;

	out.uuid = BLINK_STD_UUID_LOOP;
	out.name = "Loop";
	out.flags =
		blink_OptionFlags_IsToggle |
		blink_OptionFlags_ShowInContextMenu |
		blink_OptionFlags_ShowButton |
		blink_OptionFlags_MovesDisplay |
		blink_OptionFlags_IconOnly;
	out.icon = blink_StdIcon_Loop;
	out.default_index = 0;
	out.searcher.binary = search::step_binary;
	out.searcher.forward = search::step_forward;

	return out;
}

} // loop
} // blink
} // std_params