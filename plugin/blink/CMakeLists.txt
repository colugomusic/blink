cmake_minimum_required(VERSION 3.14)
project(blink_plugin)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/Modules)

include(CPM)

CPMAddPackage(
	NAME tweak
	GITHUB_REPOSITORY "colugomusic/tweak"
	GIT_TAG "master"
	DOWNLOAD_ONLY YES
)

CPMAddPackage(
    NAME madronalib
    GITHUB_REPOSITORY madronalabs/madronalib
    GIT_TAG master
    DOWNLOAD_ONLY YES
)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/..
	${CMAKE_CURRENT_LIST_DIR}/../../include
	${tweak_SOURCE_DIR}/include
	${madronalib_SOURCE_DIR}/source
)
