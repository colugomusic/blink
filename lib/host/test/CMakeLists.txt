cmake_minimum_required(VERSION 3.20)
project(blink-host-test)
list(APPEND blink-host-test-src
	src/doctest.h
	src/main.cpp
)
add_executable(blink-host-test ${blink-host-test-src})
target_link_libraries(blink-host-test blink::host)
add_test(NAME blink-host-test COMMAND blink-host-test)
