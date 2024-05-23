#pragma once

#include "blink.h"
#include "block_positions.hpp"
#include <functional>
#include <vector>

namespace blink {

using ApplyOffsetFn = auto (*)(float, float) -> float;
struct BufferID       { blink_BufferID value = {0}; };
struct ClampRange     { blink_Range value = {0}; };
struct EnvIdx         { blink_EnvIdx value = {0}; };
struct EnvSnapSettings{ blink_EnvSnapSettings value = {0, 1}; };
struct MaxSliderIdx   { size_t value = 0; };
struct MinSliderIdx   { size_t value = 0; };
struct OffsetEnvIdx   { blink_EnvIdx value = {0}; };
struct OverrideEnvIdx { blink_EnvIdx value = {0}; };
struct ParamCustomIdx { std::optional<size_t> value; };
struct ParamFlags     { blink_Flags value = {0}; };
struct ParamIcon      { blink_StdIcon value = {blink_StdIcon_None}; };
struct ParamSettings  { blink_ParamSettings value = {{0}, {0}, {0}, {0}, -1}; };
struct ParamTypeIdx   { size_t value = 0; };
struct SR             { blink_SR value = {0}; };
struct StringVec      { std::vector<std::string> value; };
struct SubParams      { std::vector<blink_ParamIdx> value; };
struct UnitVec        { std::vector<blink_UnitIdx> value; };
struct ValueSliderIdx { size_t value = 0; };
template <typename T> struct DefaultValue { T value = T(0); };

struct InstanceProcess {
	BufferID buffer_id;
	int active_buffer_units = 0;
};

struct UnitProcess {
	blink_InstanceIdx instance_idx;
	BlockPositions block_positions;
	BufferID buffer_id;
};

} // blink
