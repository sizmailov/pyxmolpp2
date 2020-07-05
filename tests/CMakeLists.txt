function(define_test_suite name dependencies)
    file(GLOB_RECURSE "${name}TestsSources" ${name}/*.cpp)
    message("${name}TestsSources")
    add_executable("${name}Tests" ${${name}TestsSources})
    target_link_libraries("${name}Tests" PUBLIC  ${dependencies} gtest_main)
    target_include_directories("${name}Tests" PUBLIC "${CMAKE_CURRENT_LIST_DIR}/${name}")
    add_test(NAME "${name}Tests"
            COMMAND "${name}Tests"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tests_dataset"
            )
endfunction(define_test_suite)


define_test_suite(v1 xmolpp2)

add_subdirectory(benchmarks)