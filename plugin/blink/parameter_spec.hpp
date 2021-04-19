#pragma once

#include <blink.h>

namespace blink {

struct ParameterSpec
{
	blink_UUID uuid;
	const char* name;
	int flags = 0;
};

}