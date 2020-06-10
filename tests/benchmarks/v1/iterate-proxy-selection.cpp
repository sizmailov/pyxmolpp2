#include "iterate-proxy.h"
#include "xmol/proxy/smart/selections.h"

BENCHMARK_DEFINE_F(BM_ProxyIterate, AtomSelection)(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    AtomSelection atoms(frame.atoms());
    state.ResumeTiming();
    for (auto& x : atoms) {
      benchmark::DoNotOptimize(x);
    }
  }
}

BENCHMARK_REGISTER_F(BM_ProxyIterate, AtomSelection)
    ->Args({0, 0, 0})
    ->Args({1, 1, 1})
    ->Args({2, 2, 2})
    ->Args({10, 10, 2})
    ->Args({40000, 1, 3})
    ->Args({1000, 100, 10});

BENCHMARK_DEFINE_F(BM_ProxyIterate, AtomSelectionByIndex)(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    AtomSelection atoms(frame.atoms());
    state.ResumeTiming();
    for (int i = 0; i < atoms.size(); ++i) {
      benchmark::DoNotOptimize(atoms[i]);
    }
  }
}

BENCHMARK_REGISTER_F(BM_ProxyIterate, AtomSelectionByIndex)
    ->Args({0, 0, 0})
    ->Args({1, 1, 1})
    ->Args({2, 2, 2})
    ->Args({10, 10, 2})
    ->Args({40000, 1, 3})
    ->Args({1000, 100, 10});
