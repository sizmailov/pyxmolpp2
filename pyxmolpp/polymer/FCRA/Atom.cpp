#include "../FCRA.h"

#include "xmol/pdb/PdbRecord.h"
#include "xmol/pdb/PdbWriter.h"
#include "xmol/polymer/Atom.h"
#include "xmol/utils/string.h"
#include "xmol/polymer/exceptions.h"

#include "pybind11/iostream.h"

#include <fstream>
void pyxmolpp::polymer::init_Atom(detail::FWD& fwd, py::module& polymer){

  using namespace xmol::polymer;


  (*fwd.pyAtom)
      .def_property_readonly(
          "residue",
          [](AtomRef& ref) -> ResidueRef { return ResidueRef(static_cast<Atom&>(ref).residue()); },
          ResidueRefPolicy,
          "Parent residue, guarantied to be not None")

      .def_property(
          "r",
          [](AtomRef& ref) -> const XYZ& { return static_cast<Atom&>(ref).r(); },
          [](AtomRef& ref, XYZ value) { static_cast<Atom&>(ref).set_r(value); },
          "Atom coordinates")

      .def_property(
          "id",
          [](AtomRef& ref) { return static_cast<Atom&>(ref).id(); },
          [](AtomRef& ref, atomId_t value) {static_cast<Atom&>(ref).set_id(std::move(value));},
          "Atom id")

      .def("__eq__",
           [](AtomRef& a, AtomRef& b) -> bool { return &static_cast<Atom&>(a) == &static_cast<Atom&>(b); })

      .def_property(
          "aId",
          [](AtomRef& ref) { return static_cast<Atom&>(ref).id(); },
          [](AtomRef& ref, atomId_t value) {static_cast<Atom&>(ref).set_id(std::move(value));},
          "Atom id")

      .def_property(
          "name",
          [](AtomRef& ref) { return static_cast<Atom&>(ref).name(); },
          [](AtomRef& ref, AtomName value) {static_cast<Atom&>(ref).set_name(value);},
          "Atom name")

      .def_property(
          "aName",
          [](AtomRef& ref) { return static_cast<Atom&>(ref).name(); },
          [](AtomRef& ref, AtomName value) {static_cast<Atom&>(ref).set_name(value);},
          "Atom name"
          )
      .def_property_readonly(
          "rId",
          [](AtomRef& ref) { return static_cast<Atom&>(ref).residue().id(); },
          "Residue id")

      .def_property_readonly(
          "rName",
          [](AtomRef& ref) { return static_cast<Atom&>(ref).residue().name(); },
          "Residue name")

      .def_property_readonly(
          "cIndex",
          [](AtomRef& ref) { return static_cast<Atom&>(ref).residue().chain().index(); },
          "Chain index")

      .def_property_readonly(
          "cName",
          [](AtomRef& ref) { return static_cast<Atom&>(ref).residue().chain().name(); },
          "Chain name (chainID in PDB nomenclature)"
          )

      .def_property_readonly(
          "fIndex",
          [](AtomRef& ref) { return static_cast<Atom&>(ref).residue().chain().frame().index(); },
          "Frame index")

      .def_property_readonly(
          "chain",
          [](AtomRef& ref) -> ChainRef { return ChainRef(static_cast<Atom&>(ref).residue().chain()); },
          ChainRefPolicy,
          "Parent chain")

      .def_property_readonly(
          "frame",
          [](AtomRef& ref) -> Frame& { return static_cast<Atom&>(ref).residue().chain().frame(); },
          FrameRefPolicy,
          "Parent frame"
          )
      .def("to_pdb",
           [](AtomRef& a, std::string& path, xmol::pdb::basic_PdbRecords& db) {
             std::ofstream out(path);
             if (out.fail()) {
               throw IOError("Can't open file `" + path + "` for writing");
             }
             xmol::pdb::PdbWriter writer(out);
             writer.write(static_cast<Atom&>(a), db);
           },
           py::arg("path_or_buf"),
           py::arg_v("db",std::ref(xmol::pdb::StandardPdbRecords::instance()),"pyxmolpp2.pdb.StandardPdbRecords.instance()"),
           "Writes frame to file using `db` as non-standard PDB records"
      )
      .def("to_pdb",
           [](AtomRef& a, py::object fileHandle, xmol::pdb::basic_PdbRecords& db) {

             if (!(py::hasattr(fileHandle,"write") &&
                 py::hasattr(fileHandle,"flush") )){
               throw py::type_error("AtomSelection.to_pdb(file): incompatible function argument:  `file` must be a file-like object, but `"
                                        +(std::string)(py::repr(fileHandle))+"` provided"
               );
             }
             py::detail::pythonbuf buf(fileHandle);
             std::ostream stream(&buf);
             xmol::pdb::PdbWriter writer(stream);
             writer.write(static_cast<Atom&>(a), db);
           },
           py::arg("path_or_buf"),
           py::arg_v("db",std::ref(xmol::pdb::StandardPdbRecords::instance()),"pyxmolpp2.pdb.StandardPdbRecords.instance()"),
           "Writes selection to `file` using `db` as non-standard PDB records"
      )
      .def("delete",
          [](AtomRef& ref) { return static_cast<Atom&>(ref).set_deleted(); },
          "Mark atom as deleted. Further access to deleted atom is illegal.")

      .def("__repr__",
          [](AtomRef& atom) {
                return "<pyxmolpp2.polymer.Atom id="
                + std::to_string(static_cast<Atom&>(atom).id())
                + " name=\""
                + static_cast<Atom&>(atom).name().str()
                + "\" at 0x"
                + xmol::utils::string::int2hex((uint64_t)(std::addressof(static_cast<Atom&>(atom))))
                + ">";
      });
  ;

}