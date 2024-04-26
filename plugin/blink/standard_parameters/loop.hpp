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
		blink_ParamFlags_IsToggle |
		blink_ParamFlags_ShowInContextMenu |
		blink_ParamFlags_ShowButton |
		blink_ParamFlags_MovesDisplay |
		blink_ParamFlags_IconOnly;
	out.icon = blink_StdIcon_Loop;
	out.default_index = 0;
	out.searcher.binary = search::step_binary;
	out.searcher.forward = search::step_forward;

	return out;
}

} // loop
} // blink
} // std_params