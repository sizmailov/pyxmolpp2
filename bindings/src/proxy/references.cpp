#include "references.h"
#include "to_pdb_shortcuts.h"
#include "xmol/proxy/smart/references.h"
#include "xmol/proxy/smart/selections.h"
#include "xmol/proxy/smart/spans.h"

#include <sstream>

#include <pybind11/operators.h>
#include <pybind11/stl.h>

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
      .def_readwrite("cell", &SRef::cell)
      .def_readwrite("index", &SRef::index, "Zero-based index in trajectory")
      .def_readwrite("time", &SRef::time, "Time point in trajectory, a.u.")
      .def("add_molecule", [](SRef& ref) { return ref.add_molecule().smart(); })
      .def("to_pdb", to_pdb_file<SRef>, py::arg("path_or_buf"))
      .def("to_pdb", to_pdb_stream<SRef>, py::arg("path_or_buf"))
      .def("__getitem__",
           [](SRef& ref, const char* name) {
             auto r = ref[name];
             if (r) {
               return r->smart();
             }
             throw py::index_error("No molecule with name " + std::string(name));
           })
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def("__hash__", [](SRef& self){ return std::hash<SRef*>{}(&self); })
      .def("__str__", [](Frame& self) {
        return "Frame<n_mol=" + std::to_string(self.n_molecules()) + ", n_res=" + std::to_string(self.n_molecules()) +
               ", n_ats=" + std::to_string(self.n_atoms()) + ">";
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
      .def_property_readonly("coords", [](SRef& ref) { return ref.coords().smart(); })
      .def_property_readonly("atoms", [](SRef& ref) { return ref.atoms().smart(); })
      .def_property_readonly("residues", [](SRef& ref) { return ref.residues().smart(); })
      .def_property_readonly("index", &SRef::index )
      .def("to_pdb", to_pdb_file<SRef>, py::arg("path_or_buf"))
      .def("to_pdb", to_pdb_stream<SRef>, py::arg("path_or_buf"))
      .def_property_readonly(
          "frame", [](SRef& ref) -> Frame& { return ref.frame(); }, py::return_value_policy::reference)
      .def("__getitem__",
           [](SRef& ref, const ResidueId& id) {
             auto r = ref[id];
             if (r) {
               return r->smart();
             }
             throw py::index_error("No residue with id " + std::to_string(id.serial) + id.iCode.str());
           })
      .def("__getitem__",
           [](SRef& ref, int id) {
             auto r = ref[id];
             if (r) {
               return r->smart();
             }
             throw py::index_error("No residue with id " + std::to_string(id));
           })
      .def("add_residue", [](SRef& ref) { return ref.add_residue().smart(); })
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def("__hash__", [](SRef& self){ return self.index(); })
      .def("__repr__",
           [](SRef& self) {
             return "Molecule<name=" + self.name().str() + ", size=" + std::to_string(self.size()) + ">";
           })
      .def("__str__", [](SRef& self) { return to_string(self); });
}
void pyxmolpp::v1::populate(pybind11::class_<ResidueSmartRef>& pyResidue) {
  using SRef = ResidueSmartRef;
  pyResidue.def(py::init<const SRef&>())
      .def_property("id", py::overload_cast<>(&SRef::id, py::const_), [](SRef& self, ResidueId& id) { self.id(id); })
      .def_property(
          "name", [](const SRef& self) { return self.name().str(); },
          [](SRef& self, std::string& name) { self.name(ResidueName(name)); })
      .def_property_readonly("empty", &SRef::empty)
      .def_property_readonly("size", &SRef::size)
      .def_property_readonly("coords", [](SRef& ref) { return ref.coords().smart(); })
      .def_property_readonly("atoms", [](SRef& ref) { return ref.atoms().smart(); })
      .def_property_readonly("molecule", [](SRef& ref) { return ref.molecule().smart(); })
      .def_property_readonly("index", &SRef::index)
      .def_property_readonly(
          "frame", [](SRef& ref) -> Frame& { return ref.frame(); }, py::return_value_policy::reference)
      .def_property_readonly("next", [](SRef& ref) -> std::optional<SRef> { return ref.next(); })
      .def_property_readonly("prev", [](SRef& ref) -> std::optional<SRef> { return ref.prev(); })
      .def("to_pdb", to_pdb_file<SRef>, py::arg("path_or_buf"))
      .def("to_pdb", to_pdb_stream<SRef>, py::arg("path_or_buf"))
      .def("add_atom", [](SRef& ref) { return ref.add_atom().smart(); })
      .def("__getitem__",
           [](SRef& ref, const char* name) {
             auto r = ref[name];
             if (r) {
               return r->smart();
             }
             throw py::index_error("No atom with name " + std::string(name));
           })
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def("__hash__", [](SRef& self){ return self.index(); })
      .def("__repr__",
           [](SRef& self) {
             return "Residue<name=" + self.name().str() + ", size=" + std::to_string(self.size()) + ">";
           })
      .def("__str__", [](SRef& self) { return to_string(self); });
}
void pyxmolpp::v1::populate(pybind11::class_<AtomSmartRef>& pyAtom) {
  using SRef = AtomSmartRef;
  pyAtom.def(py::init<const SRef&>())
      .def_property("id", py::overload_cast<>(&SRef::id, py::const_), [](SRef& self, AtomId& id) { self.id(id); })
      .def_property("mass", py::overload_cast<>(&SRef::mass, py::const_), [](SRef& self, float value) { self.mass(value); })
      .def_property("vdw_radius", py::overload_cast<>(&SRef::vdw_radius, py::const_), [](SRef& self, float value) { self.vdw_radius(value); })
      .def_property(
          "name", [](const SRef& self) { return self.name().str(); },
          [](SRef& self, std::string& name) { self.name(AtomName(name)); })
      .def_property("r", py::overload_cast<>(&SRef::r, py::const_), [](SRef& self, const XYZ& r) { self.r(r); })
      .def_property_readonly("residue", [](SRef& ref) { return ref.residue().smart(); })
      .def_property_readonly("molecule", [](SRef& ref) { return ref.molecule().smart(); })
      .def_property_readonly("index", &SRef::index)
      .def_property_readonly(
          "frame", [](SRef& ref) -> Frame& { return ref.frame(); }, py::return_value_policy::reference)
      .def("to_pdb", to_pdb_file<SRef>, py::arg("path_or_buf"))
      .def("to_pdb", to_pdb_stream<SRef>, py::arg("path_or_buf"))
      .def_property_readonly("__eq__", &SRef::operator==)
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def("__hash__", [](SRef& self){ return self.index(); })
      .def("__repr__",
           [](SRef& self) {
             return "Atom<name=" + self.name().str() + ", id=" + std::to_string(self.id())  + ">";
           })
      .def("__str__", [](SRef& self) { return to_string(self); });
}
