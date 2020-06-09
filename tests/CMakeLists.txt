function(define_test_suite name dependencies)
    file(GLOB_RECURSE "${name}TestsSources" ${name}/*.cpp)
    message("${name}TestsSources")
    add_executable("${name}Tests" ${${name}TestsSources})
    target_link_libraries("${name}Tests" ${dependencies} gtest_main)
    add_test(NAME "${name}Tests"
            COMMAND "${name}Tests"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tests_dataset"
            )
endfunction(define_test_suite)


define_test_suite(selection xmolpp2)
define_test_suite(polymer xmolpp2)
define_test_suite(pdb xmolpp2)
define_test_suite(trjtool xmolpp2)
define_test_suite(geometry xmolpp2)
define_test_suite(amber xmolpp2)
define_test_suite(v1 xmolpp2)

add_subdirectory(benchmarks)