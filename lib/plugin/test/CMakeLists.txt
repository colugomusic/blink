cmake_minimum_required(VERSION 3.20)
project(blink-plugin-test)
list(APPEND blink-plugin-test-src
	src/doctest.h
	src/main.cpp
)
add_executable(blink-plugin-test ${blink-plugin-test-src})
target_link_libraries(blink-plugin-test blink::plugin)
add_test(NAME blink-plugin-test COMMAND blink-plugin-test)
