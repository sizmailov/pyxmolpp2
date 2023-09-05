cmake_minimum_required(VERSION 3.5)
project(pyxmolpp2)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT DEFINED PYBIND_PYTHON_VERSION)
    message(FATAL_ERROR, "PYBIND_PYTHON_VERSION is unset. Hint: Add '-DPYBIND_PYTHON_VERSION=...' to cmake command")
endif()

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

find_package(PkgConfig REQUIRED)
pkg_search_module(PKG_TRIPC REQUIRED IMPORTED_TARGET libtirpc)

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

file(GLOB_RECURSE XMOL_HEADERS include/**.h)
file(GLOB_RECURSE XMOL_SOURCES src/**.cpp src/**.h)

add_library(xmolpp2 STATIC ${XMOL_SOURCES} ${XMOL_HEADERS})
target_include_directories(xmolpp2 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
set_target_properties(xmolpp2 PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(
        xmolpp2
    PUBLIC
        NetCDF::NetCDF
        PkgConfig::PKG_TRIPC
)

#target_compile_definitions(xmolpp2_static PRIVATE GSL_THROW_ON_CONTRACT_VIOLATION)
#target_compile_definitions(xmolpp2 PRIVATE GSL_THROW_ON_CONTRACT_VIOLATION)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_link_libraries(xmolpp2 PUBLIC stdc++ m)
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

add_subdirectory(bindings)