cmake_minimum_required(VERSION 3.30)
project(blink-plugin)

add_library(blink-plugin INTERFACE)
add_library(blink::plugin ALIAS blink-plugin)
target_sources(blink-plugin INTERFACE
	FILE_SET HEADERS
	BASE_DIRS
		${CMAKE_CURRENT_LIST_DIR}/..
		${CMAKE_CURRENT_LIST_DIR}/../../include
	FILES
		${CMAKE_CURRENT_LIST_DIR}/../blink/plugin_impl.hpp
)
target_link_libraries(blink-plugin INTERFACE blink::common)
set_target_properties(blink-plugin PROPERTIES EXPORT_NAME plugin)
include(CMakePackageConfigHelpers)
install(TARGETS blink-plugin EXPORT blink-plugin-targets FILE_SET HEADERS)
install(EXPORT blink-plugin-targets FILE blink-plugin-targets.cmake NAMESPACE blink:: DESTINATION lib/cmake/blink-plugin)
configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/cmake/blink-plugin-config.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/blink-plugin-config.cmake" INSTALL_DESTINATION lib/cmake/blink-plugin)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/blink-plugin-config.cmake" DESTINATION lib/cmake/blink-plugin)
install(FILES
	"${CMAKE_CURRENT_LIST_DIR}/cmake/blink-plugin-funcs.cmake"
	"${CMAKE_CURRENT_LIST_DIR}/cmake/CMakeRC.cmake"
	DESTINATION lib/cmake/blink-plugin
)
