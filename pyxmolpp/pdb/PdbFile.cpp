#include "init.h"

#include "xmol/pdb/PdbFile.h"

#include "xmol/pdb/PdbReader.h"

using namespace xmol::pdb;

void pyxmolpp::pdb::init_PDBFile(py::module& pdb) {
  py::class_<PdbFile>(pdb,"PdbFile")
      .def(py::init<const std::string&>())
      .def("get_frame",(xmol::polymer::Frame (PdbFile::*)())(&PdbFile::get_frame));
}