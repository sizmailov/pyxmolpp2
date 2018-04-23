cmake_minimum_required(VERSION 3.5)
project(observable)

set(CMAKE_CXX_STANDARD 14)

find_package(PythonInterp)
find_package(PythonLibs REQUIRED)

if (PYTHONLIBS_FOUND)
    include_directories(${PYTHON_INCLUDE_DIRS})
    message("Using default python version: " ${PYTHONLIBS_VERSION_STRING})
endif()

IF(CMAKE_COMPILER_IS_GNUCC)

    IF(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fno-inline -fno-inline-small-functions -fno-default-inline -g ")
        message("USE COVERAGE PROFILE")
    else()
        message("DONT USE COVERAGE PROFILE")
    ENDIF()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wreturn-type")

    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register") # pybind11 has depricated `register` keyword (not needed with -std=c++-14)

ENDIF(CMAKE_COMPILER_IS_GNUCC)



enable_testing()


add_subdirectory(external/googletest)
include_directories(external/googletest/googletest/include)
include_directories(external/range-v3/include)
include_directories(external/GSL/include)
include_directories(external/eigen-git-mirror)
add_subdirectory(external/pybind11)


include_directories(include)

add_subdirectory(tests)


file(GLOB_RECURSE XMOL_HEADERS include/**.h)
file(GLOB_RECURSE XMOL_SOURCES src/**.cpp src/**.h)

add_library(observable_static STATIC ${XMOL_SOURCES} ${XMOL_HEADERS})
add_library(observable SHARED ${XMOL_SOURCES} ${XMOL_HEADERS})

IF(CMAKE_COMPILER_IS_GNUCC)
    IF(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
        target_link_libraries(observable_static gcov)
        target_link_libraries(observable gcov)
    ENDIF()
ENDIF()
file(GLOB_RECURSE PYXMOL_SOURCES pyxmolpp/**.cpp pyxmolpp/**.h )

pybind11_add_module(pyxmolpp2 ${PYXMOL_SOURCES} )
target_link_libraries(pyxmolpp2 PRIVATE observable)
