#pragma once

#include <set>
#include <blink/plugin.hpp>
#include <blink/synth_instance.hpp>

namespace blink {

class SynthPlugin : public Plugin
{
public:

	~SynthPlugin();

	SynthInstance* add_instance();
	void destroy_instance(SynthInstance* instance);

private:

	virtual SynthInstance* make_instance() = 0;

	std::set<SynthInstance*> instances_;
};

inline SynthPlugin::~SynthPlugin()
{
	for (auto instance : instances_)
	{
		delete instance;
	}
}

inline SynthInstance* SynthPlugin::add_instance()
{
	const auto instance = make_instance();

	instances_.insert(instance);

	return instance;
}

inline void SynthPlugin::destroy_instance(SynthInstance* instance)
{
	instances_.erase(instance);
	delete instance;
}

} // tract