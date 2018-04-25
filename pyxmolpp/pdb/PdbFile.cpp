#include "init.h"

#include "xmol/pdb/PdbFile.h"
#include "xmol/pdb/PdbReader.h"
#include "xmol/pdb/PdbRecord.h"

using namespace xmol::pdb;
using namespace xmol::polymer;

void pyxmolpp::pdb::init_PdbFile(py::module& pdb) {
  py::class_<PdbFile, xmol::trajectory::TrajectoryPortion>(pdb, "PdbFile")
      .def(py::init<const std::string&>(),py::arg("filename"))
      .def("close", &PdbFile::close, "Release file handle")
      .def("match", &PdbFile::match, py::arg("reference_atoms"),
           "Checks atom names, residue names and residue ids to match reference_atoms")
      .def_property_readonly("n_frames", &PdbFile::n_frames,
                             "Number of frames in PDB file")
      .def_property_readonly("n_atoms_per_frame", &PdbFile::n_atoms_per_frame,
                             "Number of atoms in first frame")
      .def("get_frame",
           (xmol::polymer::Frame (PdbFile::*)())(&PdbFile::get_frame),
           "Read (next) frame from PDB file using standard PDB records")
      .def("get_frame", (xmol::polymer::Frame (PdbFile::*)(
                            const basic_PdbRecords&))(&PdbFile::get_frame),
           py::arg("db"), "Read (next) frame from PDB file using non-standard "
                          "PDB records, db")
      .def("set_coordinates",
           (void (PdbFile::*)(frameIndex_t, const AtomSelection&))(
               &PdbFile::set_coordinates),
           py::arg("frame_index"), py::arg("atoms"),
           "Set atoms coordinates from frame #frame_index");
}
