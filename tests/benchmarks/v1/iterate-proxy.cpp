#include "common.h"
#include <cassert>

class BM_ProxyIterate : public benchmark::Fixture {
public:
  void SetUp(const ::benchmark::State& state) {
    int n_molecules = state.range(0);
    int n_residues = state.range(1);
    int n_atoms = state.range(2);
    frame.reserve_molecules(n_molecules);
    frame.reserve_residues(n_residues * n_molecules);
    frame.reserve_atoms(n_atoms * n_residues * n_molecules);
    populate_frame(frame, n_molecules, n_residues, n_atoms);
    assert(frame.n_molecules() == n_molecules);
    assert(frame.n_residues() == n_residues * n_molecules);
    assert(frame.n_atoms() == n_residues * n_molecules * n_atoms);
  }

  void TearDown(const ::benchmark::State& state) { frame = {}; }
  Frame frame;
};

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

BENCHMARK_DEFINE_F(BM_ProxyIterate, AtomSmartSelection)(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    AtomSmartSelection atoms(AtomSelection(frame.atoms()));
    state.ResumeTiming();
    for (auto& x : atoms) {
      benchmark::DoNotOptimize(x);
    }
  }
}

BENCHMARK_REGISTER_F(BM_ProxyIterate, AtomSmartSelection)
->Args({0, 0, 0})
    ->Args({1, 1, 1})
    ->Args({2, 2, 2})
    ->Args({10, 10, 2})
    ->Args({40000, 1, 3})
    ->Args({1000, 100, 10});