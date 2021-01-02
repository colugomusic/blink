#pragma once

#include <blkhdgen/draft.h>

namespace blkhdgen {

//
// Generates a vector of block read positions and a vector of search reset points to
// indicate places where traversers need to reset themselves
// 
// Can also be used to generate a single position and reset event (the first elements
// of the vectors are used)
//
class Traverser
{
public:

	using DataOffset = std::int32_t;

	void generate(const blkhdgen_Position* block_pos, DataOffset data_offset)
	{
		reset_ = 0;

		blkhdgen_Position position = last_read_position_;

		for (int i = 0; i < kFloatsPerDSPVector; i++)
		{
			read_position_[i] = float(block_pos[i]) - data_offset;

			if (read_position_[i] < position)
			{
				reset_[i] = 1;
			}

			position = read_position_[i];
		}

		last_data_offset_ = data_offset;
		last_read_position_ = position;
		is_vector_ = true;
	}
	
	void generate(const blkhdgen_Position block_pos, DataOffset data_offset)
	{
		reset_[0] = 0;

		blkhdgen_Position position = last_read_position_;

		read_position_[0] = float(block_pos) - data_offset;

		if (read_position_[0] < last_read_position_)
		{
			reset_[0] = 1;
		}

		last_data_offset_ = data_offset;
		last_read_position_ = read_position_[0];
		is_vector_ = false;
	}

	bool is_vector() const { return is_vector_; }
	const ml::DSPVector& get_read_position() const { return read_position_; }
	const ml::DSPVectorInt& get_resets() const { return reset_; }

	void set_reset(int index) { reset_[index] = 1; }

private:

	blkhdgen_Position last_read_position_ = std::numeric_limits<blkhdgen_Position>::min();
	DataOffset last_data_offset_ = 0;

	ml::DSPVector read_position_;
	ml::DSPVectorInt reset_;
	bool is_vector_ = false;
};

class TraverserPointDataResetter
{
public:

	void check(const blkhdgen_EnvelopePoints* points, Traverser* traverser)
	{
		if (points != last_point_data_)
		{
			traverser->set_reset(0);
		}

		last_point_data_ = points;
	}

private:

	const blkhdgen_EnvelopePoints* last_point_data_ = nullptr;
};
}