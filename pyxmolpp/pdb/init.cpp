#include "../pyxmolpp.h"

#include "init.h"

void pyxmolpp::init_pdb(pybind11::module& pdb) {
  pyxmolpp::pdb::init_PDBFile(pdb);
}