cmake_minimum_required(VERSION 3.14)
project(blink_plugin)

set(PLUGIN_OUTPUT_DIR bin CACHE PATH "plugin output directory")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/Modules)

include(CMakeRC)

add_library(adhoc-madronalib INTERFACE)
add_library(adhoc::madronalib ALIAS adhoc-madronalib)
target_sources(adhoc-madronalib INTERFACE
	FILE_SET adhoc_madronalib
	TYPE HEADERS
	BASE_DIRS
		../extern/submodules/madronalib/source
		../extern/submodules/madronalib/source/app
)

add_library(${PROJECT_NAME} INTERFACE)

add_subdirectory(../extern/submodules/ent ent)
add_subdirectory(../extern/submodules/snd snd)

target_link_libraries(${PROJECT_NAME} INTERFACE
	ent::ent
	adhoc::madronalib
	snd::snd
)

target_include_directories(${PROJECT_NAME} INTERFACE
	..
	../../include
)

if (CMAKE_SYSTEM_NAME STREQUAL Darwin)
	target_include_directories(${PROJECT_NAME} INTERFACE
		../extern/submodules/sse2neon
	)
endif()

function(blink_plugin_get_base_filename out_var name type)
  set(on_windows $<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>)
  set(on_macos $<STREQUAL:${CMAKE_SYSTEM_NAME},Darwin>)
  set(on_linux $<STREQUAL:${CMAKE_SYSTEM_NAME},Linux>)
  set(debug_build $<OR:$<CONFIG:Debug>,$<STREQUAL:${CMAKE_BUILD_TYPE},Debug}>>)
  set(PLATFORM_DIR $<IF:${on_windows},win64,$<IF:${on_macos},macos,linux>>)
	set(${out_var} ${type}.${name}.v${PROJECT_VERSION}.${PLATFORM_DIR}$<${debug_build}:.dbg> PARENT_SCOPE)
endfunction()

function(blink_plugin_get_target_name out_var name type)
	set(${out_var} ${type}_${name} PARENT_SCOPE)
endfunction()

function(blink_plugin_add name type src resources)
	blink_plugin_get_base_filename(output_name ${name} ${type})
	blink_plugin_get_target_name(target_name ${name} ${type})
	cmrc_add_resource_library(${target_name}_resources ALIAS ${target_name}::rc NAMESPACE plugin ${resources})
	add_library(${target_name} SHARED ${src})
	target_link_libraries(${target_name} PRIVATE blink_plugin ${target_name}::rc)
	target_include_directories(${target_name} PRIVATE ${extra_include_dirs})
	set_target_properties(${target_name} PROPERTIES
		OUTPUT_NAME ${output_name}
		FOLDER "plugins/${type}s"
		PREFIX ""
		SUFFIX ".blink"
		RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${PLUGIN_OUTPUT_DIR}
		RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${PLUGIN_OUTPUT_DIR}
		RUNTIME_OUTPUT_DIRECTORY                ${PLUGIN_OUTPUT_DIR}
		RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${PLUGIN_OUTPUT_DIR}
		LIBRARY_OUTPUT_DIRECTORY_DEBUG          ${PLUGIN_OUTPUT_DIR}
		LIBRARY_OUTPUT_DIRECTORY_RELEASE        ${PLUGIN_OUTPUT_DIR}
		LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${PLUGIN_OUTPUT_DIR}
		LIBRARY_OUTPUT_DIRECTORY                ${PLUGIN_OUTPUT_DIR}
		CXX_STANDARD 17
	)
	set_target_properties(${target_name}_resources PROPERTIES FOLDER "plugins/${type}s/resources")
	target_compile_definitions(
		${target_name} PRIVATE
		PLUGIN_VERSION="${PROJECT_VERSION}"
		_USE_MATH_DEFINES
	)
endfunction()

function(blink_plugin_add_effect name src resources)
	blink_plugin_add(${name} "effect" "${src}" "${resources}")
endfunction()

function(blink_plugin_add_sampler name src resources)
	blink_plugin_add(${name} "sampler" "${src}" "${resources}")
endfunction()

function(blink_plugin_add_synth name src resources)
	blink_plugin_add(${name} "synth" "${src}" "${resources}")
endfunction()

source_group(src REGULAR_EXPRESSION "/src/.*\\.(h|cpp)")