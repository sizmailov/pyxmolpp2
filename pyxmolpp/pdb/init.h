#pragma once
#include "../pyxmolpp.h"

namespace pyxmolpp{ namespace pdb {

void init_exceptions(py::module& pdb);
void init_PdbRecords(py::module& pdb);
void init_PdbFile(py::module& pdb);

}
}

