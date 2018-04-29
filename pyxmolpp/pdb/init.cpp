#include "../pyxmolpp.h"

#include "init.h"

void pyxmolpp::init_pdb_fwd(pybind11::module& pdb) {
  pyxmolpp::pdb::init_exceptions(pdb);
  pyxmolpp::pdb::init_PdbRecords(pdb);
}

void pyxmolpp::init_pdb(pybind11::module& pdb) {
  pyxmolpp::pdb::init_PdbFile(pdb);
}

