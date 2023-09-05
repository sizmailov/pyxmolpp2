file(GLOB_RECURSE PYXMOLPP_SOURCES
        src/**.cpp
        src/**.h
        )
pybind11_add_module(_core ${PYXMOLPP_SOURCES})
target_include_directories(_core PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)
target_link_libraries(_core PRIVATE xmolpp2)