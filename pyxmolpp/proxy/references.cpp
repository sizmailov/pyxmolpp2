#include "references.h"
#include "xmol/proxy/smart/references.h"
#include "xmol/proxy/smart/selections.h"
#include "xmol/proxy/smart/spans.h"
#include "xmol/io/pdb/PdbWriter.h"
#include <sstream>
#include <fstream>

#include <pybind11/iostream.h>
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
      .def("add_molecule", [](SRef& ref) { return ref.add_molecule().smart(); })
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
      .def("__repr__", [](Frame& self) {
        std::ostringstream out;
        out << "Frame<addr=" << std::hex << &self << std::dec << ", size=(" << self.n_molecules() << ", "
            << self.n_residues() << ", " << self.n_atoms() << ")>";
        return out.str();
      })
      .def("to_pdb",
           [](Frame& sel, std::string& path) {
             std::ofstream out(path);
             if (out.fail()) {
               throw std::runtime_error("Can't open file `" + path + "` for writing"); // toto: replace with IOError
             }
             xmol::io::pdb::PdbWriter writer(out);
             writer.write(sel);
           },
           py::arg("path_or_buf")
      )
      .def("to_pdb",
           [](Frame& sel, py::object fileHandle) {

             if (!(py::hasattr(fileHandle,"write") &&
                   py::hasattr(fileHandle,"flush") )){
               throw py::type_error("Frame.to_pdb(file): incompatible function argument: `file` must be a file-like object, but `"
                                    +(std::string)(py::repr(fileHandle))+"` provided"
               );
             }
             py::detail::pythonbuf buf(fileHandle);
             std::ostream stream(&buf);
             xmol::io::pdb::PdbWriter writer(stream);
             writer.write(sel);
           },
           py::arg("path_or_buf"),
           "Write PDB file"
      );
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
      .def_property_readonly("index", &SRef::index )
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
      .def(py::self != py::self);
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
      .def_property_readonly("atoms", [](SRef& ref) { return ref.atoms().smart(); })
      .def_property_readonly("molecule", [](SRef& ref) { return ref.molecule().smart(); })
      .def_property_readonly("index", &SRef::index )
      .def_property_readonly(
          "frame", [](SRef& ref) -> Frame& { return ref.frame(); }, py::return_value_policy::reference)
      .def_property_readonly("next", [](SRef& ref) -> std::optional<SRef> { return ref.next(); })
      .def_property_readonly("prev", [](SRef& ref) -> std::optional<SRef> { return ref.prev(); })
      .def("__getitem__",
           [](SRef& ref, const char* name) {
             auto r = ref[name];
             if (r) {
               return r->smart();
             }
             throw py::index_error("No atom with name " + std::string(name));
           })
      .def("add_atom", [](SRef& ref) { return ref.add_atom().smart(); })
      .def(py::self == py::self)
      .def(py::self != py::self);
}
void pyxmolpp::v1::populate(pybind11::class_<AtomSmartRef>& pyAtom) {
  using SRef = AtomSmartRef;
  pyAtom.def(py::init<const SRef&>())
      .def_property("id", py::overload_cast<>(&SRef::id, py::const_), [](SRef& self, AtomId& id) { self.id(id); })
      .def_property("mass", py::overload_cast<>(&SRef::mass, py::const_), [](SRef& self, float value) { self.mass(value); })
      .def_property(
          "name", [](const SRef& self) { return self.name().str(); },
          [](SRef& self, std::string& name) { self.name(AtomName(name)); })
      .def_property("r", py::overload_cast<>(&SRef::r, py::const_), [](SRef& self, const XYZ& r) { self.r(r); })
      .def_property_readonly("residue", [](SRef& ref) { return ref.residue().smart(); })
      .def_property_readonly("molecule", [](SRef& ref) { return ref.molecule().smart(); })
      .def_property_readonly("index", &SRef::index )
      .def_property_readonly(
          "frame", [](SRef& ref) -> Frame& { return ref.frame(); }, py::return_value_policy::reference)
      .def_property_readonly("__eq__", &SRef::operator==)
      .def(py::self == py::self)
      .def(py::self != py::self);
}
