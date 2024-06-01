#pragma once

#include "blink.h"
#include "tweak.hpp"

namespace blink {
namespace add {
namespace slider {

[[nodiscard]] inline
auto empty_int(const blink_HostFns& host) -> blink_SliderIntIdx {
	return host.add_slider_int(host.usr);
}

[[nodiscard]] inline
auto empty_real(const blink_HostFns& host) -> blink_SliderRealIdx {
	return host.add_slider_real(host.usr);
}

template <int MIN = 0, int MAX = 100> [[nodiscard]]
auto percentage(const blink_HostFns& host) -> blink_SliderRealIdx {
	const auto idx = add::slider::empty_real(host);
	host.write_slider_real_default_value(host.usr, idx, 0.0f);
	host.write_slider_real_tweaker(host.usr, idx, tweak::percentage::tweaker<MIN, MAX>());
	return idx;
}

[[nodiscard]] inline
auto percentage_bipolar(const blink_HostFns& host) -> blink_SliderRealIdx {
	const auto idx = add::slider::empty_real(host);
	host.write_slider_real_default_value(host.usr, idx, 0.0f);
	host.write_slider_real_tweaker(host.usr, idx, tweak::percentage::bipolar::tweaker());
	return idx;
}

} // namespace slider

namespace env {

[[nodiscard]] inline
auto empty(const blink_HostFns& host) -> blink_EnvIdx {
	return host.add_env(host.usr);
}

template <int MIN = 0, int MAX = 100> [[nodiscard]]
auto percentage(const blink_HostFns& host) -> blink_EnvIdx {
	const auto idx = add::env::empty(host);
	blink_EnvFns fns;
	fns.stepify   = tweak::percentage::stepify;
	fns.to_string = tweak::percentage::to_string;
	host.write_env_default_max(host.usr, idx, 1.0f);
	host.write_env_default_min(host.usr, idx, 0.0f);
	host.write_env_default_value(host.usr, idx, 0.0f);
	host.write_env_fns(host.usr, idx, fns);
	host.write_env_value_slider(host.usr, idx, add::slider::percentage<MIN, MAX>(host));
	return idx;
}

[[nodiscard]] inline
auto percentage_bipolar(const blink_HostFns& host) -> blink_EnvIdx {
	const auto idx = add::env::empty(host);
	blink_EnvFns fns;
	fns.stepify   = tweak::percentage::stepify;
	fns.to_string = tweak::percentage::to_string;
	host.write_env_default_max(host.usr, idx, 1.0f);
	host.write_env_default_min(host.usr, idx, -1.0f);
	host.write_env_default_value(host.usr, idx, 0.0f);
	host.write_env_fns(host.usr, idx, fns);
	host.write_env_value_slider(host.usr, idx, add::slider::percentage_bipolar(host));
	return idx;
}

} // env

} // namespace add
} // blink