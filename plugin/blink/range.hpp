#pragma once

namespace blink {

template <class T>
struct Range
{
	T min = T(0);
	T max = T(0);
};

}