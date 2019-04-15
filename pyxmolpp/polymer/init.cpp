#include "../pyxmolpp.h"

#include "init.h"

void pyxmolpp::init_polymer(pybind11::module& polymer) {
  pyxmolpp::polymer::init_exceptions(polymer);
  pyxmolpp::polymer::init_FCRA(polymer);
  pyxmolpp::polymer::init_predicates(polymer);
  pyxmolpp::polymer::init_TorsionAngle(polymer);
}
