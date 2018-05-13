#include "init.h"

#include "xmol/trjtool/DATReader.h"
#include "xmol/trjtool/DatFile.h"

#include <pybind11/stl.h>
#include <fstream>

using namespace xmol::trjtool;
using namespace xmol::polymer;

void pyxmolpp::trjtool::init_DatFile(py::module& trjtool) {

  py::class_<DatFile, xmol::trajectory::TrajectoryPortion>(trjtool, "DatFile", "Represents `.dat` file on disk")
      .def(py::init<const std::string&>(),py::arg("filename"))
      .def("close", &DatFile::close, "Release file handle")
      .def("match", &DatFile::match, py::arg("atoms"), "Checks atom names, residue names and "
                                     "residue ids to match reference_atoms")
      .def_property_readonly("n_frames", &DatFile::n_frames, "Number of frames in PDB file")
      .def_property_readonly("n_atoms_per_frame", &DatFile::n_atoms_per_frame, "Number of atoms per frame")
      .def("set_coordinates", &DatFile::set_coordinates, "Set atoms coordinates from frame #frame_index");
}
