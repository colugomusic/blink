cmake_minimum_required(VERSION 3.23)
project(blink)
option(BLINK_HOST "blink host" OFF)

if (BLINK_HOST)
	add_subdirectory(lib/host)
endif()