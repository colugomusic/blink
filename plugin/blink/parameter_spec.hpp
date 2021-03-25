#pragma once

#include <blink.h>

namespace blink {

struct ParameterSpec
{
	blink_UUID uuid;
	int group_index = -1;
	const char* name;
	int flags = 0;
};

}