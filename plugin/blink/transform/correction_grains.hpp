#pragma once

#include <cassert>
#pragma warning(push, 0)
#include <DSP/MLDSPOps.h>
#pragma warning(pop)

namespace blink {
namespace transform {

struct CorrectionGrains
{
	int count {};
	ml::DSPVectorInt buffer_index;
	ml::DSPVector ff;
	ml::DSPVector length;

	struct Grain
	{
		int buffer_index;
		float ff;
		float length;
	};

	void push(Grain && grain)
	{
		assert(count < kFloatsPerDSPVector);

		buffer_index[count] = grain.buffer_index;
		ff[buffer_index[count]] = grain.ff;
		length[buffer_index[count]] = grain.length;

		count++;
	}
};

} // transform
} // blink
