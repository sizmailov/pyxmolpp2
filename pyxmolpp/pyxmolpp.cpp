#include "init.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(pyxmolpp2, m) {
  m.doc() = "pyxmolpp2 module";
  pyxmolpp::v1::init(m);
}
