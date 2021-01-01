#pragma once

#include <blkhdgen/draft.h>

namespace blkhdgen {

class Traverser
{
public:

	using DataOffset = std::int32_t;

	void set_data_offset(DataOffset offset);
	float get_derivative() const;

protected:

	DataOffset data_offset_ = 0;

private:

	float derivative_ = 0.0f;
};

void Traverser::set_data_offset(DataOffset offset)
{
	data_offset_ = offset;
}

float Traverser::get_derivative() const
{
	return derivative_;
}

}