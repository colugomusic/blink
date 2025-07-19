cmake_minimum_required(VERSION 3.23)
project(blink-host)
add_library(blink-host INTERFACE)
add_library(blink::host ALIAS blink-host)
target_sources(blink-host INTERFACE
	FILE_SET HEADERS
	BASE_DIRS
		..
		../../include
	FILES
		../blink/types.hpp
		../blink/bits.hpp
		../blink/block_positions.hpp
		../blink/common_impl.hpp
		../blink/const_math.hpp
		../blink/data.hpp
		../blink/dsp.hpp
		../blink/host_impl.hpp
		../blink/math.hpp
		../blink/plugin_impl.hpp
		../blink/resource_store.hpp
		../blink/sample_data.hpp
		../blink/search.hpp
		../blink/traverser.hpp
		../blink/tweak.hpp
		../blink/transform/tape.hpp
		../blink/transform/correction_grains.hpp
		../blink/transform/stretch.hpp
		../blink/transform/calculators/pitch.hpp
		../blink/transform/calculators/reverse.hpp
		../blink/transform/calculators/speed.hpp
		../blink/transform/calculators/warp.hpp
)
target_compile_features(blink-host INTERFACE cxx_std_20)