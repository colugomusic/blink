#pragma once

#include <blink.h>
#include <cstdint>

#pragma warning(push, 0)
#include <DSP/MLDSPOps.h>
#pragma warning(pop)

namespace blink {

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

	void generate(const blink_Position* block_pos, int n, DataOffset data_offset)
	{
		reset_ = 0;

		blink_Position position = last_read_position_;

		for (int i = 0; i < n; i++)
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
	
	void generate(const blink_Position block_pos, DataOffset data_offset)
	{
		reset_[0] = 0;

		blink_Position position = last_read_position_;

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

	blink_Position last_read_position_ = std::numeric_limits<blink_Position>::min();
	DataOffset last_data_offset_ = 0;

	ml::DSPVector read_position_;
	ml::DSPVectorInt reset_;
	bool is_vector_ = false;
};

//
// Puts a reset event at the traverser's start if the pointer changed
//
template <class T>
class TraverserResetter
{
public:

	void check(const T* ptr, Traverser* traverser)
	{
		if (ptr != last_ptr_)
		{
			traverser->set_reset(0);
		}

		last_ptr_ = ptr;
	}

private:

	const T* last_ptr_ = nullptr;
};
}