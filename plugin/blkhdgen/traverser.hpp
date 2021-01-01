#pragma once

#include <blkhdgen/draft.h>

namespace blkhdgen {

class PointTraverser
{
public:

	using DataOffset = std::int32_t;

	void set_points(const blkhdgen_EnvelopePoints* points)
	{
		if (points != last_point_data_)
		{
			last_point_data_ = points;
			needs_reset_ = true;
		}
	}

	void set_block_position(blkhdgen_Position block_position)
	{
		read_position_ = block_position - data_offset_;

		if (read_position_ < last_read_position_)
		{
			needs_reset_ = true;
		}

		last_read_position_ = read_position_;
	}

	void set_data_offset(DataOffset offset)
	{
		data_offset_ = offset;
	}

	blkhdgen_Position get_read_position() const { return read_position_; }
	bool needs_reset() const { return needs_reset_; }
	void reset() { needs_reset_ = false; }
	const blkhdgen_EnvelopePoints* get_points() const;

private:

	DataOffset data_offset_ = 0;
	blkhdgen_Position read_position_ = 0;
	bool needs_reset_ = false;
	const blkhdgen_EnvelopePoints* last_point_data_ = nullptr;
	mutable blkhdgen_Position last_read_position_ = std::numeric_limits<blkhdgen_Position>::min();
};

}