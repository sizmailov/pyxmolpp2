#include "iterate-proxy.h"

BENCHMARK_DEFINE_F(BM_ProxyIterate, AtomRefSpan)(benchmark::State& state) {
  for (auto _ : state) {
    for (auto& x : frame.atoms()) {
      benchmark::DoNotOptimize(x);
    }
  }
}

BENCHMARK_REGISTER_F(BM_ProxyIterate, AtomRefSpan)
    ->Args({0, 0, 0})
    ->Args({1, 1, 1})
    ->Args({2, 2, 2})
    ->Args({10, 10, 2})
    ->Args({40000, 1, 3})
    ->Args({1000, 100, 10});

BENCHMARK_DEFINE_F(BM_ProxyIterate, AtomRefSpanByIndex)(benchmark::State& state) {
  for (auto _ : state) {
    auto atoms = frame.atoms();
    for (int i = 0; i < atoms.size(); ++i) {
      benchmark::DoNotOptimize(atoms[i]);
    }
  }
}

BENCHMARK_REGISTER_F(BM_ProxyIterate, AtomRefSpanByIndex)
    ->Args({0, 0, 0})
    ->Args({1, 1, 1})
    ->Args({2, 2, 2})
    ->Args({10, 10, 2})
    ->Args({40000, 1, 3})
    ->Args({1000, 100, 10});