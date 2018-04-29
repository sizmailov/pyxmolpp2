#include "init.h"

#include "xmol/pdb/exceptions.h"

void pyxmolpp::pdb::init_exceptions(pybind11::module& pdb) {
  auto base = py::register_exception<xmol::pdb::PdbException>(pdb, "PdbException");
  py::register_exception<xmol::pdb::PdbFieldReadError>(pdb, "PdbFieldReadError",base.ptr());
  py::register_exception<xmol::pdb::PdbUknownRecord>(pdb, "PdbUknownRecord",base.ptr());
  py::register_exception<xmol::pdb::PdbUknownRecordField>(pdb, "PdbUknownRecordField",base.ptr());

}
