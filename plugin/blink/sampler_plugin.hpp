#pragma once

#include <set>
#include <blink/plugin.hpp>
#include <blink/sampler_instance.hpp>

namespace blink {

class SamplerPlugin : public Plugin
{
public:

	~SamplerPlugin();

	SamplerInstance* add_instance();
	void destroy_instance(SamplerInstance* instance);

private:

	virtual SamplerInstance* make_instance() = 0;

	std::set<SamplerInstance*> instances_;
};

inline SamplerPlugin::~SamplerPlugin()
{
	for (auto instance : instances_)
	{
		delete instance;
	}
}

inline SamplerInstance* SamplerPlugin::add_instance()
{
	const auto instance = make_instance();

	instances_.insert(instance);

	return instance;
}

inline void SamplerPlugin::destroy_instance(SamplerInstance* instance)
{
	instances_.erase(instance);
	delete instance;
}

} // tract