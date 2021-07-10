#pragma once

#include <blink.h>

namespace blink {

inline const char* get_std_error_string(blink_StdError error)
{
	switch (error)
	{
		case blink_StdError_AlreadyInitialized: return "already initialized";
		case blink_StdError_NotInitialized: return "not initialized";
		case blink_StdError_NotImplemented: return "not implemented";
		default: return "unknown error";
	}
}

} // blink