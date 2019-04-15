#include "../FCRA.h"

#include "xmol/pdb/PdbRecord.h"
#include "xmol/pdb/PdbWriter.h"
#include "xmol/polymer/Atom.h"
#include "xmol/polymer/exceptions.h"

#include "xmol/utils/string.h"

#include "pybind11/functional.h"
#include "pybind11/iostream.h"
#include "pybind11/numpy.h"
#include "pybind11/operators.h"

#include <fstream>

void pyxmolpp::polymer::init_ChainSelection(detail::FWD& fwd, py::module& polymer) {

  using namespace xmol::polymer;

  (*fwd.pyChainSelectionRange)
  .def("__next__",
                            [](xmol::selection::SelectionRange<Chain>& rng) -> ChainRef {
                              if (rng != rng) {
                                Chain& a = *rng;
                                ++rng;
                                return ChainRef(a);
                              }
                              throw py::stop_iteration();
                            },
                            ChainRefPolicy, py::keep_alive<0, 1>());

  (*fwd.pyChainSelection)
      .def(py::init([](py::iterable chain_list){
             std::vector<Chain*> elements;
             auto iter = py::iter(chain_list);
             while (iter != py::iterator::sentinel())
             {
               ChainRef ref = py::cast<ChainRef>(*iter);
               elements.push_back(std::addressof(static_cast<Chain&>(ref)));
               ++iter;
             }
             return ChainSelection(elements.begin(),elements.end());
           }),
           py::arg("chain_list"),
           "Construct ChainSelection from iterable with Chain elements")

      .def("filter",
           [](ChainSelection& sel, std::function<bool(ChainRef)>& predicate) {
             return sel.filter([&](const Chain& a) { return predicate(ChainRef(const_cast<Chain&>(a))); });
           },
           py::arg("predicate"),
           "Keeps in selection only elements that match predicate")

      .def(
          "index", // python predicates may alter atom, no way to prevent it
          [](ChainSelection& sel, std::function<bool(ChainRef)>& predicate) {
            auto result = new std::vector<int>(sel.index([&](const Chain& a) { return predicate(ChainRef(const_cast<Chain&>(a))); }));
            auto cap = py::capsule(result, [](void *v) { delete reinterpret_cast<std::vector<int>*>(v); });
            return py::array(result->size(), result->data(), cap);
          },
          py::arg("predicate"),
          "Return indices for which predicate is true")


      .def("for_each",
           [](ChainSelection& sel, std::function<void(ChainRef)>& func) {
             return sel.for_each([&](Chain& a) { func(ChainRef(a)); });
           },
           py::arg("transformation"),
           "Apply (mutating) transformation to selection")

      .def(
          "__len__",
          &ChainSelection::size)

      .def_property_readonly(
          "size", &ChainSelection::size)

      .def_property_readonly(
          "asResidues",
          [](ChainSelection& cSel) { return cSel.asResidues(); },
          "Returns selection of child residues")

      .def_property_readonly(
          "asAtoms",
          [](ChainSelection& cSel) { return cSel.asAtoms(); },
          "Returns selection of child chains")

      .def("to_pdb",
           [](ChainSelection& sel, std::string& path, xmol::pdb::basic_PdbRecords& db) {
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
           [](ChainSelection& sel, py::object fileHandle, xmol::pdb::basic_PdbRecords& db) {

             if (!(py::hasattr(fileHandle,"write") &&
                 py::hasattr(fileHandle,"flush") )){
               throw py::type_error("ChainSelection.to_pdb(file): incompatible function argument:  `file` must be a file-like object, but `"
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
          "__iter__",
          [](ChainSelection& sel) { return sel.begin(); }, py::keep_alive<0, 1>())

      .def(
          "__getitem__",
          [](ChainSelection& sel, py::buffer& b) {
            py::buffer_info info = b.request();

            /* Some sanity checks ... */
            if (info.ndim != 1) {
              throw std::runtime_error("ChainSelection::__getitem__(indices): index array dimension != 1");
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
                throw std::runtime_error("ChainSelection::__getitem__(indices): bool array size doesn't match selection size");
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
          [](ChainSelection& sel, int index) -> ChainRef { return ChainRef(sel[index]); },
          ChainRefPolicy,
          py::arg("n"),
          "Access n'th element")

      .def(
          "__getitem__",
          [](ChainSelection& sel, py::slice slice) {
            ssize_t start, stop, step, slicelength;
            if (!slice.compute(sel.size(),
                               reinterpret_cast<size_t*>(&start), reinterpret_cast<size_t*>(&stop),
                               reinterpret_cast<size_t*>(&step), reinterpret_cast<size_t*>(&slicelength))) {
              throw py::error_already_set();
            }
            return sel.slice(start, stop, step, xmol::selection::SlicingScheme::USE_INDICES_AS_IS);
          },
          py::arg("slice"),
          "Slice overload")

      .def(
          "__repr__",
          [](ChainSelection& selection) {
            return "<pyxmolpp2.polymer.ChainSelection size="
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
