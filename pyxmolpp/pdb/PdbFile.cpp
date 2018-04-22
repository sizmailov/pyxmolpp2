#include "init.h"

#include "xmol/pdb/PdbFile.h"
#include "xmol/pdb/PdbReader.h"

using namespace xmol::pdb;
using namespace xmol::polymer;

void pyxmolpp::pdb::init_PDBFile(py::module& pdb) {
  py::class_<PdbFile,xmol::trajectory::TrajectoryPortion>(pdb,"PdbFile")
      .def(py::init<const std::string&>())
      .def("close",&PdbFile::close)
      .def("match",&PdbFile::match)
      .def("n_frames",&PdbFile::n_frames)
      .def("n_atoms_per_frame",&PdbFile::n_atoms_per_frame)
      .def("get_frame",(xmol::polymer::Frame (PdbFile::*)())(&PdbFile::get_frame))
      .def("set_coordinates",(void (PdbFile::*)(frameIndex_t, const AtomSelection&))(&PdbFile::set_coordinates))
      ;
}
