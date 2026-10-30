#include "common-impl.hpp"
#include <cassert>

namespace blink {

auto as_global_idx(blink_ParamIdx v) -> blink::ParamGlobalIdx {
	assert (v.value >= 0);
	return {static_cast<uint32_t>(v.value)};
}

auto as_local_idx(blink_ParamIdx v) -> blink::ParamLocalIdx {
	assert (v.value >= 0);
	return {static_cast<uint32_t>(v.value)};
}

auto as_generic_idx(ParamGlobalIdx v) -> blink_ParamIdx {
	return {static_cast<int64_t>(v.value)};
}

auto as_generic_idx(ParamLocalIdx v) -> blink_ParamIdx {
	return {static_cast<int64_t>(v.value)};
}

} // blink

namespace blink::add::slider {

auto empty_int(const blink_HostFns& host) -> blink_SliderIntIdx {
	return host.add_slider_int(host.usr);
}

auto empty_real(const blink_HostFns& host) -> blink_SliderRealIdx {
	return host.add_slider_real(host.usr);
}

auto amp(const blink_HostFns& host) -> blink_SliderRealIdx {
	const auto idx = add::slider::empty_real(host);
	host.write_slider_real_tweaker(host.usr, idx, tweak::amp::tweaker());
	host.write_slider_real_default_value(host.usr, idx, 1.0f);
	return idx;
}

auto amp_offset(const blink_HostFns& host, float default_value) -> blink_SliderRealIdx {
	const auto idx = add::slider::empty_real(host);
	host.write_slider_real_tweaker(host.usr, idx, tweak::amp::tweaker());
	host.write_slider_real_default_value(host.usr, idx, default_value);
	return idx;
}

auto filter_frequency(const blink_HostFns& host, float default_value) -> blink_SliderRealIdx {
	const auto idx = add::slider::empty_real(host);
	host.write_slider_real_default_value(host.usr, idx, default_value);
	host.write_slider_real_tweaker(host.usr, idx, tweak::filter_frequency::tweaker());
	return idx;
}

auto percentage_bipolar(const blink_HostFns& host) -> blink_SliderRealIdx {
	const auto idx = add::slider::empty_real(host);
	host.write_slider_real_default_value(host.usr, idx, 0.0f);
	host.write_slider_real_tweaker(host.usr, idx, tweak::percentage::bipolar::tweaker());
	return idx;
}

} // namespace blink::add::slider

namespace blink::add::env {

auto empty(const blink_HostFns& host) -> blink_EnvIdx {
	return host.add_env(host.usr);
}

auto amp_offset(const blink_HostFns& host, float default_value) -> blink_EnvIdx {
	const auto idx = add::env::empty(host);
	auto fns = blink_EnvFns{
		.get_grid_line = [](int index, float* out) -> blink_Bool {
			*out = math::convert::linear_to_speed(float(index));
			return {true};
		},
		.stepify   = tweak::amp::stepify,
		.to_string = tweak::amp::to_string,
	};
	host.write_env_default_max(host.usr, idx, 2.0f);
	host.write_env_default_min(host.usr, idx, 0.0f);
	host.write_env_default_value(host.usr, idx, 1.0f);
	host.write_env_fns(host.usr, idx, fns);
	host.write_env_max_slider(host.usr, idx, add::slider::amp_offset(host, 2.0f));
	host.write_env_min_slider(host.usr, idx, add::slider::amp_offset(host, 0.0f));
	host.write_env_value_slider(host.usr, idx, add::slider::amp(host));
	return idx;
}

auto filter_frequency(const blink_HostFns& host, float default_value) -> blink_EnvIdx {
	const auto idx = add::env::empty(host);
	auto fns = blink_EnvFns{
		.to_string = tweak::filter_frequency::to_string,
	};
	host.write_env_default_value(host.usr, idx, default_value);
	host.write_env_default_max(host.usr, idx, 1.0f);
	host.write_env_default_min(host.usr, idx, 0.0f);
	host.write_env_fns(host.usr, idx, fns);
	host.write_env_value_slider(host.usr, idx, add::slider::filter_frequency(host, default_value));
	return idx;
}

auto percentage_bipolar(const blink_HostFns& host) -> blink_EnvIdx {
	const auto idx = add::env::empty(host);
	host.write_env_default_max(host.usr, idx, 1.0f);
	host.write_env_default_min(host.usr, idx, -1.0f);
	host.write_env_default_value(host.usr, idx, 0.0f);
	host.write_env_fns(host.usr, idx, blink::tweak::percentage::fns());
	host.write_env_value_slider(host.usr, idx, add::slider::percentage_bipolar(host));
	return idx;
}

} // blink::add::env
