#include "init.h"

#include "xmol/pdb/PdbFile.h"
#include "xmol/pdb/PdbReader.h"
#include "xmol/pdb/PdbRecord.h"

#include "pybind11/stl.h"

using namespace xmol::pdb;
using namespace xmol::polymer;

void pyxmolpp::pdb::init_PdbFile(py::module& pdb) {
  py::class_<PdbFile, xmol::trajectory::TrajectoryPortion>(pdb, "PdbFile")
      .def(py::init<const std::string&>(),py::arg("filename"),"Create PDB file with standard PDB records")
      .def(py::init<const std::string&, const basic_PdbRecords&>(), py::arg("filename"),py::arg("db"),
          R"pydoc(Create PDB file with non-standard PDB records

:param db: Non-standard PDB records
)pydoc")
      .def("close", (void (PdbFile::*)())&PdbFile::close, "Release file handle")
      .def("match", &PdbFile::match, py::arg("reference_atoms"),
           "Checks atom names, residue names and residue ids to match reference_atoms")
      .def_property_readonly("n_frames", &PdbFile::n_frames,
                             "Number of frames in PDB file")
      .def_property_readonly("n_atoms_per_frame", &PdbFile::n_atoms_per_frame,
                             "Number of atoms in first frame")
      .def("get_frame",&PdbFile::get_frame,"Read first frame from PDB file")
      .def("get_frames",&PdbFile::get_frames,"Read all frames from PDB file")
      .def("set_coordinates",
           (void (PdbFile::*)(frameIndex_t, const AtomSelection&))(
               &PdbFile::set_coordinates),
           py::arg("frame_index"), py::arg("atoms"),
           "Set atoms coordinates from frame #frame_index");
}
