#include "references.h"
#include "pybind11/pybind11.h"
#include "xmol/proxy/smart/references.h"
#include "xmol/proxy/smart/selections.h"
#include "xmol/proxy/smart/spans.h"

namespace py = pybind11;
using namespace xmol;
using namespace xmol::proxy;
using namespace xmol::proxy::smart;

void pyxmolpp::v1::populate(pybind11::class_<Frame>& pyFrame) {
  using SRef = Frame;
  pyFrame
      .def(py::init<>())
      .def(py::init<const SRef&>())
      .def_property_readonly("coords", [](SRef& ref) { return ref.coords().smart(); })
      .def_property_readonly("atoms", [](SRef& ref) { return ref.atoms().smart(); })
      .def_property_readonly("residues", [](SRef& ref) { return ref.residues().smart(); })
      .def_property_readonly("molecules", [](SRef& ref) { return ref.molecules().smart(); })
      .def("add_molecule", [](SRef& ref) { return ref.add_molecule().smart(); })
  ;
}
void pyxmolpp::v1::populate(pybind11::class_<MoleculeSmartRef>& pyMolecule) {
  using SRef = MoleculeSmartRef;
  pyMolecule.def(py::init<const SRef&>())
      .def_property("name", py::overload_cast<>(&SRef::name, py::const_),
                    py::overload_cast<const MoleculeName&>(&SRef::name))
      .def_property_readonly("empty", &SRef::empty)
      .def_property_readonly("size", &SRef::size)
      .def_property_readonly("atoms", [](SRef& ref) { return ref.atoms().smart(); })
      .def_property_readonly("residues", [](SRef& ref) { return ref.residues().smart(); })
      .def_property_readonly("frame", [](SRef& ref) { return ref.frame(); })
      .def("add_residue", [](SRef& ref) { return ref.add_residue().smart(); });
}
void pyxmolpp::v1::populate(pybind11::class_<xmol::proxy::smart::ResidueSmartRef>& pyResidue) {
  using SRef = ResidueSmartRef;
  pyResidue.def(py::init<const SRef&>())
      .def_property("id", py::overload_cast<>(&SRef::id, py::const_), py::overload_cast<const ResidueId&>(&SRef::id))
      .def_property("name", py::overload_cast<>(&SRef::name, py::const_),
                    py::overload_cast<const ResidueName&>(&SRef::name))
      .def_property_readonly("empty", &SRef::empty)
      .def_property_readonly("size", &SRef::size)
      .def_property_readonly("atoms", [](SRef& ref) { return ref.atoms().smart(); })
      .def_property_readonly("molecule", [](SRef& ref) { return ref.molecule(); })
      .def_property_readonly("frame", [](SRef& ref) { return ref.frame(); })
      .def_property_readonly("next", [](SRef& ref) -> std::optional<SRef> { return ref.next(); })
      .def_property_readonly("prev", [](SRef& ref) -> std::optional<SRef> { return ref.prev(); })
      .def("__getitem__", [](SRef& ref, const AtomName& name) { return ref[name]; })
      .def("__getitem__", [](SRef& ref, const std::string& name) { return ref[name.c_str()]; })
      .def("add_atom", [](SRef& ref) { return ref.add_atom().smart(); });
}
void pyxmolpp::v1::populate(pybind11::class_<AtomSmartRef>& pyAtom) {
  using SRef = AtomSmartRef;
  pyAtom.def(py::init<const SRef&>())
      .def_property("id", py::overload_cast<>(&SRef::id, py::const_), py::overload_cast<const AtomId&>(&SRef::id))
      .def_property("name", py::overload_cast<>(&SRef::name, py::const_),
                    py::overload_cast<const AtomName&>(&SRef::name))
      .def_property("r", py::overload_cast<>(&SRef::r, py::const_), py::overload_cast<const XYZ&>(&SRef::r))
      .def_property_readonly("residue", [](SRef& ref) { return ref.residue().smart(); })
      .def_property_readonly("molecule", [](SRef& ref) { return ref.molecule(); })
      .def_property_readonly("frame", [](SRef& ref) { return ref.frame(); })
      .def_property_readonly("__eq__", &SRef::operator==);
}
