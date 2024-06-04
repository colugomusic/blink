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

template <int Min, int Max, int Resolution, int EditPrecision> [[nodiscard]] inline
auto linear(const blink_HostFns& host, float default_value) -> blink_SliderRealIdx {
	const auto idx = add::slider::empty_real(host);
	host.write_slider_real_default_value(host.usr, idx, default_value);
	host.write_slider_real_tweaker(host.usr, idx, tweak::linear::tweaker<Min, Max, Resolution, EditPrecision>());
	return idx;
}

template <int Min, int Max, int Resolution, int EditPrecision> [[nodiscard]] inline
auto ms(const blink_HostFns& host, float default_value) -> blink_SliderRealIdx {
	const auto idx = add::slider::empty_real(host);
	host.write_slider_real_default_value(host.usr, idx, default_value);
	host.write_slider_real_tweaker(host.usr, idx, tweak::ms::tweaker<Min, Max, Resolution, EditPrecision>());
	return idx;
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

template <int Min, int Max, int Resolution, int EditPrecision> [[nodiscard]] inline
auto linear(const blink_HostFns& host, float default_value) -> blink_EnvIdx {
	const auto idx = add::env::empty(host);
	host.write_env_default_max(host.usr, idx, float(Max) / Resolution);
	host.write_env_default_min(host.usr, idx, float(Min) / Resolution);
	host.write_env_default_value(host.usr, idx, default_value);
	host.write_env_fns(host.usr, idx, tweak::linear::fns<Resolution>());
	host.write_env_value_slider(host.usr, idx, add::slider::linear<Min, Max, Resolution, EditPrecision>(host, default_value));
	return idx;
}

template <int Min, int Max, int Resolution, int EditPrecision> [[nodiscard]] inline
auto ms(const blink_HostFns& host, float default_value) -> blink_EnvIdx {
	const auto idx = linear<Min, Max, Resolution, EditPrecision>(host, default_value);
	host.write_env_fns(host.usr, idx, tweak::ms::fns<Resolution>());
	host.write_env_value_slider(host.usr, idx, add::slider::ms<Min, Max, Resolution, EditPrecision>(host, default_value));
	return idx;
}

template <int MIN = 0, int MAX = 100> [[nodiscard]]
auto percentage(const blink_HostFns& host) -> blink_EnvIdx {
	const auto idx = add::env::empty(host);
	host.write_env_default_max(host.usr, idx, 1.0f);
	host.write_env_default_min(host.usr, idx, 0.0f);
	host.write_env_default_value(host.usr, idx, 0.0f);
	host.write_env_fns(host.usr, idx, blink::tweak::percentage::fns());
	host.write_env_value_slider(host.usr, idx, add::slider::percentage<MIN, MAX>(host));
	return idx;
}

[[nodiscard]] inline
auto percentage_bipolar(const blink_HostFns& host) -> blink_EnvIdx {
	const auto idx = add::env::empty(host);
	host.write_env_default_max(host.usr, idx, 1.0f);
	host.write_env_default_min(host.usr, idx, -1.0f);
	host.write_env_default_value(host.usr, idx, 0.0f);
	host.write_env_fns(host.usr, idx, blink::tweak::percentage::fns());
	host.write_env_value_slider(host.usr, idx, add::slider::percentage_bipolar(host));
	return idx;
}

} // env

} // namespace add
} // blink