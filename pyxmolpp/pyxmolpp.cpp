#include "pyxmolpp.h"

#include "xmol/polymer/Atom.h"
#include "xmol/utils/string.h"

namespace py = pybind11;

PYBIND11_MODULE(pyxmolpp2, m) {
  m.doc() = "pyxmolpp2 module";


  {
    pybind11::module geometry = m.def_submodule("geometry", "pyxmolpp2.geometry module");
    pyxmolpp::init_geometry(geometry);
  }
  {
    pybind11::module pdb = m.def_submodule("pdb", "pyxmolpp2.pdb module");
    pyxmolpp::init_pdb(pdb);
  }
  {
    pybind11::module polymer = m.def_submodule("polymer", "pyxmolpp2.polymer module");
    pyxmolpp::init_polymer(polymer);
  }

}
