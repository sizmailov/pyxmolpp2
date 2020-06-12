#include "selections.h"
#include "v1/iterator-helpers.h"
#include "xmol/proxy/smart/references.h"
#include "xmol/proxy/smart/selections.h"

#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>

namespace py = pybind11;
using namespace xmol;
using namespace xmol::proxy;
using namespace xmol::proxy::smart;

void pyxmolpp::v1::populate(pybind11::class_<xmol::proxy::smart::CoordSmartSelection>& pyCoordSelection) {
  using Sel = CoordSmartSelection;
  pyCoordSelection.def(py::init<Sel>())
      .def_property_readonly("size", &Sel::size)
      .def_property_readonly("empty", &Sel::empty)
      .def("filter", [](Sel& sel, const std::function<bool(const XYZ&)>& f) { return sel.filter(f).smart(); })
      .def("__len__", &Sel::size)
      //      .def("__contains__", &Sel::contains)
      .def("__getitem__", [](Sel& sel, size_t i) { return XYZ(sel[i]); })
      .def(
          "__iter__", [](Sel& s) { return common::make_coord_value_iterator(s.begin(), s.end()); },
          py::keep_alive<0, 1>())
      .def_property("values", py::overload_cast<>(&Sel::_eigen),
                    py::overload_cast<const CoordEigenMatrix&>(&Sel::_eigen));
}
void pyxmolpp::v1::populate(pybind11::class_<xmol::proxy::smart::AtomSmartSelection>& pyAtomSelection) {
  using Sel = AtomSmartSelection;
  pyAtomSelection.def(py::init<Sel>())
      .def_property_readonly("size", &Sel::size)
      .def_property_readonly("empty", &Sel::empty)
      .def("coords", [](Sel& sel) { return sel.coords().smart(); })
      .def("residues", [](Sel& sel) { return sel.residues().smart(); })
      .def("molecules", [](Sel& sel) { return sel.molecules().smart(); })
      .def("filter", [](Sel& sel, const std::function<bool(const AtomSmartRef&)>& f) { return sel.filter(f).smart(); })
      .def("__len__", &Sel::size)
      .def("__contains__", &Sel::contains)
      .def("__getitem__", [](Sel& sel, size_t i) { return sel[i].smart(); })
      .def(
          "__iter__", [](Sel& s) { return common::make_smart_iterator(s.begin(), s.end()); }, py::keep_alive<0, 1>())
      .def("__or__", [](Sel& lhs, Sel& rhs) { return (lhs | rhs).smart(); })
      .def("__and__", [](Sel& lhs, Sel& rhs) { return (lhs & rhs).smart(); })
      .def("__sub__", [](Sel& lhs, Sel& rhs) { return (lhs - rhs).smart(); });
}
void pyxmolpp::v1::populate(pybind11::class_<xmol::proxy::smart::ResidueSmartSelection>& pyResidueSelection) {
  using Sel = ResidueSmartSelection;
  pyResidueSelection.def(py::init<Sel>())
      .def_property_readonly("size", &Sel::size)
      .def_property_readonly("empty", &Sel::empty)
      .def("filter",
           [](Sel& sel, const std::function<bool(const ResidueSmartRef&)>& f) { return sel.filter(f).smart(); })
      .def("coords", [](Sel& sel) { return sel.coords().smart(); })
      .def("atoms", [](Sel& sel) { return sel.atoms().smart(); })
      .def("molecules", [](Sel& sel) { return sel.molecules().smart(); })
      .def("__len__", &Sel::size)
      .def("__contains__", &Sel::contains)
      .def("__getitem__", [](Sel& sel, size_t i) { return sel[i].smart(); })
      .def(
          "__iter__", [](Sel& s) { return common::make_smart_iterator(s.begin(), s.end()); }, py::keep_alive<0, 1>())
      .def("__or__", [](Sel& lhs, Sel& rhs) { return (lhs | rhs).smart(); })
      .def("__and__", [](Sel& lhs, Sel& rhs) { return (lhs & rhs).smart(); })
      .def("__sub__", [](Sel& lhs, Sel& rhs) { return (lhs - rhs).smart(); });
}
void pyxmolpp::v1::populate(pybind11::class_<xmol::proxy::smart::MoleculeSmartSelection>& pyMoleculeSelection) {
  using Sel = MoleculeSmartSelection;
  pyMoleculeSelection.def(py::init<Sel>())
      .def_property_readonly("size", &Sel::size)
      .def_property_readonly("empty", &Sel::empty)
      .def("filter",
           [](Sel& sel, const std::function<bool(const MoleculeSmartRef&)>& f) { return sel.filter(f).smart(); })
      .def("coords", [](Sel& sel) { return sel.coords().smart(); })
      .def("atoms", [](Sel& sel) { return sel.atoms().smart(); })
      .def("residues", [](Sel& sel) { return sel.residues().smart(); })
      .def("__len__", &Sel::size)
      .def("__contains__", &Sel::contains)
      .def("__getitem__", [](Sel& sel, size_t i) { return sel[i].smart(); })
      .def(
          "__iter__", [](Sel& s) { return common::make_smart_iterator(s.begin(), s.end()); }, py::keep_alive<0, 1>())
      .def("__or__", [](Sel& lhs, Sel& rhs) { return (lhs | rhs).smart(); })
      .def("__and__", [](Sel& lhs, Sel& rhs) { return (lhs & rhs).smart(); })
      .def("__sub__", [](Sel& lhs, Sel& rhs) { return (lhs - rhs).smart(); });
}