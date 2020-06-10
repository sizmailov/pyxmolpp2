#pragma once

#include "xmol/Frame.h"
#include <benchmark/benchmark.h>

using namespace xmol;
using namespace xmol::proxy;
using namespace xmol::proxy::smart;

void populate_frame(Frame& frame, int n_molecules, int n_residues, int n_atoms);