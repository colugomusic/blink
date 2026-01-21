#pragma once

#if defined(__GNUC__) || defined(__clang__)
#	define BLINK_QUIET_INCLUDE_BEGIN _Pragma("GCC system_header")
#elif defined(_MSC_VER)
#	define BLINK_QUIET_INCLUDE_BEGIN __pragma(warning(push, 0))
#	define BLINK_QUIET_INCLUDE_END   __pragma(warning(pop))
#endif

#ifndef BLINK_QUIET_INCLUDE_BEGIN
#	define BLINK_QUIET_INCLUDE_BEGIN
#endif

#ifndef BLINK_QUIET_INCLUDE_END
#	define BLINK_QUIET_INCLUDE_END
#endif
