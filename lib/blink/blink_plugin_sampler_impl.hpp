#pragma once

#include "blink_plugin_impl.hpp"

namespace blink {

/* TODO: move this logic to the host
template <typename UnitProcessFn, typename UnitResetFn, typename InstanceResetFn> [[nodiscard]]
auto sampler_process(Plugin* plugin, blink_UnitIdx unit_idx, const blink_SamplerBuffer& buffer, const blink_SamplerUnitState& unit_state, float* out, UnitProcessFn&& process_fn, UnitResetFn&& unit_reset_fn, InstanceResetFn&& instance_reset_fn) -> blink_Error {
	auto& unit_proc = plugin->unit.get<UnitProcess>(unit_idx.value);
	begin_process(plugin, unit_proc.instance_idx, buffer.buffer_id, instance_reset_fn);
	if (buffer.buffer_id.value > unit_proc.buffer_id.value.value + 1) {
		// unit is reset at the start of the buffer if we have gone
		// at least one buffer without processing this unit
		unit_reset_fn(unit_idx);
	}
	unit_proc.buffer_id.value = buffer.buffer_id;
	unit_proc.block_positions(buffer.positions, kFloatsPerDSPVector);
	return process_fn(buffer, unit_state, out);
}
*/

} // blink
