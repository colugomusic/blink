#pragma once

#include <blink.h>

namespace blink {

struct ParameterSpec
{
	blink_UUID uuid;
	blink_ID group_id = 0;
	const char* name;
	int flags = 0;
};

}