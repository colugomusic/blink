/* TODELETE:
#pragma once

#include <cassert>
#include "block_positions.hpp"
#include "searcher_spec.hpp"

namespace blink {

template <class T, class Data>
class Searcher {
public: 
	auto search(const Data& data, blink_Position block_position) const {
		int left; 
		return binary_search(data, block_position, 0, &left);
	} 
	void search_vec(const Data& data, const BlockPositions& block_positions, T* out) const {
		search_vec(data, block_positions, block_positions.count, out);
	} 
	void search_vec(const Data& data, const BlockPositions& block_positions, int n, T* out) const {
		int left = 0;
		bool reset = false;
		auto prev_pos = block_positions.prev_pos; 
		for (int i = 0; i < n; i++) {
			const auto pos = block_positions.positions[i]; 
			if (pos < prev_pos) {
				// This occurs when Blockhead loops back to an earlier song position.
				// We perform a binary search to get back on track
				reset = true;
			} 
			if (reset) {
				reset = false; 
				out[i] = binary_search(data, block_positions.positions[i], 0, &left);
			}
			else {
				out[i] = forward_search(data, block_positions.positions[i], left, &left);
			} 
			prev_pos = pos;
		}
	} 
private: 
	virtual T binary_search(const Data& data, blink_Position position, int search_beg_index, int* left) const = 0;
	virtual T forward_search(const Data& data, blink_Position position, int search_beg_index, int* left) const = 0;
};

class RealSearcher : public Searcher<float, blink_RealPoints> {
public: 
	RealSearcher(RealSearcherSpec spec, float default_value) : spec_(spec), default_value_(default_value) {} 
	float binary_search(const blink_RealPoints& data, blink_Position position, int search_beg_index, int* left) const override {
		return spec_.binary(&data, default_value_, position, search_beg_index, left);
	} 
	float forward_search(const blink_RealPoints& data, blink_Position position, int search_beg_index, int* left) const override {
		return spec_.forward(&data, default_value_, position, search_beg_index, left);
	} 
	auto search_vec_(const blink_RealPoints& data, const BlockPositions& block_positions) const {
		ml::DSPVector out; 
		Searcher::search_vec(data, block_positions, out.getBuffer()); 
		return out;
	} 
private: 
	RealSearcherSpec spec_;
	float default_value_;
};

class ChordSearcher : public Searcher<blink_Scale, blink_ChordData> {
public: 
	ChordSearcher(ChordSearcherSpec spec) : spec_(spec) {} 
	blink_Scale binary_search(const blink_ChordData& data, blink_Position position, int search_beg_index, int* left) const override {
		return spec_.binary(&data, position, search_beg_index, left);
	} 
	blink_Scale forward_search(const blink_ChordData& data, blink_Position position, int search_beg_index, int* left) const override {
		return spec_.forward(&data, position, search_beg_index, left);
	} 
	auto search_vec_(const blink_ChordData& data, const BlockPositions& block_positions) const {
		ml::DSPVectorInt out;
		std::array<blink_Scale, kFloatsPerDSPVector> buffer; 
		Searcher::search_vec(data, block_positions, buffer.data()); 
		for (int i = 0; i < kFloatsPerDSPVector; i++) {
			out[i] = buffer[i];
		} 
		return out;
	} 
private: 
	ChordSearcherSpec spec_;
};

class StepSearcher : public Searcher<int64_t, blink_IntPoints> {
public: 
	StepSearcher(StepSearcherSpec spec, int64_t default_value) : spec_(spec), default_value_{default_value} {} 
	int64_t binary_search(const blink_IntPoints& points, blink_Position position, int search_beg_index, int* left) const override {
		assert (spec_.binary);
		return spec_.binary(&points, default_value_, position, search_beg_index, left);
	} 
	int64_t forward_search(const blink_IntPoints& points, blink_Position position, int search_beg_index, int* left) const override {
		assert (spec_.forward);
		return spec_.forward(&points, default_value_, position, search_beg_index, left);
	} 
	auto search_vec_(const blink_IntPoints& data, const BlockPositions& block_positions) const {
		ml::DSPVectorInt out;
		std::array<int64_t, kFloatsPerDSPVector> buffer; 
		Searcher::search_vec(data, block_positions, buffer.data()); 
		for (int i = 0; i < kFloatsPerDSPVector; i++) {
			out[i] = buffer[i] ? 1 : 0;
		} 
		return out;
	} 
private: 
	StepSearcherSpec spec_;
	int64_t default_value_;
};

} // blink
*/
