#pragma once

#include <blink.h>

namespace blink {

struct ParameterSpec
{
	blink_UUID uuid;
	const char* name;
	const char* long_desc = nullptr;
	int flags = 0;
};

}