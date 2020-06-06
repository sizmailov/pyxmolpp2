#pragma once

#include "common.h"
#include <cassert>

class BM_ProxyIterate : public benchmark::Fixture {
public:
  void SetUp(const ::benchmark::State& state) {
    int n_molecules = state.range(0);
    int n_residues = state.range(1);
    int n_atoms = state.range(2);
    populate_frame(frame, n_molecules, n_residues, n_atoms);
  }

  void TearDown(const ::benchmark::State& state) { frame = Frame(0); }

  Frame frame{0};
};