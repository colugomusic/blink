cmake_minimum_required(VERSION 3.23)
project(blink-host)
add_library(blink-host INTERFACE)
add_library(blink::host ALIAS blink-host)
target_sources(blink-host INTERFACE
	FILE_SET blink_host
	TYPE HEADERS
	BASE_DIRS
		..
		../../include
	FILES
		../blink/host_impl.hpp
)
target_compile_features(blink-host INTERFACE cxx_std_20)