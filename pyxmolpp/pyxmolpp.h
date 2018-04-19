#pragma once
#include <pybind11/pybind11.h>

namespace pyxmolpp {

namespace py = pybind11;

void init_geometry(py::module& m);
void init_pdb(py::module& m);
void init_polymer(py::module& m);
void init_selection(py::module& m);
void init_trajectory(py::module& m);
void init_trjtool(py::module& m);
void init_utils(py::module& m);

}