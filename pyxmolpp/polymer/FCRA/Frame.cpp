#include "../FCRA.h"

#include "pybind11/iostream.h"

#include "xmol/polymer/Atom.h"
#include "xmol/utils/string.h"
#include "xmol/pdb/PdbRecord.h"
#include "xmol/pdb/PdbWriter.h"

void pyxmolpp::polymer::init_Frame(detail::FWD& fwd, pybind11::module &polymer) {

  using namespace xmol::polymer;

  (*fwd.pyFrame)
      .def(py::init<frameIndex_t>(),py::arg("frame_index"))
      .def("__len__", &Frame::size, "Returns number of chains")
      .def("__getitem__",
          [](Frame& frame, ChainName& name) { return ChainRef(frame[name]); })
      .def_property_readonly("size", &Frame::size, "Number of chains")
      .def_property_readonly("index", [](Frame& frame) { return frame.index(); }, "Frame index")
      .def_property_readonly("asChains", [](Frame& frame) { return frame.asChains(); },
                             "Returns selection of child chains")
      .def_property_readonly("asResidues", [](Frame& frame) { return frame.asResidues(); },
                             "Returns selection of child residues")
      .def_property_readonly("asAtoms", [](Frame& frame) { return frame.asAtoms(); },
                             "Returns selection of child atoms")
      .def("emplace",
           [](Frame& frame, ChainName name, int reserve) -> ChainRef { return ChainRef(frame.emplace(name, reserve)); },
           ChainRefPolicy, py::arg("index"), py::arg("reserve") = 0, "Adds an empty Chain to the end of frame")
      .def("emplace",
           +[](Frame& frame, ChainRef& chain) -> ChainRef {
             return ChainRef(frame.emplace(static_cast<Chain&>(chain)));
           },
           ChainRefPolicy, py::arg("chain"), "Adds a copy of given Chain to the end of frame")
      .def("to_pdb",
           [](Frame& frame, std::string& path, xmol::pdb::basic_PdbRecords& db) {
             frame.to_pdb(path, db);
           },
           py::arg("path_or_buf"),
           py::arg_v("db",std::ref(xmol::pdb::StandardPdbRecords::instance()),"pyxmolpp2.pdb.StandardPdbRecords.instance()"),
           "Writes frame to file using `db` as non-standard PDB records"
      )
      .def("to_pdb",
           [](Frame& frame, py::object fileHandle, xmol::pdb::basic_PdbRecords& db) {

             if (!(py::hasattr(fileHandle,"write") &&
                 py::hasattr(fileHandle,"flush") )){
               throw py::type_error("Frame.to_pdb(file): incompatible function argument:  `file` must be a file-like object, but `"
                                        +(std::string)(py::repr(fileHandle))+"` provided"
               );
             }
             py::detail::pythonbuf buf(fileHandle);
             std::ostream stream(&buf);
             xmol::pdb::PdbWriter writer(stream);
             writer.write(frame, db);
           },
           py::arg("path_or_buf"),
           py::arg_v("db",std::ref(xmol::pdb::StandardPdbRecords::instance()),"pyxmolpp2.pdb.StandardPdbRecords.instance()"),
           "Writes frame to `file` using `db` as non-standard PDB records"
      )
      .def("copy",[](Frame& frame) -> Frame{
        return frame;
      }, py::return_value_policy::move)
      .def("__repr__", [](Frame& frame) {
        return "<pyxmolpp2.polymer.Frame index=" + std::to_string(frame.index()) + " at 0x" +
               xmol::utils::string::int2hex((uint64_t)(std::addressof(frame))) + ">";
      });
  ;
}