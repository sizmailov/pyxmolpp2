cmake_minimum_required(VERSION 3.10)
project(observable)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE HEADERS  *.h)
set(CMAKE_CXX_FLAGS "-fsanitize=address" ${CMAKE_CXX_FLAGS})
add_executable(observable main.cpp Logger.cpp ${HEADERS})
