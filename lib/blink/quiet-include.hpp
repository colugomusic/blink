#pragma once

#if defined(_MSC_VER)
#	define BLINK_QUIET_INCLUDE_BEGIN __pragma(warning(push, 0))
#	define BLINK_QUIET_INCLUDE_END   __pragma(warning(pop))
#elif defined(__clang__)
#	define BLINK_QUIET_INCLUDE_BEGIN \
		_Pragma("clang diagnostic push") \
		_Pragma("clang diagnostic ignored \"-Weverything\"")
#	define BLINK_QUIET_INCLUDE_END \
		_Pragma("clang diagnostic pop")
#elif defined(__GNUC__)
#	define BLINK_QUIET_INCLUDE_BEGIN \
		_Pragma("GCC diagnostic push") \
		_Pragma("GCC diagnostic ignored \"-Wall\"") \
		_Pragma("GCC diagnostic ignored \"-Wextra\"") \
		_Pragma("GCC diagnostic ignored \"-Wpedantic\"") \
		_Pragma("GCC diagnostic ignored \"-Wconversion\"") \
		_Pragma("GCC diagnostic ignored \"-Wsign-conversion\"") \
		_Pragma("GCC diagnostic ignored \"-Wold-style-cast\"") \
		_Pragma("GCC diagnostic ignored \"-Wshadow\"") \
		_Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")
#	define BLINK_QUIET_INCLUDE_END \
		_Pragma("GCC diagnostic pop")
#else
#	define BLINK_QUIET_INCLUDE_BEGIN
#	define BLINK_QUIET_INCLUDE_END
#endif
