cmake_minimum_required(VERSION 3.30)
project(blink-host)
add_library(blink-host INTERFACE)
add_library(blink::host ALIAS blink-host)
target_sources(blink-host INTERFACE
	FILE_SET HEADERS
	BASE_DIRS
		..
		../../include
	FILES
		../blink/host_impl.hpp
)
include(CMakePackageConfigHelpers)
install(TARGETS blink-host EXPORT blink-host-targets FILE_SET HEADERS)
install(EXPORT blink-host-targets FILE blink-host-targets.cmake NAMESPACE blink:: DESTINATION lib/cmake/blink-host)
configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/cmake/blink-host-config.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/blink-host-config.cmake" INSTALL_DESTINATION lib/cmake/blink-host)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/blink-host-config.cmake" DESTINATION lib/cmake/blink-host)