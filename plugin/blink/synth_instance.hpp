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

	SynthUnit* add_unit();

private:

	virtual std::shared_ptr<SynthUnit> make_unit() = 0;

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

inline SynthUnit* SynthInstance::add_unit()
{
	const auto unit = make_unit();

	unit->stream_init();

	units_.push_back(unit);

	register_unit(unit.get());

	return unit.get();
}

} // blink