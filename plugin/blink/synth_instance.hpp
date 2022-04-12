#pragma once

#include <memory>
#include <vector>
#include "synth_unit.hpp"
#include "instance.hpp"

namespace blink {

class SynthInstance : public Instance
{
public:

	SynthInstance(Plugin* plugin);

	virtual ~SynthInstance();

	auto add_unit() -> SynthUnit*;

private:

	virtual auto  make_unit() -> std::shared_ptr<SynthUnit> = 0;

	std::vector<std::shared_ptr<SynthUnit>> units_;
};

inline SynthInstance::SynthInstance(Plugin* plugin)
	: Instance(plugin)
{
}

inline SynthInstance::~SynthInstance()
{
	for (auto unit : units_)
	{
		unregister_unit(unit.get());
	}
}

inline auto SynthInstance::add_unit() -> SynthUnit*
{
	const auto unit = make_unit();

	if (is_initialized())
	{
		unit->stream_init();
	}

	units_.push_back(unit);

	register_unit(unit.get());

	return unit.get();
}

} // blink