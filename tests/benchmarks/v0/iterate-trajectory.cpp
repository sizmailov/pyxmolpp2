#include "iterate-trajectory.h"

BENCHMARK_DEFINE_F(BM_TrajectoryTrjtool, Stride)(benchmark::State& state) {
  for (auto& x : *trj_ptr) { // warm-up
    benchmark::DoNotOptimize(x);
  }
  for (auto _ : state) {
    for (auto& x : trj_ptr->slice(state.range(0),state.range(1),state.range(2))) {
      benchmark::DoNotOptimize(x);
    }
  }
}

BENCHMARK_REGISTER_F(BM_TrajectoryTrjtool, Stride)
    ->Args({0, 2000, 1})
    ->Args({1000, 2000, 1})
    ->Args({0, 2000, 10})
    ->Args({0, 2000, 100})
    ->Args({0, 2000, 1000});

