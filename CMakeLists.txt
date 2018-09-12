cmake_minimum_required(VERSION 3.5)
project(observable)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PythonInterp REQUIRED)
find_package(PythonLibs "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}" REQUIRED )
if (PYTHONLIBS_FOUND)
    include_directories(${PYTHON_INCLUDE_DIRS})
    message("Using default python version: " ${PYTHONLIBS_VERSION_STRING})
endif()

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

enable_testing()


add_subdirectory(external/googletest)
include_directories(SYSTEM external/googletest/googletest/include)
include_directories(SYSTEM external/range-v3/include)
include_directories(SYSTEM external/GSL/include)
include_directories(SYSTEM external/eigen-git-mirror)
add_subdirectory(external/pybind11)
include_directories(SYSTEM external/pybind11/include)


include_directories(include)

add_subdirectory(tests)

include(WriteVersionInfo.cmake)

file(GLOB_RECURSE XMOL_HEADERS include/**.h)
file(GLOB_RECURSE XMOL_SOURCES src/**.cpp src/**.h)

add_library(observable_static STATIC ${XMOL_SOURCES} ${XMOL_HEADERS})
add_library(observable SHARED ${XMOL_SOURCES} ${XMOL_HEADERS})

set_target_properties(observable_static PROPERTIES SUFFIX "-${XMOL_VERSION}.a")
set_target_properties(observable PROPERTIES SUFFIX "-${XMOL_VERSION}.so")

add_dependencies(observable_static write_version_info)
add_dependencies(observable write_version_info)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_link_libraries(observable stdc++ m)
    target_link_libraries(observable_static stdc++ m)
endif()


IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    IF(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
        target_link_libraries(observable_static gcov)
        target_link_libraries(observable gcov)
    ENDIF()
endif()

file(GLOB_RECURSE PYXMOL_SOURCES pyxmolpp/**.cpp pyxmolpp/**.h )

pybind11_add_module(pyxmolpp2 ${PYXMOL_SOURCES} )
target_link_libraries(pyxmolpp2 PRIVATE observable)
