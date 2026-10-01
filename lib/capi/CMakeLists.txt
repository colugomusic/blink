cmake_minimum_required(VERSION 3.30)
project(blink-capi)
add_library(blink-capi INTERFACE)
add_library(blink::capi ALIAS blink-capi)
target_sources(blink-capi INTERFACE
	FILE_SET HEADERS
	TYPE HEADERS
	BASE_DIRS
		${CMAKE_CURRENT_LIST_DIR}/../../include
	FILES
		${CMAKE_CURRENT_LIST_DIR}/../../include/blink.h
		${CMAKE_CURRENT_LIST_DIR}/../../include/blink_std.h
)
set_target_properties(blink-capi PROPERTIES EXPORT_NAME capi)
include(CMakePackageConfigHelpers)
install(TARGETS blink-capi EXPORT blink-capi-targets FILE_SET HEADERS DESTINATION include/blink)
install(EXPORT blink-capi-targets FILE blink-capi-targets.cmake NAMESPACE blink:: DESTINATION lib/cmake/blink-capi)
configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/cmake/blink-capi-config.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/blink-capi-config.cmake" INSTALL_DESTINATION lib/cmake/blink-capi)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/blink-capi-config.cmake" DESTINATION lib/cmake/blink-capi)
