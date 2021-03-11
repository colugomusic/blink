#pragma once

#include <functional>
#include <optional>
#include "parameter_spec.hpp"

namespace blkhdgen {

template <class T>
struct SliderSpec : public ParameterSpec
{
	//std::function<T(T)> curve;
	//std::function<T(T)> inverse_curve;
	std::function<std::string(T)> display_value;
	std::function<std::optional<T>(const std::string&)> from_string;
	std::function<T(T)> constrain;
	std::function<T(T, int, bool)> drag;
	std::function<T(T, bool)> increment;
	std::function<T(T, bool)> decrement;
	T default_value;
};

}