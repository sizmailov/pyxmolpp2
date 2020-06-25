cmake_minimum_required(VERSION 3.5)
project(pyxmolpp2)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Python REQUIRED COMPONENTS Interpreter Development)
find_package(NetCDF REQUIRED)

IF(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
    IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fno-inline -fno-inline-small-functions -fno-default-inline -g ")
    ENDIF()
    message("USE COVERAGE PROFILE")
else()
    message("DONT USE COVERAGE PROFILE")
ENDIF()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
endif()


add_subdirectory(external/googletest EXCLUDE_FROM_ALL)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(external/benchmark EXCLUDE_FROM_ALL)
include_directories(SYSTEM external/googletest/googletest/include)
include_directories(SYSTEM external/benchmark/include)
include_directories(SYSTEM external/GSL/include)
include_directories(SYSTEM external/eigen-git-mirror)
add_subdirectory(external/pybind11 EXCLUDE_FROM_ALL)
include_directories(SYSTEM external/pybind11/include)


enable_testing()
add_subdirectory(tests)

include(cmake/WriteVersionInfo.cmake)

file(GLOB_RECURSE XMOL_HEADERS include/**.h)
file(GLOB_RECURSE XMOL_SOURCES src/**.cpp src/**.h)

add_library(xmolpp2 SHARED ${XMOL_SOURCES} ${XMOL_HEADERS})
target_include_directories(xmolpp2 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(xmolpp2 PUBLIC NetCDF::NetCDF)
set_target_properties(xmolpp2 PROPERTIES SUFFIX "-${XMOL_VERSION}.so")

add_dependencies(xmolpp2 write_version_info)

#target_compile_definitions(xmolpp2_static PRIVATE GSL_THROW_ON_CONTRACT_VIOLATION)
#target_compile_definitions(xmolpp2 PRIVATE GSL_THROW_ON_CONTRACT_VIOLATION)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_link_libraries(xmolpp2 stdc++ m)
endif()


if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
        target_link_libraries(xmolpp2 PRIVATE gcov)
    endif()
#    if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
#        message("! Enabled address sanitizer for xmolpp2")
#        target_compile_options(xmolpp2 PUBLIC -fsanitize=address)
#        target_link_libraries(xmolpp2 PUBLIC asan)
#    endif()
endif()

add_subdirectory(pyxmolpp2)