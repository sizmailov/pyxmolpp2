#pragma once

#include "xmol/v1/Frame.h"
#include <benchmark/benchmark.h>

using namespace xmol::v1;
using namespace xmol::v1::proxy;
using namespace xmol::v1::proxy::smart;

void populate_frame(Frame& frame, int n_molecules, int n_residues, int n_atoms);