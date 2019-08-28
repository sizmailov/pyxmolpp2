#include "init.h"
#include "xmol/amber/NetCDFTrajectoryFile.h"
#include <pybind11/stl.h>

using namespace xmol::polymer;
using namespace xmol::amber;

void pyxmolpp::amber::init_NetCDFTrajectoryFile(pybind11::module& amber) {
    py::class_<NetCDFTrajectoryFile, xmol::trajectory::TrajectoryPortion>(amber, "NetCDFTrajectoryFile",
        "Represents `.nc` file on disk")
        .def(py::init<const std::string&>(), py::arg("filename"))
        .def("close", &NetCDFTrajectoryFile::close, "Release file handle")
        .def("match", &NetCDFTrajectoryFile::match, py::arg("atoms"), "Checks atom names, residue names and "
                                                                      "residue ids to match reference_atoms")
        .def_property_readonly("n_frames", &NetCDFTrajectoryFile::n_frames, "Number of frames in PDB file")
        .def_property_readonly("n_atoms_per_frame", &NetCDFTrajectoryFile::n_atoms_per_frame,
            "Number of atoms per frame")
        .def("set_coordinates", &NetCDFTrajectoryFile::set_coordinates,
            "Set atoms coordinates from frame #frame_index");

}
