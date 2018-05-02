#pragma once
#include <pybind11/pybind11.h>

namespace pyxmolpp {

namespace py = pybind11;

void init_version(py::module& version);
void init_geometry(py::module& geometry);
void init_crystal(py::module& crystal);
void init_pdb_fwd(py::module& pdb);
void init_polymer(py::module& polymer);
void init_pdb(py::module& pdb);
void init_selection(py::module& m);
void init_trajectory(py::module& trajectory);
void init_trjtool(py::module& trjtool);
void init_utils(py::module& m);

}
