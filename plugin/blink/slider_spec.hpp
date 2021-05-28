#pragma once

#include <functional>
#include <optional>
#include <tweak/tweak.hpp>

namespace blink {

template <class T>
struct SliderSpec : public tweak::Spec<T>
{
	T default_value;
	int flags = 0;
};

}
