#pragma once

#pragma warning(push, 0)
#include <DSP/MLDSPOps.h>
#pragma warning(pop)

namespace blink {
namespace dsp {

constexpr auto SILENCE_THRESHOLD = 0.0001f;

template <std::size_t ROWS>
inline bool is_silent(const ml::DSPVectorArray<ROWS>& v, float threshold = SILENCE_THRESHOLD)
{
	for (int row = 0; row < ROWS; row++)
	{
		if (ml::sum(v.constRow(row) > threshold)) return true;
	}

	return false;
}

template <std::size_t ROWS>
inline int find_first_non_silent(const ml::DSPVectorArray<ROWS>& v, float threshold = SILENCE_THRESHOLD)
{
	for (int i = 0; i < kFloatsPerDSPVector; i++)
	{
		for (int row = 0; row < ROWS; row++)
		{
			if (v.constRow(row)[i] > threshold) return i;
		}
	}

	return -1;
}

}
}