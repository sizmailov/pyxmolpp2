#include "init.h"

void pyxmolpp::init_crystal(pybind11::module& m) {
  crystal::init_lattice_vectors(m);
  crystal::init_best_shift_finder(m);
}
