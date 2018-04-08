cmake_minimum_required(VERSION 3.10)
project(observable)

set(CMAKE_CXX_STANDARD 17)

IF(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
    IF(CMAKE_COMPILER_IS_GNUCC)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
        set(CMAKE_CXX_FLAGS "--coverage -fno-inline -fno-inline-small-functions -fno-default-inline -g" ${CMAKE_CXX_FLAGS})
    ENDIF(CMAKE_COMPILER_IS_GNUCC)
    message("USE COVERAGE PROFILE")
else()
    message("DONT USE COVERAGE PROFILE")
ENDIF()



add_subdirectory(external/googletest)
include_directories(external/googletest/googletest/include)

include_directories(include)

add_subdirectory(tests)


file(GLOB_RECURSE XMOL_HEADERS include/*.h)
file(GLOB_RECURSE XMOL_SOURCES src/*.cpp)


add_library(observable ${XMOL_SOURCES} ${XMOL_HEADERS})

add_executable(main main.cpp)
target_link_libraries(main observable)
target_link_libraries(observable gcov)
