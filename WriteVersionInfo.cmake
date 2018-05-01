
add_custom_target (
        write_version_info
        COMMAND python ${CMAKE_SOURCE_DIR}/write_version_info.py
                       ${CMAKE_BINARY_DIR}/xmol_version_info.h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

include_directories(${CMAKE_BINARY_DIR})