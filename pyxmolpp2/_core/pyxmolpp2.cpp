#include "init.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(_core, m) {
  m.doc() = "pyxmolpp2 implementation";
  pyxmolpp::v1::init(m);
}
