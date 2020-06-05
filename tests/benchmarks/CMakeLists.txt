function(benchmark_suite name dependencies)
    file(GLOB_RECURSE "BenchmarkSources" ${name}/*.cpp)
    add_executable("benchmark-${name}" ${BenchmarkSources})
    target_link_libraries("benchmark-${name}" ${dependencies} benchmark::benchmark_main)
endfunction(benchmark_suite)

benchmark_suite(v1 xmolpp2)
