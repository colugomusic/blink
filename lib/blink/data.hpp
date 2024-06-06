#pragma once

#include <blink.h>

namespace blink {
namespace uniform {

struct Chord {
	const blink_UniformChordData* data;
};

struct Env {
	const blink_UniformEnvData* data;
	float default_value;
	float value;
};

struct Option {
	const blink_UniformOptionData* data;
	int64_t value;
	int64_t default_value;
};

struct SliderInt {
	const blink_UniformSliderIntData* data;
	int64_t value;
	int64_t default_value;
};

struct SliderReal {
	const blink_UniformSliderRealData* data;
	float value;
	float default_value;
};

} // uniform

namespace varying {

struct Chord { blink_Scale value; };
struct Env { float value; };
struct Option { int64_t value; };
struct SliderInt { int64_t value; };
struct SliderReal { float value; };

} // varying
} // blink
