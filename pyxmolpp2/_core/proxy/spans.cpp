#include "spans.h"
#include "iterator-helpers.h"
#include "repr-helpers.h"
#include "to_pdb_shortcuts.h"
#include "xmol/Frame.h"
#include "xmol/geom/affine/Transformation3d.h"
#include "xmol/proxy/smart/references.h"
#include "xmol/proxy/smart/selections.h"
#include "xmol/proxy/smart/spans.h"
#include "xmol/proxy/spans-impl.h"

#include <pybind11/eigen.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/pybind11.h>

#include <variant>

namespace py = pybind11;
using namespace xmol;
using namespace xmol::proxy;
using namespace xmol::geom::affine;
using namespace xmol::proxy::smart;

void pyxmolpp::v1::populate(pybind11::class_<xmol::proxy::smart::CoordSmartSpan>& pyCoordSpan) {
  using Sel = CoordSmartSelection;
  using Span = CoordSmartSpan;
  pyCoordSpan.def(py::init<Span>())
      .def_property_readonly("size", &Span::size)
      .def_property_readonly("empty", &Span::empty)
      .def_property_readonly("__frame", &Span::frame, py::return_value_policy::reference)
      .def("filter", [](Span& span, const std::function<bool(const XYZ&)>& f) { return span.filter(f).smart(); })
      .def_property_readonly("index", &Span::index)
      .def("__len__", &Span::size)
      //      .def("__contains__", &Span::contains)
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
           [](Span& span, py::slice& slice) -> std::variant<CoordSmartSpan, CoordSmartSelection> {
             ssize_t start, stop, step, slicelength;
             if (!slice.compute(span.size(), &start, &stop, &step, &slicelength)) {
               throw py::error_already_set();
             }
             if (step < 0) {
               throw py::type_error("Negative strides are not (yet) supported");
             }
             assert(start >= 0);
             assert(stop >= 0);
             if (step == 1) {
               return span.slice(start, stop).smart();
             }
             return span.slice(start, stop, step).smart();
           })
      .def(
          "__iter__", [](Span& s) { return common::make_coord_value_iterator(s.begin(), s.end()); },
          py::keep_alive<0, 1>())
      .def_property_readonly("values",
                             [](py::object self) {
                               py::object owner = self.attr("__frame");
                               Span& span = self.cast<Span&>();
                               auto eigen_map = span._eigen();
                               size_t shape[] = {(size_t)eigen_map.rows(), (size_t)eigen_map.cols()};
                               size_t strides[] = {(size_t)eigen_map.rowStride() * sizeof(double),
                                                   (size_t)eigen_map.colStride() * sizeof(double)};
                               auto array = py::array(shape, strides, eigen_map.data(), owner);
                               return array;
                             })
      .def("alignment_to", [](Span& span, Sel& other) { return span.alignment_to(other); })
      .def("alignment_to", [](Span& span, Span& other) { return span.alignment_to(other); })
      .def("align_to", [](Span& span, Sel& other) { span.apply(span.alignment_to(other)); })
      .def("align_to", [](Span& span, Span& other) { span.apply(span.alignment_to(other)); })
      .def("rmsd", [](Span& span, Sel& other) { return span.rmsd(other); })
      .def("rmsd", [](Span& span, Span& other) { return span.rmsd(other); })
      .def("apply", [](Span& sel, Transformation3d& other) { return sel.apply(other); })
      .def("apply", [](Span& sel, UniformScale3d& other) { return sel.apply(other); })
      .def("apply", [](Span& sel, Rotation3d& other) { return sel.apply(other); })
      .def("apply", [](Span& sel, Translation3d& other) { return sel.apply(other); })
      .def("mean", [](Span& sel) { return sel.mean(); })
      .def("inertia_tensor", &Span::inertia_tensor)
      .def("__str__", [](Span& self) { return "CoordsSpan<size=" + std::to_string(self.size()) + ">"; });
}
void pyxmolpp::v1::populate(pybind11::class_<xmol::proxy::smart::AtomSmartSpan>& pyAtomSpan) {
  using Span = AtomSmartSpan;
  pyAtomSpan.def(py::init<Span>())
      .def_property_readonly("size", &Span::size)
      .def_property_readonly("empty", &Span::empty)
      .def("filter",
           [](Span& span, const std::function<bool(const AtomSmartRef&)>& f) { return span.filter(f).smart(); })
      .def_property_readonly("coords", [](Span& span) { return span.coords().smart(); })
      .def_property_readonly("residues", [](Span& span) { return span.residues().smart(); })
      .def_property_readonly("molecules", [](Span& span) { return span.molecules().smart(); })
      .def_property_readonly("index", &Span::index)
      .def("guess_mass", &Span::guess_mass)
      .def(
          "alignment_to",
          [](Span& span, AtomSmartSelection& rhs, bool weighted) { return span.alignment_to(rhs, weighted); },
          py::arg("other"), py::kw_only{}, py::arg("weighted") = false)
      .def(
          "alignment_to",
          [](Span& span, AtomSmartSpan& rhs, bool weighted) {
            AtomSpan rhs_span(rhs);
            return span.alignment_to(rhs_span, weighted);
          },
          py::arg("other"), py::kw_only{}, py::arg("weighted") = false)
      .def(
          "align_to",
          [](Span& span, AtomSmartSelection& rhs, bool weighted) {
            span.coords().apply(span.alignment_to(rhs, weighted));
          },
          py::arg("other"), py::kw_only{}, py::arg("weighted") = false)
      .def(
          "align_to",
          [](Span& span, AtomSmartSpan& rhs, bool weighted) {
            AtomSpan rhs_span(rhs);
            span.coords().apply(span.alignment_to(rhs_span, weighted));
          },
          py::arg("other"), py::kw_only{}, py::arg("weighted") = false)
      .def(
          "rmsd",
          [](Span& span, AtomSmartSelection& rhs, bool weighted) { return span.rmsd(rhs, weighted); },
          py::arg("other"), py::kw_only{}, py::arg("weighted") = false)
      .def(
          "rmsd",
          [](Span& span, AtomSmartSpan& rhs, bool weighted) {
            AtomSpan rhs_span(rhs);
            return span.rmsd(rhs_span, weighted);
          },
          py::arg("other"), py::kw_only{}, py::arg("weighted") = false)
      .def("mean", &Span::mean, py::arg("weighted")=false, "Mean coordinates")
      .def("inertia_tensor", &Span::inertia_tensor, "Inertia tensor")
      .def("to_pdb", to_pdb_file<Span>, py::arg("path_or_buf"), "Write atoms as `.pdb` file")
      .def("to_pdb", to_pdb_stream<Span>, py::arg("path_or_buf"), "Write atoms in PDB format")
      .def("__len__", &Span::size)
      .def("__contains__", [](Span& span, AtomSmartRef& ref) { return span.contains(ref); })
      .def("__getitem__",
           [](Span& span, ssize_t i) {
             if (i < 0) {
               i += span.size();
             }
             if (i < 0 || i >= span.size()) {
               throw py::index_error();
             }
             return span[i].smart();
           })
      .def("__getitem__",
           [](Span& span, py::slice& slice) -> std::variant<AtomSmartSpan, AtomSmartSelection> {
             ssize_t start, stop, step, slicelength;
             if (!slice.compute(span.size(), &start, &stop, &step, &slicelength)) {
               throw py::error_already_set();
             }
             if (step < 0) {
               throw py::type_error("Negative strides are not (yet) supported");
             }
             assert(start >= 0);
             assert(stop >= 0);
             if (step == 1) {
               return span.slice(start, stop).smart();
             }
             return span.slice(start, stop, step).smart();
           })
      .def(
          "__iter__", [](Span& s) { return common::make_smart_iterator(s.begin(), s.end()); }, py::keep_alive<0, 1>())
      .def("__or__", [](Span& lhs, Span& rhs) { return (lhs | rhs).smart(); })
      .def("__and__", [](Span& lhs, Span& rhs) { return (lhs & rhs).smart(); })
      .def("__sub__", [](Span& lhs, Span& rhs) { return (lhs - rhs).smart(); })
      .def("__str__", [](Span& self) {
        return "AtomSpan<size=" + std::to_string(self.size()) + ", atoms=[" + to_string_3_elements(self) + "]>";
      });
}
void pyxmolpp::v1::populate(pybind11::class_<xmol::proxy::smart::ResidueSmartSpan>& pyResidueSpan) {
  using Span = ResidueSmartSpan;
  pyResidueSpan.def(py::init<Span>())
      .def_property_readonly("size", &Span::size)
      .def_property_readonly("empty", &Span::empty)
      .def("filter",
           [](Span& span, const std::function<bool(const ResidueSmartRef&)>& f) { return span.filter(f).smart(); })
      .def_property_readonly("coords", [](Span& span) { return span.coords().smart(); })
      .def_property_readonly("atoms", [](Span& span) { return span.atoms().smart(); })
      .def_property_readonly("molecules", [](Span& span) { return span.molecules().smart(); })
      .def_property_readonly("index", &Span::index)
      .def("to_pdb", to_pdb_file<Span>, py::arg("path_or_buf"))
      .def("to_pdb", to_pdb_stream<Span>, py::arg("path_or_buf"))
      .def("__len__", &Span::size)
      .def("__contains__", [](Span& span, ResidueSmartRef& ref) { return span.contains(ref); })
      .def("__getitem__",
           [](Span& span, ssize_t i) {
             if (i < 0) {
               i += span.size();
             }
             if (i < 0 || i >= span.size()) {
               throw py::index_error();
             }
             return span[i].smart();
           })
      .def("__getitem__",
           [](Span& span, py::slice& slice) -> std::variant<ResidueSmartSpan, ResidueSmartSelection> {
             ssize_t start, stop, step, slicelength;
             if (!slice.compute(span.size(), &start, &stop, &step, &slicelength)) {
               throw py::error_already_set();
             }
             if (step < 0) {
               throw py::type_error("Negative strides are not (yet) supported");
             }
             assert(start >= 0);
             assert(stop >= 0);
             if (step == 1) {
               return span.slice(start, stop).smart();
             }
             return span.slice(start, stop, step).smart();
           })
      .def(
          "__iter__", [](Span& s) { return common::make_smart_iterator(s.begin(), s.end()); }, py::keep_alive<0, 1>())
      .def("__or__", [](Span& lhs, Span& rhs) { return (lhs | rhs).smart(); })
      .def("__and__", [](Span& lhs, Span& rhs) { return (lhs & rhs).smart(); })
      .def("__sub__", [](Span& lhs, Span& rhs) { return (lhs - rhs).smart(); })
      .def("__str__", [](Span& self) {
        return "ResidueSpan<size=" + std::to_string(self.size()) + ", residues=[" + to_string_3_elements(self) + "]>";
      });
}
void pyxmolpp::v1::populate(pybind11::class_<xmol::proxy::smart::MoleculeSmartSpan>& pyMoleculeSpan) {
  using Span = MoleculeSmartSpan;
  pyMoleculeSpan.def(py::init<Span>())
      .def_property_readonly("size", &Span::size)
      .def_property_readonly("empty", &Span::empty)
      .def("filter",
           [](Span& span, const std::function<bool(const MoleculeSmartRef&)>& f) { return span.filter(f).smart(); })
      .def_property_readonly("coords", [](Span& span) { return span.coords().smart(); })
      .def_property_readonly("atoms", [](Span& span) { return span.atoms().smart(); })
      .def_property_readonly("residues", [](Span& span) { return span.residues().smart(); })
      .def_property_readonly("index", &Span::index)
      .def("to_pdb", to_pdb_file<Span>, py::arg("path_or_buf"))
      .def("to_pdb", to_pdb_stream<Span>, py::arg("path_or_buf"))
      .def("__len__", &Span::size)
      .def("__contains__", [](Span& span, MoleculeSmartRef& ref) { return span.contains(ref); })
      .def("__getitem__",
           [](Span& span, ssize_t i) {
             if (i < 0) {
               i += span.size();
             }
             if (i < 0 || i >= span.size()) {
               throw py::index_error();
             }
             return span[i].smart();
           })
      .def("__getitem__",
           [](Span& span, py::slice& slice) -> std::variant<MoleculeSmartSpan, MoleculeSmartSelection> {
             ssize_t start, stop, step, slicelength;
             if (!slice.compute(span.size(), &start, &stop, &step, &slicelength)) {
               throw py::error_already_set();
             }
             if (step < 0) {
               throw py::type_error("Negative strides are not (yet) supported");
             }
             assert(start >= 0);
             assert(stop >= 0);
             if (step == 1) {
               return span.slice(start, stop).smart();
             }
             return span.slice(start, stop, step).smart();
           })
      .def(
          "__iter__", [](Span& s) { return common::make_smart_iterator(s.begin(), s.end()); }, py::keep_alive<0, 1>())
      .def("__or__", [](Span& lhs, Span& rhs) { return (lhs | rhs).smart(); })
      .def("__and__", [](Span& lhs, Span& rhs) { return (lhs & rhs).smart(); })
      .def("__sub__", [](Span& lhs, Span& rhs) { return (lhs - rhs).smart(); })
      .def("__str__", [](Span& self) {
        return "MoleculeSpan<size=" + std::to_string(self.size()) + ", molecules=[" + to_string_3_elements(self) + "]>";
      });;
}