cmake_minimum_required(VERSION 3.30)
project(blink)

find_package(ent REQUIRED CONFIG)
find_package(madronalib REQUIRED CONFIG)
find_package(snd REQUIRED CONFIG)

add_library(blink-common INTERFACE)
add_library(blink::common ALIAS blink-common)
target_sources(blink-common INTERFACE
	FILE_SET api_headers
	TYPE HEADERS
	BASE_DIRS
		${CMAKE_CURRENT_LIST_DIR}/include
	FILES
		${CMAKE_CURRENT_LIST_DIR}/include/blink.h
		${CMAKE_CURRENT_LIST_DIR}/include/blink_std.h
)
target_sources(blink-common INTERFACE
	FILE_SET HEADERS
	BASE_DIRS
		${CMAKE_CURRENT_LIST_DIR}/lib
	FILES
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/transform/calculators/pitch.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/transform/calculators/reverse.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/transform/calculators/speed.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/transform/calculators/warp.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/transform/correction_grains.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/transform/stretch.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/transform/tape.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/bits.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/block_positions.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/common_impl.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/const_math.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/data.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/dsp.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/math.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/resource_store.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/sample_data.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/search.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/traverser.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/tweak.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/types.hpp
		${CMAKE_CURRENT_LIST_DIR}/lib/blink/quiet-include.hpp
)
target_link_libraries(blink-common INTERFACE
	ent::ent
	madronalib::madronalib
	snd::snd
)
set_target_properties(blink-common PROPERTIES EXPORT_NAME common)
include(CMakePackageConfigHelpers)
install(TARGETS blink-common
	EXPORT blink-common-targets
	FILE_SET api_headers DESTINATION include/blink
	FILE_SET HEADERS
)
install(EXPORT blink-common-targets FILE blink-common-targets.cmake NAMESPACE blink:: DESTINATION lib/cmake/blink-common)
configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/cmake/blink-common-config.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/blink-common-config.cmake" INSTALL_DESTINATION lib/cmake/blink-common)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/blink-common-config.cmake" DESTINATION lib/cmake/blink-common)
add_subdirectory(lib/host)
add_subdirectory(lib/plugin)
