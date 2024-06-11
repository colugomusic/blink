cmake_minimum_required(VERSION 3.14)
project(blink_lib)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/Modules)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} PARENT_SCOPE)

include(CPM)

CPMAddPackage(
	NAME ent
	GITHUB_REPOSITORY "colugomusic/ent"
	GIT_TAG "master"
	DOWNLOAD_ONLY YES
)

CPMAddPackage(
	NAME snd
	GITHUB_REPOSITORY "colugomusic/snd"
	GIT_TAG "master"
	DOWNLOAD_ONLY YES
)

CPMAddPackage(
	NAME madronalib
	GITHUB_REPOSITORY "madronalabs/madronalib"
	GIT_TAG "master"
	DOWNLOAD_ONLY YES
)

if (CMAKE_SYSTEM_NAME STREQUAL Darwin)
	CPMAddPackage(
		NAME sse2neon
		GITHUB_REPOSITORY DLTcollab/sse2neon
		GIT_TAG master
		DOWNLOAD_ONLY YES
	)
	list(APPEND extra_include_dirs ${sse2neon_SOURCE_DIR})
endif()

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
	${CMAKE_CURRENT_LIST_DIR}
	${CMAKE_CURRENT_LIST_DIR}/../include
	${ent_SOURCE_DIR}/include
	${snd_SOURCE_DIR}/include
	${madronalib_SOURCE_DIR}/source
	${extra_include_dirs}
)
