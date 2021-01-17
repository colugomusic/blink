#pragma once

#include <map>
#include "envelope_spec.hpp"
#include "group.hpp"
#include "parameter.hpp"
#include "sample_data.hpp"
#include "slider_spec.hpp"

namespace blkhdgen {

class GeneratorBase
{

public:

	virtual ~GeneratorBase() {}

	virtual const char* get_error_string(blkhdgen_Error error) const = 0;

	void set_data_offset(int offset);
	int get_num_channels() const { return 2; }

	int get_num_groups() const;
	int get_num_parameters() const;

	const Group& get_group(blkhdgen_Index index) const;
	const Group& get_group_by_id(blkhdgen_ID id) const;
	Parameter& get_parameter(blkhdgen_Index index);
	Parameter& get_parameter_by_id(blkhdgen_UUID uuid);

protected:

	void add_group(blkhdgen_ID id, std::string name);
	std::shared_ptr<EnvelopeParameter> add_parameter(EnvelopeSpec spec);
	std::shared_ptr<OptionParameter> add_parameter(OptionSpec spec);
	std::shared_ptr<ToggleParameter> add_parameter(ToggleSpec spec);

	template <class T>
	std::shared_ptr<SliderParameter<T>> add_parameter(SliderSpec<T> spec);

	int get_data_offset() const { return data_offset_; }

	static ml::DSPVectorArray<2> stereo_pan(const ml::DSPVectorArray<2> in, float pan, std::shared_ptr<EnvelopeParameter> pan_envelope, Traverser* traverser);

private:

	std::map<blkhdgen_ID, Group> groups_;
	std::map<blkhdgen_UUID, std::shared_ptr<Parameter>> parameters_;
	std::atomic<int> data_offset_;
};

void GeneratorBase::set_data_offset(int offset)
{
	data_offset_ = offset;
}

void GeneratorBase::add_group(blkhdgen_ID id, std::string name)
{
	groups_[id] = { id, name };
}

std::shared_ptr<EnvelopeParameter> GeneratorBase::add_parameter(EnvelopeSpec spec)
{
	const auto param = std::make_shared<EnvelopeParameter>(spec);

	parameters_[spec.uuid] = param;

	return param;
}

std::shared_ptr<OptionParameter> GeneratorBase::add_parameter(OptionSpec spec)
{
	const auto param = std::make_shared<OptionParameter>(spec);

	parameters_[spec.uuid] = param;

	return param;
}

template <class T>
std::shared_ptr<SliderParameter<T>> GeneratorBase::add_parameter(SliderSpec<T> spec)
{
	const auto param = std::make_shared<SliderParameter<T>>(spec);

	parameters_[spec.uuid] = param;

	return param;
}

std::shared_ptr<ToggleParameter> GeneratorBase::add_parameter(ToggleSpec spec)
{
	const auto param = std::make_shared<ToggleParameter>(spec);

	parameters_[spec.uuid] = param;

	return param;
}

int GeneratorBase::get_num_groups() const
{
	return int(groups_.size());
}

int GeneratorBase::get_num_parameters() const
{
	return int(parameters_.size());
}

const Group& GeneratorBase::get_group(blkhdgen_Index index) const
{
	auto pos = groups_.begin();

	std::advance(pos, index);

	return pos->second;
}

const Group& GeneratorBase::get_group_by_id(blkhdgen_ID id) const
{
	auto pos = groups_.find(id);

	return pos->second;
}

Parameter& GeneratorBase::get_parameter(blkhdgen_Index index)
{
	auto pos = parameters_.begin();

	std::advance(pos, index);

	return *pos->second;
}

Parameter& GeneratorBase::get_parameter_by_id(blkhdgen_UUID uuid)
{
	auto pos = parameters_.find(uuid);

	return *pos->second;
}

ml::DSPVectorArray<2> GeneratorBase::stereo_pan(const ml::DSPVectorArray<2> in, float pan, std::shared_ptr<EnvelopeParameter> pan_envelope, Traverser* traverser)
{
	auto out = in;

	ml::DSPVector env_pan;

	for (int i = 0; i < kFloatsPerDSPVector; i++)
	{
		env_pan[i] = pan_envelope->get_mod_value(traverser);
	}

	const auto zero = ml::DSPVector(0.0f);
	const auto one = ml::DSPVector(1.0f);

	env_pan = ml::clamp(env_pan + pan, ml::DSPVector(-1.0f), ml::DSPVector(1.0f));

	const auto pan_amp_L = ml::lerp(one, zero, ml::max(zero, env_pan));
	const auto pan_amp_R = ml::lerp(one, zero, ml::max(zero, 0.0f - env_pan));

	const auto pan_vec = ml::concatRows(pan_amp_L, pan_amp_R);

	out *= pan_vec;

	out.row(0) += out.row(1) * (1.0f - pan_amp_R);
	out.row(1) += out.row(0) * (1.0f - pan_amp_L);

	return out;
}

}