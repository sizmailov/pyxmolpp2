
add_custom_target(run-benchmarks)

add_custom_target(create-benchmark-output-dir
        COMMAND             ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/.benchmark-results
)

function(benchmark_suite name dependencies)
    file(GLOB_RECURSE BenchmarkSources ${name}/*.cpp)
    add_executable(benchmark-${name} ${BenchmarkSources})
    target_link_libraries(benchmark-${name} ${dependencies} benchmark::benchmark_main)
    add_custom_target(run-benchmark-${name}
        COMMAND             ${CMAKE_COMMAND} -E env TEST_DATA_PATH=${PROJECT_SOURCE_DIR}/tests_dataset ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/run-benchmark.py --benchmark=$<TARGET_FILE:benchmark-${name}> --name=${name}
        DEPENDS             benchmark-${name} create-benchmark-output-dir
        WORKING_DIRECTORY   ${PROJECT_SOURCE_DIR}/.benchmark-results
    )
    add_dependencies(run-benchmarks run-benchmark-${name})
endfunction(benchmark_suite)

benchmark_suite(v1 xmolpp2)
