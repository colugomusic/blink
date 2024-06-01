#pragma once

#include <blink.h>

namespace blink {

struct ChordData {
	const blink_ChordData* data;
};

struct EnvData {
	const blink_EnvData* data;
	float default_value;
	float value;
};

struct OptionData {
	const blink_OptionData* data;
	int64_t value;
	int64_t default_value;
};

struct SliderIntData {
	const blink_SliderIntData* data;
	int64_t value;
	int64_t default_value;
};

struct SliderRealData {
	const blink_SliderRealData* data;
	float value;
	float default_value;
};

} // blink
