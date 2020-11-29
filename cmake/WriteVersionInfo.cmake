
add_custom_target (
        write_version_info
        COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/write_version_info.py
                       ${CMAKE_BINARY_DIR}/xmol_version_info.h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

include_directories(${CMAKE_BINARY_DIR})

execute_process(COMMAND ${Python_EXECUTABLE} tools/print_version_number.py
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE XMOL_VERSION)

message(${XMOL_VERSION})