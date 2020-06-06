
add_custom_target(run-benchmarks)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/benchmark-results
        COMMAND             cmake -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/benchmark-results
)

function(benchmark_suite name dependencies)
    file(GLOB_RECURSE BenchmarkSources ${name}/*.cpp)
    add_executable(benchmark-${name} ${BenchmarkSources})
    target_link_libraries(benchmark-${name} ${dependencies} benchmark::benchmark_main)
    add_custom_target(run-benchmark-${name}
        COMMAND             ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/run-benchmark.py --benchmark=$<TARGET_FILE:benchmark-${name}> --name=${name}
        DEPENDS             benchmark-${name} ${CMAKE_CURRENT_BINARY_DIR}/benchmark-results
        WORKING_DIRECTORY   ${CMAKE_CURRENT_BINARY_DIR}/benchmark-results
    )
    add_dependencies(run-benchmarks run-benchmark-${name})
endfunction(benchmark_suite)

benchmark_suite(v1 xmolpp2)
