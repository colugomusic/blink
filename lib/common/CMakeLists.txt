cmake_minimum_required(VERSION 3.30)
project(blink-common)

find_package(ent REQUIRED CONFIG)
find_package(madronalib REQUIRED CONFIG)
find_package(snd REQUIRED CONFIG)

add_library(blink-common)
add_library(blink::common ALIAS blink-common)
target_sources(blink-common PUBLIC
	FILE_SET HEADERS
	BASE_DIRS
		${CMAKE_CURRENT_LIST_DIR}/..
	FILES
		${CMAKE_CURRENT_LIST_DIR}/../blink/transform/calculators/pitch.hpp
		${CMAKE_CURRENT_LIST_DIR}/../blink/transform/calculators/reverse.hpp
		${CMAKE_CURRENT_LIST_DIR}/../blink/transform/calculators/speed.hpp
		${CMAKE_CURRENT_LIST_DIR}/../blink/transform/calculators/warp.hpp
		${CMAKE_CURRENT_LIST_DIR}/../blink/transform/correction_grains.hpp
		${CMAKE_CURRENT_LIST_DIR}/../blink/transform/stretch.hpp
		${CMAKE_CURRENT_LIST_DIR}/../blink/transform/tape.hpp
		${CMAKE_CURRENT_LIST_DIR}/../blink/bits.hpp
		${CMAKE_CURRENT_LIST_DIR}/../blink/block_positions.hpp
		${CMAKE_CURRENT_LIST_DIR}/../blink/common-impl.hpp
		${CMAKE_CURRENT_LIST_DIR}/../blink/data.hpp
		${CMAKE_CURRENT_LIST_DIR}/../blink/dsp.hpp
		${CMAKE_CURRENT_LIST_DIR}/../blink/math.hpp
		${CMAKE_CURRENT_LIST_DIR}/../blink/resource_store.hpp
		${CMAKE_CURRENT_LIST_DIR}/../blink/sample_data.hpp
		${CMAKE_CURRENT_LIST_DIR}/../blink/search.hpp
		${CMAKE_CURRENT_LIST_DIR}/../blink/traverser.hpp
		${CMAKE_CURRENT_LIST_DIR}/../blink/tweak.hpp
		${CMAKE_CURRENT_LIST_DIR}/../blink/types.hpp
		${CMAKE_CURRENT_LIST_DIR}/../blink/quiet-include.hpp
)
target_sources(blink-common PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/../blink/common-impl.cpp
)
target_link_libraries(blink-common PUBLIC
	blink::capi
	ent::ent
	madronalib::madronalib
	snd::snd
)
set_target_properties(blink-common PROPERTIES EXPORT_NAME common)
include(CMakePackageConfigHelpers)
install(TARGETS blink-common EXPORT blink-common-targets FILE_SET HEADERS)
install(EXPORT blink-common-targets FILE blink-common-targets.cmake NAMESPACE blink:: DESTINATION lib/cmake/blink-common)
configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/cmake/blink-common-config.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/blink-common-config.cmake" INSTALL_DESTINATION lib/cmake/blink-common)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/blink-common-config.cmake" DESTINATION lib/cmake/blink-common)
