cmake_minimum_required(VERSION 3.30)
project(blink-plugin)

find_package(ent REQUIRED CONFIG)
find_package(madronalib REQUIRED CONFIG)
find_package(snd REQUIRED CONFIG)

add_library(blink-plugin INTERFACE)
add_library(blink::plugin ALIAS blink-plugin)
target_sources(blink-plugin INTERFACE
	FILE_SET HEADERS
	BASE_DIRS
		${CMAKE_CURRENT_LIST_DIR}/..
		${CMAKE_CURRENT_LIST_DIR}/../../include
)
target_link_libraries(blink-plugin INTERFACE
	ent::ent
	madronalib::madronalib
	snd::snd
)
include(CMakePackageConfigHelpers)
install(TARGETS blink-plugin EXPORT blink-plugin-targets FILE_SET HEADERS)
install(EXPORT blink-plugin-targets FILE blink-plugin-targets.cmake NAMESPACE blink:: DESTINATION lib/cmake/blink-plugin)
configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/cmake/blink-plugin-config.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/blink-plugin-config.cmake" INSTALL_DESTINATION lib/cmake/blink-plugin)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/blink-plugin-config.cmake" DESTINATION lib/cmake/blink-plugin)