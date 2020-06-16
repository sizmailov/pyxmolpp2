#include "common.h"

enum Reserve { withReserve, woReserve };

template <Reserve reserve> static void BM_FrameCreate(benchmark::State& state) {
  for (auto _ : state) {
    Frame frame;
    int n_molecules = state.range(0);
    int n_residues = state.range(1);
    int n_atoms = state.range(2);
    if (reserve == withReserve) {
      frame.reserve_molecules(n_molecules);
      frame.reserve_residues(n_residues * n_molecules);
      frame.reserve_atoms(n_atoms * n_residues * n_molecules);
    }
    populate_frame(frame, n_molecules, n_residues, n_atoms);
  }
}

BENCHMARK_TEMPLATE(BM_FrameCreate, woReserve)
    ->Args({0, 0, 0})
    ->Args({1, 1, 1})
    ->Args({2, 2, 2})
    ->Args({10, 10, 2})
    ->Args({40000, 1, 3})
    ->Args({1000, 100, 10});

BENCHMARK_TEMPLATE(BM_FrameCreate, withReserve)
    ->Args({0, 0, 0})
    ->Args({1, 1, 1})
    ->Args({2, 2, 2})
    ->Args({10, 10, 2})
    ->Args({40000, 1, 3})
    ->Args({1000, 100, 10});
