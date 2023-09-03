#include "selections.h"
#include "iterator-helpers.h"
#include "repr-helpers.h"
#include "to_pdb_shortcuts.h"
#include "xmol/geom/affine/Transformation3d.h"
#include "xmol/proxy/smart/references.h"
#include "xmol/proxy/smart/selections.h"
#include "xmol/proxy/smart/spans.h"

#include <pybind11/eigen.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace xmol;
using namespace xmol::proxy;
using namespace xmol::proxy::smart;
using namespace xmol::geom::affine;

void pyxmolpp::v1::populate(pybind11::class_<xmol::proxy::smart::CoordSmartSelection>& pyCoordSelection) {
  using Sel = CoordSmartSelection;
  using Span = CoordSmartSpan;
  pyCoordSelection.def(py::init<Sel>())
      .def_property_readonly("size", &Sel::size)
      .def_property_readonly("empty", &Sel::empty)
      .def("filter", [](Sel& sel, const std::function<bool(const XYZ&)>& f) { return sel.filter(f).smart(); })
      .def_property_readonly("index", &Sel::index)
      .def("__len__", &Sel::size)
      //      .def("__contains__", &Sel::contains)
      .def("__getitem__",
           [](Span& span, ssize_t i) {
             if (i < 0) {
               i += span.size();
             }
             if (i < 0 || i >= span.size()) {
               throw py::index_error();
             }
             return XYZ(span[i]);
           })
      .def("__getitem__",
           [](Sel& sel, py::slice& slice) {
             ssize_t start, stop, step, slicelength;
             if (!slice.compute(sel.size(), &start, &stop, &step, &slicelength)) {
               throw py::error_already_set();
             }
             if (step < 0) {
               throw py::type_error("Negative strides are not (yet) supported");
             }
             assert(start >= 0);
             assert(stop >= 0);
             return sel.slice(start, stop, step).smart();
           })
      .def(
          "__iter__", [](Sel& s) { return common::make_coord_value_iterator(s.begin(), s.end()); },
          py::keep_alive<0, 1>())
      .def_property("values", py::overload_cast<>(&Sel::_eigen),
                    py::overload_cast<const CoordEigenMatrix&>(&Sel::_eigen))
      .def("alignment_to", [](Sel& sel, Sel& other) { return sel.alignment_to(other); })
      .def("alignment_to", [](Sel& sel, Span& other) { return sel.alignment_to(other); })
      .def("align_to", [](Sel& sel, Span& other) { return sel.apply(sel.alignment_to(other)); })
      .def("align_to", [](Sel& sel, Sel& other) { return sel.apply(sel.alignment_to(other)); })
      .def("rmsd", [](Sel& sel, Sel& other) { return sel.rmsd(other); })
      .def("rmsd", [](Sel& sel, Span& other) { return sel.rmsd(other); })
      .def("apply", [](Sel& sel, Transformation3d& other) { return sel.apply(other); })
      .def("apply", [](Sel& sel, UniformScale3d& other) { return sel.apply(other); })
      .def("apply", [](Sel& sel, Rotation3d& other) { return sel.apply(other); })
      .def("apply", [](Sel& sel, Translation3d& other) { return sel.apply(other); })
      .def("mean", [](Sel& sel) { return sel.mean(); })
      .def("inertia_tensor", &Sel::inertia_tensor)
      .def("__str__", [](Sel& self) { return "CoordSelection<size=" + std::to_string(self.size()) + ">"; });
  ;
}
void pyxmolpp::v1::populate(pybind11::class_<xmol::proxy::smart::AtomSmartSelection>& pyAtomSelection) {
  using Sel = AtomSmartSelection;
  pyAtomSelection.def(py::init<Sel>())
      .def(py::init([](py::iterable& it) {
        std::vector<AtomRef> refs;
        for (auto&& el : it) {
          refs.emplace_back(el.cast<AtomSmartRef&>());
        }
        return AtomSelection(std::move(refs)).smart();
      }))
      .def_property_readonly("size", &Sel::size)
      .def_property_readonly("empty", &Sel::empty)
      .def_property_readonly("coords", [](Sel& sel) { return sel.coords().smart(); })
      .def_property_readonly("residues", [](Sel& sel) { return sel.residues().smart(); })
      .def_property_readonly("molecules", [](Sel& sel) { return sel.molecules().smart(); })
      .def("filter", [](Sel& sel, const std::function<bool(const AtomSmartRef&)>& f) { return sel.filter(f).smart(); })
      .def_property_readonly("index", &Sel::index)
      .def("guess_mass", &Sel::guess_mass)
      .def(
          "alignment_to",
          [](Sel& span, AtomSmartSelection& rhs, bool weighted) { return span.alignment_to(rhs, weighted); },
          py::arg("other"), py::kw_only{}, py::arg("weighted") = false)
      .def(
          "alignment_to",
          [](Sel& span, AtomSmartSpan& rhs, bool weighted) {
            AtomSpan rhs_span(rhs);
            return span.alignment_to(rhs_span, weighted);
          },
          py::arg("other"), py::kw_only{}, py::arg("weighted") = false)
      .def(
          "align_to",
          [](Sel& span, AtomSmartSelection& rhs, bool weighted) {
            span.coords().apply(span.alignment_to(rhs, weighted));
          },
          py::arg("other"), py::kw_only{}, py::arg("weighted") = false)
      .def(
          "align_to",
          [](Sel& span, AtomSmartSpan& rhs, bool weighted) {
            AtomSpan rhs_span(rhs);
            span.coords().apply(span.alignment_to(rhs_span, weighted));
          },
          py::arg("other"), py::kw_only{}, py::arg("weighted") = false)
      .def(
          "rmsd",
          [](Sel& span, AtomSmartSelection& rhs, bool weighted) { return span.rmsd(rhs, weighted); },
          py::arg("other"), py::kw_only{}, py::arg("weighted") = false)
      .def(
          "rmsd",
          [](Sel& span, AtomSmartSpan& rhs, bool weighted) {
            AtomSpan rhs_span(rhs);
            return span.rmsd(rhs_span, weighted);
          },
          py::arg("other"), py::kw_only{}, py::arg("weighted") = false)
      .def("mean", &Sel::mean, py::arg("weighted")=false, "Mean coordinates")
      .def("inertia_tensor", &Sel::inertia_tensor)
      .def("to_pdb", to_pdb_file<Sel>, py::arg("path_or_buf"))
      .def("to_pdb", to_pdb_stream<Sel>, py::arg("path_or_buf"))
      .def("__len__", &Sel::size)
      .def("__contains__", [](Sel& sel, AtomSmartRef& ref) { return sel.contains(ref); })
      .def("__getitem__",
           [](Sel& span, ssize_t i) {
             if (i < 0) {
               i += span.size();
             }
             if (i < 0 || i >= span.size()) {
               throw py::index_error();
             }
             return span[i].smart();
           })
      .def("__getitem__",
           [](Sel& sel, py::slice& slice) {
             ssize_t start, stop, step, slicelength;
             if (!slice.compute(sel.size(), &start, &stop, &step, &slicelength)) {
               throw py::error_already_set();
             }
             if (step < 0) {
               throw py::type_error("Negative strides are not (yet) supported");
             }
             assert(start >= 0);
             assert(stop >= 0);
             return sel.slice(start, stop, step).smart();
           })
      .def(
          "__iter__", [](Sel& s) { return common::make_smart_iterator(s.begin(), s.end()); }, py::keep_alive<0, 1>())
      .def("__or__", [](Sel& lhs, Sel& rhs) { return (lhs | rhs).smart(); })
      .def("__and__", [](Sel& lhs, Sel& rhs) { return (lhs & rhs).smart(); })
      .def("__sub__", [](Sel& lhs, Sel& rhs) { return (lhs - rhs).smart(); })
      .def("__str__", [](Sel& self) {
        return "AtomSelection<size=" + std::to_string(self.size()) + ", atoms=[" + to_string_3_elements(self) + "]>";
      });
}
void pyxmolpp::v1::populate(pybind11::class_<xmol::proxy::smart::ResidueSmartSelection>& pyResidueSelection) {
  using Sel = ResidueSmartSelection;
  pyResidueSelection.def(py::init<Sel>())
      .def(py::init([](py::iterable& it) {
        std::vector<ResidueRef> refs;
        for (auto&& el : it) {
          refs.emplace_back(el.cast<ResidueSmartRef&>());
        }
        return ResidueSelection(std::move(refs)).smart();
      }))
      .def_property_readonly("size", &Sel::size)
      .def_property_readonly("empty", &Sel::empty)
      .def("filter",
           [](Sel& sel, const std::function<bool(const ResidueSmartRef&)>& f) { return sel.filter(f).smart(); })
      .def_property_readonly("coords", [](Sel& sel) { return sel.coords().smart(); })
      .def_property_readonly("atoms", [](Sel& sel) { return sel.atoms().smart(); })
      .def_property_readonly("molecules", [](Sel& sel) { return sel.molecules().smart(); })
      .def_property_readonly("index", &Sel::index)
      .def("to_pdb", to_pdb_file<Sel>, py::arg("path_or_buf"))
      .def("to_pdb", to_pdb_stream<Sel>, py::arg("path_or_buf"))
      .def("__len__", &Sel::size)
      .def("__contains__", [](Sel& sel, ResidueSmartRef& ref) { return sel.contains(ref); })
      .def("__getitem__",
           [](Sel& span, ssize_t i) {
             if (i < 0) {
               i += span.size();
             }
             if (i < 0 || i >= span.size()) {
               throw py::index_error();
             }
             return span[i].smart();
           })
      .def("__getitem__",
           [](Sel& sel, py::slice& slice) {
             ssize_t start, stop, step, slicelength;
             if (!slice.compute(sel.size(), &start, &stop, &step, &slicelength)) {
               throw py::error_already_set();
             }
             if (step < 0) {
               throw py::type_error("Negative strides are not (yet) supported");
             }
             assert(start >= 0);
             assert(stop >= 0);
             return sel.slice(start, stop, step).smart();
           })
      .def(
          "__iter__", [](Sel& s) { return common::make_smart_iterator(s.begin(), s.end()); }, py::keep_alive<0, 1>())
      .def("__or__", [](Sel& lhs, Sel& rhs) { return (lhs | rhs).smart(); })
      .def("__and__", [](Sel& lhs, Sel& rhs) { return (lhs & rhs).smart(); })
      .def("__sub__", [](Sel& lhs, Sel& rhs) { return (lhs - rhs).smart(); })
      .def("__str__", [](Sel& self) {
        return "ResidueSelection<size=" + std::to_string(self.size()) + ", residues=[" + to_string_3_elements(self) +
               "]>";
      });
}
void pyxmolpp::v1::populate(pybind11::class_<xmol::proxy::smart::MoleculeSmartSelection>& pyMoleculeSelection) {
  using Sel = MoleculeSmartSelection;
  pyMoleculeSelection.def(py::init<Sel>())
      .def(py::init([](py::iterable& it) {
        std::vector<MoleculeRef> refs;
        for (auto&& el : it) {
          refs.emplace_back(el.cast<MoleculeSmartRef&>());
        }
        return MoleculeSelection(std::move(refs)).smart();
      }))
      .def_property_readonly("size", &Sel::size)
      .def_property_readonly("empty", &Sel::empty)
      .def("filter",
           [](Sel& sel, const std::function<bool(const MoleculeSmartRef&)>& f) { return sel.filter(f).smart(); })
      .def_property_readonly("coords", [](Sel& sel) { return sel.coords().smart(); })
      .def_property_readonly("atoms", [](Sel& sel) { return sel.atoms().smart(); })
      .def_property_readonly("residues", [](Sel& sel) { return sel.residues().smart(); })
      .def_property_readonly("index", &Sel::index)
      .def("to_pdb", to_pdb_file<Sel>, py::arg("path_or_buf"))
      .def("to_pdb", to_pdb_stream<Sel>, py::arg("path_or_buf"))
      .def("__len__", &Sel::size)
      .def("__contains__", [](Sel& sel, MoleculeSmartRef& ref) { return sel.contains(ref); })
      .def("__getitem__",
           [](Sel& span, ssize_t i) {
             if (i < 0) {
               i += span.size();
             }
             if (i < 0 || i >= span.size()) {
               throw py::index_error();
             }
             return span[i].smart();
           })
      .def("__getitem__",
           [](Sel& sel, py::slice& slice) {
             ssize_t start, stop, step, slicelength;
             if (!slice.compute(sel.size(), &start, &stop, &step, &slicelength)) {
               throw py::error_already_set();
             }
             if (step < 0) {
               throw py::type_error("Negative strides are not (yet) supported");
             }
             assert(start >= 0);
             assert(stop >= 0);
             return sel.slice(start, stop, step).smart();
           })
      .def(
          "__iter__", [](Sel& s) { return common::make_smart_iterator(s.begin(), s.end()); }, py::keep_alive<0, 1>())
      .def("__or__", [](Sel& lhs, Sel& rhs) { return (lhs | rhs).smart(); })
      .def("__and__", [](Sel& lhs, Sel& rhs) { return (lhs & rhs).smart(); })
      .def("__sub__", [](Sel& lhs, Sel& rhs) { return (lhs - rhs).smart(); })
      .def("__str__", [](Sel& self) {
        return "MoleculeSelection<size=" + std::to_string(self.size()) + ", molecules=[" + to_string_3_elements(self) + "]>";
      });
  ;
}