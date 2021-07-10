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

	SamplerUnit* add_unit();

private:

	virtual std::shared_ptr<SamplerUnit> make_unit() = 0;

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

inline SamplerUnit* SamplerInstance::add_unit()
{
	const auto unit = make_unit();

	units_.push_back(unit);

	register_unit(unit.get());

	return unit.get();
}

} // blink