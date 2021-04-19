#pragma once

namespace blink {
namespace bits {

template <class T>
inline bool check(T mask, T bit)
{
	return (mask >> bit) & 1U;
}

}}