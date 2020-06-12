#pragma once

#include <pybind11/pybind11.h>

namespace pyxmolpp::v1 {

void define_algo_functions(pybind11::module& coords);

}