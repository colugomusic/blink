#include "common-impl.hpp"

namespace blink::add::slider {

auto empty_int(const blink_HostFns& host) -> blink_SliderIntIdx {
	return host.add_slider_int(host.usr);
}

auto empty_real(const blink_HostFns& host) -> blink_SliderRealIdx {
	return host.add_slider_real(host.usr);
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
