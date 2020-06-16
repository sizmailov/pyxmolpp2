#pragma once

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