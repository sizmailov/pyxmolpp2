#include <xmol/polymer/Atom.h>
#include "init.h"

#include "xmol/polymer/exceptions.h"

void pyxmolpp::polymer::init_exceptions(pybind11::module& pdb) {
//  auto ex = py::register_exception<xmol::polymer::PolymerException>(pdb, "PolymerException");
  py::register_exception<xmol::polymer::DeadAtomSelectionAccess>(pdb, "DeadAtomSelectionAccess");
  py::register_exception<xmol::polymer::DeadResidueSelectionAccess>(pdb, "DeadResidueSelectionAccess");
  py::register_exception<xmol::polymer::DeadChainSelectionAccess>(pdb, "DeadChainSelectionAccess");

  py::register_exception<xmol::polymer::DeletedAtomAccess>(pdb, "DeletedAtomAccess");
  py::register_exception<xmol::polymer::DeletedResidueAccess>(pdb, "DeletedResidueAccess");
  py::register_exception<xmol::polymer::DeletedChainAccess>(pdb, "DeletedChainAccess");

  py::register_exception<xmol::polymer::OutOfRangeAtomSelection>(pdb, "OutOfRangeAtomSelection");
  py::register_exception<xmol::polymer::OutOfRangeResidueSelection>(pdb, "OutOfRangeResidueSelection");
  py::register_exception<xmol::polymer::OutOfRangeChainSelection>(pdb, "OutOfRangeChainSelection");

  py::register_exception<xmol::polymer::OutOfRangeResidue>(pdb, "OutOfRangeResidue");
  py::register_exception<xmol::polymer::OutOfRangeChain>(pdb, "OutOfRangeChain");
  py::register_exception<xmol::polymer::OutOfRangeFrame>(pdb, "OutOfRangeFrame");

}
