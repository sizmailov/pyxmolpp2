cmake_minimum_required(VERSION 3.10)
project(observable)

set(CMAKE_CXX_STANDARD 17)

IF(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
    IF(CMAKE_COMPILER_IS_GNUCC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
        set(CMAKE_CXX_FLAGS "--coverage -fno-inline -fno-inline-small-functions -fno-default-inline -g ${CMAKE_CXX_FLAGS}")
    ENDIF(CMAKE_COMPILER_IS_GNUCC)
    message("USE COVERAGE PROFILE")

else()
    message("DONT USE COVERAGE PROFILE")
ENDIF()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wreturn-type")

enable_testing()


add_subdirectory(external/googletest)
include_directories(external/googletest/googletest/include)
include_directories(external/range-v3/include)
include_directories(external/GSL/include)

include_directories(include)

add_subdirectory(tests)


file(GLOB_RECURSE XMOL_HEADERS include/**.h)
file(GLOB_RECURSE XMOL_SOURCES src/**.cpp src/**.h)


add_library(observable ${XMOL_SOURCES} ${XMOL_HEADERS})

target_link_libraries(observable gcov)
