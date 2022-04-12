#pragma once

#include <memory>
#include <vector>
#include "sampler_unit.hpp"
#include "instance.hpp"

namespace blink {

class SamplerInstance : public Instance
{
public:

	SamplerInstance(Plugin* plugin);

	virtual ~SamplerInstance();

	auto add_unit() -> SamplerUnit*;

private:

	virtual auto make_unit() -> std::shared_ptr<SamplerUnit> = 0;

	std::vector<std::shared_ptr<SamplerUnit>> units_;
};

inline SamplerInstance::SamplerInstance(Plugin* plugin)
	: Instance(plugin)
{
}

inline SamplerInstance::~SamplerInstance()
{
	for (auto unit : units_)
	{
		unregister_unit(unit.get());
	}
}

inline auto SamplerInstance::add_unit() -> SamplerUnit*
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