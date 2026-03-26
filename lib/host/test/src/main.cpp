#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <blink/host_impl.hpp>

TEST_CASE("no test") {
	auto host = blink::Host{};
	auto env_amp = blink::add::env::amp(&host);
	auto env_pitch = blink::add::env::pitch(&host);
}
