#pragma once

#include <map>
#include "block_positions.hpp"

namespace blink {

class Instance;

extern blink_SR get_SR(Instance*);

class Unit
{

public:

	Unit(Instance* instance)
		: instance_(instance)
	{
	}

	virtual ~Unit() {}

	// Called in UI thread
	virtual void stream_init() {};

	int get_num_channels() const { return 2; }
	Instance* get_instance() const { return instance_; }

protected:

	blink_SR SR() const { return get_SR(instance_); }

	void begin_process(std::uint64_t buffer_id, const blink_Position* positions, int64_t data_offset)
	{
		//
		// Unit::reset() is called at the start of the buffer if we have gone
		// at least one buffer without processing this unit
		//

		if (buffer_id > buffer_id_ + 1)
		{
			reset();
		}

		buffer_id_ = buffer_id;

		block_positions_(positions, data_offset, kFloatsPerDSPVector);
	}

	const BlockPositions& block_positions() const
	{
		return block_positions_;
	}

private:

	virtual void reset() = 0;

	BlockPositions block_positions_;
	Instance* instance_;
	std::uint64_t buffer_id_ = 0;
};

}