#include "../FCRA.h"

#include "xmol/pdb/PdbRecord.h"
#include "xmol/pdb/PdbWriter.h"
#include "xmol/polymer/Atom.h"
#include "xmol/polymer/exceptions.h"

#include "xmol/geometry/Transformation3d.h"
#include "xmol/geometry/alignment.h"

#include "xmol/utils/string.h"

#include "pybind11/functional.h"
#include "pybind11/iostream.h"
#include "pybind11/operators.h"
#include "pybind11/numpy.h"
#include "pybind11/eigen.h"
#include "pybind11/stl.h"

#include "../../geometry/init.h"

#include <fstream>

void pyxmolpp::polymer::init_AtomSelection(detail::FWD& fwd, py::module& polymer) {
  using namespace xmol::polymer;

  (*fwd.pyAtomSelectionRange)
  .def("__next__",
                           [](xmol::selection::SelectionRange<Atom>& rng) -> AtomRef {
                             if (rng != rng) {
                               Atom& a = *rng;
                               ++rng;
                               return AtomRef(a);
                             }
                             throw py::stop_iteration();
                           },
                           AtomRefPolicy, py::keep_alive<0, 1>());

  (*fwd.pyAtomSelection)
      .def(
          py::init([](py::iterable atom_list){
            std::vector<Atom*> ats;
            auto iter = py::iter(atom_list);
            while (iter != py::iterator::sentinel())
            {
              AtomRef atom_ref = py::cast<AtomRef>(*iter);
              ats.push_back(std::addressof(static_cast<Atom&>(atom_ref)));
              ++iter;
            }
            return AtomSelection(ats.begin(),ats.end());
          }),
          py::arg("atom_list"),
          "Construct AtomSelection from iterable with Atom elements")

      .def(
          "filter", // python predicates may alter atom, no way to prevent it
          [](AtomSelection& sel, std::function<bool(AtomRef)>& predicate) {
            return sel.filter([&](const Atom& a) { return predicate(AtomRef(const_cast<Atom&>(a))); });
          },
          py::arg("predicate"),
          "Keeps in selection only elements that match predicate")

      .def(
          "index", // python predicates may alter atom, no way to prevent it
          [](AtomSelection& sel, std::function<bool(AtomRef)>& predicate) {
            auto result = new std::vector<int>(sel.index([&](const Atom& a) { return predicate(AtomRef(const_cast<Atom&>(a))); }));
            auto cap = py::capsule(result, [](void *v) { delete reinterpret_cast<std::vector<int>*>(v); });
            return py::array(result->size(), result->data(), cap);
          },
          py::arg("predicate"),
          "Return indices for which predicate is true")

      .def(
          "for_each",
          [](AtomSelection& sel, std::function<void(AtomRef)>& func) {
            return sel.for_each([&](Atom& a) { func(AtomRef(a)); });
          },
          py::arg("transformation"),
          "Applies (mutating) transformation to atoms")

      .def(
          "geom_center",
          [](AtomSelection& sel) {
            return xmol::geometry::calc_geom_center(sel.toCoords());
          },
          "Returns selection geometric center")

      .def(
          "mass_center",
          [](AtomSelection& sel, std::vector<double> mass) {
            return xmol::geometry::calc_mass_center(sel.toCoords(), mass);
          },
          py::arg("mass"),
          "Returns selection center of mass")

      .def(
          "inertia_tensor",
          [](AtomSelection& sel, std::vector<double> mass) {
            return xmol::geometry::calc_inertia_tensor(sel.toCoords(), mass);
          },
          py::arg("mass"),
          "Returns selection inertia tensor")

      .def(
          "geom_inertia_tensor",
          [](AtomSelection& sel) {
            return xmol::geometry::calc_inertia_tensor(sel.toCoords());
          },
          "Returns inertia tensor calculated with equal mass assumption")

      .def(
          "rmsd",
          [](AtomSelection& sel, AtomSelection& ref) {
            return xmol::geometry::calc_rmsd(sel.toCoords(), ref.toCoords());
          },
          py::arg("ref"),
          "Returns rmsd between two selections")

      .def(
          "rmsd",
          [](AtomSelection& sel, std::vector<XYZ> ref) {
            return xmol::geometry::calc_rmsd(sel.toCoords(), ref);
          },
          py::arg("ref"),
          "Returns rmsd between selection and reference coordinates")

      .def(
          "alignment_to",
          [](AtomSelection& sel, std::vector<XYZ> ref) {
            return xmol::geometry::calc_alignment(ref, sel.toCoords());
          },
          py::arg("ref"),
          "Equivalent to :code:`calc_alignment_to(ref, self.toCoords())`")
      .def("to_pdb",
           [](AtomSelection& sel, std::string& path, xmol::pdb::basic_PdbRecords& db) {
             std::ofstream out(path);
             if (out.fail()) {
               throw IOError("Can't open file `" + path + "` for writing");
             }
             xmol::pdb::PdbWriter writer(out);
             writer.write(sel, db);
           },
           py::arg("path_or_buf"),
           py::arg_v("db",std::ref(xmol::pdb::StandardPdbRecords::instance()),"pyxmolpp2.pdb.StandardPdbRecords.instance()"),
           "Writes frame to file using `db` as non-standard PDB records"
      )
      .def("to_pdb",
           [](AtomSelection& sel, py::object fileHandle, xmol::pdb::basic_PdbRecords& db) {

             if (!(py::hasattr(fileHandle,"write") &&
                 py::hasattr(fileHandle,"flush") )){
               throw py::type_error("AtomSelection.to_pdb(file): incompatible function argument:  `file` must be a file-like object, but `"
                                        +(std::string)(py::repr(fileHandle))+"` provided"
               );
             }
             py::detail::pythonbuf buf(fileHandle);
             std::ostream stream(&buf);
             xmol::pdb::PdbWriter writer(stream);
             writer.write(sel, db);
           },
           py::arg("path_or_buf"),
           py::arg_v("db",std::ref(xmol::pdb::StandardPdbRecords::instance()),"pyxmolpp2.pdb.StandardPdbRecords.instance()"),
           "Writes selection to `file` using `db` as non-standard PDB records"
      )

      .def(
          "alignment_to",
          [](AtomSelection& sel, AtomSelection& ref) {
            return xmol::geometry::calc_alignment(ref.toCoords(), sel.toCoords());
          },
          py::arg("ref"),
          "Equivalent to :code:`calc_alignment_to(ref.toCoords(), self.toCoords())`")

      .def(
          "align_to",
          [](AtomSelection& sel, std::vector<XYZ> ref) -> AtomSelection& {
            auto al = xmol::geometry::calc_alignment(ref, sel.toCoords());
            sel.for_each([&](Atom& a) { a.set_r(al.transform(a.r())); });
            return sel;
          },
          py::arg("ref"),
          py::return_value_policy::reference,
          "Equivalent to :code:`calc_alignment(ref, self.toCoords())`")

      .def(
          "align_to",
          [](AtomSelection& sel, AtomSelection& ref) -> AtomSelection& {
            auto al = xmol::geometry::calc_alignment(ref.toCoords(), sel.toCoords());
            sel.for_each([&](Atom& a) { a.set_r(al.transform(a.r())); });
            return sel;
          },
          py::arg("ref"),
          py::return_value_policy::reference,
          "Equivalent to :code:`calc_alignment(ref.toCoords(), self.toCoords())`")

      .def(
          "transform",
          [](AtomSelection& sel, xmol::geometry::Transformation3d& transformation3d) {
            return sel.for_each([&](Atom& a) { a.set_r(transformation3d.transform(a.r())); });
          },
          py::arg("transformation"),
          "Applies transformation to atom coordinates")

      .def("transform",
          [](AtomSelection& sel, xmol::geometry::Rotation3d& rotation3d) {
            return sel.for_each([&](Atom& a) { a.set_r(rotation3d.transform(a.r())); });
          },
          py::arg("transformation"),
          "Applies transformation to atom coordinates")

      .def("transform",
          [](AtomSelection& sel, xmol::geometry::UniformScale3d& uniformScale3d) {
            return sel.for_each([&](Atom& a) { a.set_r(uniformScale3d.transform(a.r())); });
          },
          py::arg("transformation"),
          "Applies transformation to atom coordinates")

      .def(
          "transform",
          [](AtomSelection& sel, xmol::geometry::Translation3d& translation3d) {
            return sel.for_each([&](Atom& a) { a.set_r(translation3d.transform(a.r())); });
          },
          py::arg("transformation"),
          "Applies transformation to atom coordinates")

      .def(
          "__len__",
          [](AtomSelection& asel) { return asel.size(); },
          "Returns number of elements")

      .def_property_readonly(
          "size",
          [](AtomSelection& asel) { return asel.size(); },
          "Returns number of elements")

      .def_property_readonly(
          "toCoords",
          [](AtomSelection& aSel) { return aSel.toCoords(); },
          "Returns copy of atom coordinates")

      .def_property_readonly(
          "asChains",
          [](AtomSelection& aSel) { return aSel.asChains(); },
          "Returns selection of parent chains")

      .def_property_readonly(
          "asResidues",
          [](AtomSelection& aSel) { return aSel.asResidues(); },
          "Returns selection of parent residues")

      .def(
          "__iter__",
          [](AtomSelection& sel) { return sel.begin(); },
          py::keep_alive<0, 1>())

      .def(
          "__getitem__",
          [](AtomSelection& sel, py::buffer& b) {
            py::buffer_info info = b.request();

            /* Some sanity checks ... */
            if (info.ndim != 1) {
              throw std::runtime_error("AtomSelection::__getitem__(indices): index array dimension != 1");
            }

            if (info.format == "i"){

              auto begin = static_cast<int* >(info.ptr);
              auto end = static_cast<int* >(info.ptr)+info.size;

              return sel.at_index(begin, end);
            }
            else if (info.format == "l"){

              auto begin = static_cast<long* >(info.ptr);
              auto end = static_cast<long* >(info.ptr)+info.size;

              return sel.at_index(begin, end);
            }
            else if (info.format == "L"){

              auto begin = static_cast<long long* >(info.ptr);
              auto end = static_cast<long long* >(info.ptr)+info.size;

              return sel.at_index(begin, end);
            }
            else if (info.format == py::format_descriptor<bool>::format()){
              if (info.size!=sel.size()){
                throw std::runtime_error("AtomSelection::__getitem__(indices): bool array size doesn't match selection size");
              }
              auto begin = static_cast<bool* >(info.ptr);
              auto end = static_cast<bool* >(info.ptr)+info.size;

              assert(std::distance(begin,end) == info.size);

              return sel.filter(begin, end);
            }else{
              throw std::runtime_error("Incompatible format: expected np.array[bool] or np.array[int], found buffer["+info.format+"]");
            }

          },
          py::arg("indices"),
          "Indicator array overload")

      .def(
          "__getitem__",
          [](AtomSelection& sel, int index) -> AtomRef { return AtomRef(sel[index]); }, AtomRefPolicy,
          py::arg("n"),
          "Access n'th element")

      .def(
          "__getitem__",
           [](AtomSelection& sel, py::slice slice) {
             ssize_t start, stop, step, slicelength;
             if (!slice.compute(sel.size(), reinterpret_cast<size_t*>(&start), reinterpret_cast<size_t*>(&stop),
                                reinterpret_cast<size_t*>(&step), reinterpret_cast<size_t*>(&slicelength))) {
               throw py::error_already_set();
             }
             return sel.slice(start, stop, step, xmol::selection::SlicingScheme::USE_INDICES_AS_IS);
           },
           py::arg("slice"),
           "Slice overload")

      .def(
          "__repr__",
          [](AtomSelection& selection) {
            return "<pyxmolpp2.polymer.AtomSelection size="
            + std::to_string(selection.size())
            + " at 0x"
            + xmol::utils::string::int2hex((uint64_t)(std::addressof(selection)))
            + ">";
          })

      .def(py::self *= py::self)
      .def(py::self += py::self)
      .def(py::self -= py::self)
      .def(py::self != py::self)
      .def(py::self == py::self)
      .def(py::self * py::self)
      .def(py::self + py::self)
      .def(py::self - py::self);

}
