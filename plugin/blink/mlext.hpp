#pragma once

#pragma warning(push, 0)
#include <DSP/MLDSPOps.h>
#pragma warning(pop)

namespace blink {
namespace mlext {

// Given vectors [ ?, ?, ?, 3 ], [ 4, 5, 6, 7 ]
// Returns [ 3, 4, 5, 6 ]
inline SIMDVectorFloat vecShuffleRight(SIMDVectorFloat v1, SIMDVectorFloat v2)
{
	return _mm_shuffle_ps(_mm_shuffle_ps(v2, v1, SHUFFLE(3, 3, 0, 0)), v2, SHUFFLE(2, 1, 0, 3));
}

// Given vectors [ 0, 1, 2, 3 ], [ 4, ?, ?, ? ]
// Returns [ 1, 2, 3, 4 ]
inline SIMDVectorFloat vecShuffleLeft(SIMDVectorFloat v1, SIMDVectorFloat v2)
{
	return _mm_shuffle_ps(v1, _mm_shuffle_ps(v1, v2, SHUFFLE(0, 0, 3, 3)), SHUFFLE(3, 0, 2, 1));
}

template <size_t ROWS>
inline ml::DSPVectorArray<ROWS> rotateLeft(const ml::DSPVectorArray<ROWS>& x)
{
	ml::DSPVectorArray<ROWS> vy;

	for (int row = 0; row < ROWS; row++)
	{
		const float* px1 = x.getConstBuffer() + (row * kFloatsPerDSPVector);
		const float* px2 = px1 + kFloatsPerSIMDVector;
		float* py1 = vy.getBuffer() + (row * kFloatsPerDSPVector);

		for (int n = 0; n < kSIMDVectorsPerDSPVector - 1; ++n)
		{
			vecStore(py1, vecShuffleLeft(vecLoad(px1), vecLoad(px2)));

			px1 += kFloatsPerSIMDVector;
			px2 += kFloatsPerSIMDVector;
			py1 += kFloatsPerSIMDVector;
		}

		px2 = x.getConstBuffer() + (row * kFloatsPerDSPVector);

		vecStore(py1, vecShuffleLeft(vecLoad(px1), vecLoad(px2)));
	}


	return vy;
}

template <size_t ROWS>
inline ml::DSPVectorArray<ROWS> rotateRight(const ml::DSPVectorArray<ROWS>& x)
{
	ml::DSPVectorArray<ROWS> vy;

	for (int row = 0; row < ROWS; row++)
	{
		const float* px1 = x.getConstBuffer() + (row * kFloatsPerDSPVector);
		const float* px2 = px1 + kFloatsPerSIMDVector;
		float* py1 = vy.getBuffer() + (row * kFloatsPerDSPVector) + kFloatsPerSIMDVector;

		for (int n = 0; n < kSIMDVectorsPerDSPVector - 1; ++n)
		{
			vecStore(py1, vecShuffleRight(vecLoad(px1), vecLoad(px2)));

			px1 += kFloatsPerSIMDVector;
			px2 += kFloatsPerSIMDVector;
			py1 += kFloatsPerSIMDVector;
		}

		px2 = x.getConstBuffer() + (row * kFloatsPerDSPVector);
		py1 = vy.getBuffer() + (row * kFloatsPerDSPVector);

		vecStore(py1, vecShuffleRight(vecLoad(px1), vecLoad(px2)));
	}

	return vy;
}

} // mlext
} // blink