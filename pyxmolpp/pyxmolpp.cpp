#include "pyxmolpp.h"

#include "xmol/polymer/Atom.h"
#include "xmol/utils/string.h"

namespace py = pybind11;

PYBIND11_MODULE(pyxmolpp2, m) {
  m.doc() = "pyxmolpp2 module";


  pybind11::module version = m.def_submodule("version", "version info");
  pybind11::module geometry = m.def_submodule("geometry", "pyxmolpp2.geometry module");
  pybind11::module crystal = m.def_submodule("crystal", "pyxmolpp2.crystal module");
  pybind11::module polymer = m.def_submodule("polymer", "pyxmolpp2.polymer module");
  pybind11::module pdb = m.def_submodule("pdb", "pyxmolpp2.pdb module");
  pybind11::module trajectory = m.def_submodule("trajectory", "pyxmolpp2.trajectory module");
  pybind11::module trjtool = m.def_submodule("trjtool", "pyxmolpp2.trjtool module");

  pyxmolpp::init_version(version);
  pyxmolpp::init_geometry(geometry);
  pyxmolpp::init_crystal(crystal);
  pyxmolpp::init_pdb_fwd(pdb);
  pyxmolpp::init_polymer(polymer);
  pyxmolpp::init_trajectory(trajectory);
  pyxmolpp::init_pdb(pdb);
  pyxmolpp::init_trjtool(trjtool);


}
