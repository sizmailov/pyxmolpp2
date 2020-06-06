#pragma once

#include "xmol/polymer/Atom.h"
#include <benchmark/benchmark.h>

using namespace xmol::polymer;

void populate_frame(Frame& frame, int n_molecules, int n_residues, int n_atoms);