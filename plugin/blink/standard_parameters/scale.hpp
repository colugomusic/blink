#pragma once

#include <blink/parameters/chord_spec.hpp>
#include "search.hpp"

namespace blink {
namespace std_params {
namespace scale {

static constexpr auto UUID { BLINK_STD_UUID_SCALE };

inline ChordSpec chord()
{
	ChordSpec out;

	out.uuid = UUID;
	out.name = "Scale";
	out.icon = blink_StdIcon_PianoRoll;
	out.searcher.binary = search::chord_binary;
	out.searcher.forward = search::chord_forward;

	return out;
}

} // scale
} // blink
} // std_params