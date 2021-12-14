#pragma once

#include <blink/chord_spec.hpp>
#include "search.hpp"

namespace blink {
namespace std_params {
namespace scale {

inline ChordSpec chord()
{
	ChordSpec out;

	out.uuid = BLINK_STD_UUID_CHORD_SCALE;
	out.name = "Scale";
	out.icon = blink_StdIcon_PianoRoll;
	out.search_binary = search::chord_binary;
	out.search_forward = search::chord_forward;

	return out;
}

} // scale
} // blink
} // std_params