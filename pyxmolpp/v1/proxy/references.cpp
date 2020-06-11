#include "references.h"
#include "xmol/proxy/smart/references.h"
#include "xmol/proxy/smart/selections.h"
#include "xmol/proxy/smart/spans.h"
#include <sstream>

#include <pybind11/operators.h>

namespace py = pybind11;
using namespace xmol;
using namespace xmol::proxy;
using namespace xmol::proxy::smart;

void pyxmolpp::v1::populate(pybind11::class_<Frame>& pyFrame) {
  using SRef = Frame;
  pyFrame.def(py::init<>())
      .def(py::init<const SRef&>())
      .def_property_readonly("coords", [](SRef& ref) { return ref.coords().smart(); })
      .def_property_readonly("atoms", [](SRef& ref) { return ref.atoms().smart(); })
      .def_property_readonly("residues", [](SRef& ref) { return ref.residues().smart(); })
      .def_property_readonly("molecules", [](SRef& ref) { return ref.molecules().smart(); })
      .def("add_molecule", [](SRef& ref) { return ref.add_molecule().smart(); })
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def("__repr__", [](Frame& self) {
        std::ostringstream out;
        out << "Frame<addr=" << std::hex << &self << ", size=(" << self.n_molecules() << ", " << self.n_residues()
            << ", " << self.n_atoms() << ")>";
        return out.str();
      });
}
void pyxmolpp::v1::populate(pybind11::class_<MoleculeSmartRef>& pyMolecule) {
  using SRef = MoleculeSmartRef;
  pyMolecule.def(py::init<const SRef&>())
      .def_property(
          "name", [](const SRef& self) { return self.name().str(); },
          [](SRef& self, std::string& name) { self.name(MoleculeName(name)); })
      .def_property_readonly("empty", &SRef::empty)
      .def_property_readonly("size", &SRef::size)
      .def_property_readonly("atoms", [](SRef& ref) { return ref.atoms().smart(); })
      .def_property_readonly("residues", [](SRef& ref) { return ref.residues().smart(); })
      .def_property_readonly("frame", [](SRef& ref) -> Frame& { return ref.frame(); }, py::return_value_policy::reference)
      .def("add_residue", [](SRef& ref) { return ref.add_residue().smart(); })
      .def(py::self == py::self)
      .def(py::self != py::self);
}
void pyxmolpp::v1::populate(pybind11::class_<xmol::proxy::smart::ResidueSmartRef>& pyResidue) {
  using SRef = ResidueSmartRef;
  pyResidue.def(py::init<const SRef&>())
      .def_property("id", py::overload_cast<>(&SRef::id, py::const_), [](SRef& self, ResidueId& id) { self.id(id); })
      .def_property(
          "name", [](const SRef& self) { return self.name().str(); },
          [](SRef& self, std::string& name) { self.name(ResidueName(name)); })
      .def_property_readonly("empty", &SRef::empty)
      .def_property_readonly("size", &SRef::size)
      .def_property_readonly("atoms", [](SRef& ref) { return ref.atoms().smart(); })
      .def_property_readonly("molecule", [](SRef& ref) { return ref.molecule().smart(); })
      .def_property_readonly("frame", [](SRef& ref) -> Frame&  { return ref.frame(); }, py::return_value_policy::reference)
      .def_property_readonly("next", [](SRef& ref) -> std::optional<SRef> { return ref.next(); })
      .def_property_readonly("prev", [](SRef& ref) -> std::optional<SRef> { return ref.prev(); })
      .def("__getitem__", [](SRef& ref, const AtomName& name) { return ref[name]; })
      .def("__getitem__", [](SRef& ref, const std::string& name) { return ref[name.c_str()]; })
      .def("add_atom", [](SRef& ref) { return ref.add_atom().smart(); })
      .def(py::self == py::self)
      .def(py::self != py::self);
}
void pyxmolpp::v1::populate(pybind11::class_<AtomSmartRef>& pyAtom) {
  using SRef = AtomSmartRef;
  pyAtom.def(py::init<const SRef&>())
      .def_property("id", py::overload_cast<>(&SRef::id, py::const_), [](SRef& self, AtomId& id) { self.id(id); })
      .def_property(
          "name", [](const SRef& self) { return self.name().str(); },
          [](SRef& self, std::string& name) { self.name(AtomName(name)); })
      .def_property("r", py::overload_cast<>(&SRef::r, py::const_), [](SRef& self, const XYZ& r) { self.r(r); })
      .def_property_readonly("residue", [](SRef& ref) { return ref.residue().smart(); })
      .def_property_readonly("molecule", [](SRef& ref) { return ref.molecule().smart(); })
      .def_property_readonly("frame", [](SRef& ref) -> Frame& { return ref.frame(); }, py::return_value_policy::reference)
      .def_property_readonly("__eq__", &SRef::operator==)
      .def(py::self == py::self)
      .def(py::self != py::self);
}
