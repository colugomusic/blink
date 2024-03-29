#pragma once

#include "amp.hpp"
#include "filter_frequency.hpp"
#include "formant.hpp"
#include "linear.hpp"
#include "loop.hpp"
#include "mix.hpp"
#include "ms.hpp"
#include "noise_amount.hpp"
#include "noise_color.hpp"
#include "noise_mode.hpp"
#include "noise_width.hpp"
#include "pan.hpp"
#include "percentage.hpp"
#include "pitch.hpp"
#include "resonance.hpp"
#include "reverse.hpp"
#include "sample_offset.hpp"
#include "scale.hpp"
#include "search.hpp"
#include "speed.hpp"

template <class Spec, class Flags>
auto add_flags(Spec spec, Flags flags)
{
	spec.flags |= flags;

	return spec;
}