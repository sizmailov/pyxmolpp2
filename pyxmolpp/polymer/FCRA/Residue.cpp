#include "../FCRA.h"
#include "xmol/pdb/PdbRecord.h"
#include "xmol/pdb/PdbWriter.h"
#include "xmol/polymer/Atom.h"
#include "xmol/polymer/exceptions.h"
#include "xmol/utils/string.h"

#include "pybind11/iostream.h"

#include <fstream>
void pyxmolpp::polymer::init_Residue(detail::FWD& fwd, pybind11::module &polymer) {

  using namespace xmol::polymer;


  (*fwd.pyResidue)
      .def_property_readonly(
          "chain",
          [](ResidueRef& residue) -> ChainRef { return ChainRef(static_cast<Residue&>(residue).chain()); },
          ChainRefPolicy)

      .def(
          "__len__",
          [](ResidueRef& residue) { return static_cast<Residue&>(residue).size(); },
          "Returns number of atoms in residues")

      .def_property_readonly(
          "size",
          [](ResidueRef& residue) { return static_cast<Residue&>(residue).size(); },
          "Returns number of atoms in residues")

      .def_property_readonly(
          "frame",
          [](ResidueRef& residue) -> FrameRef& {
            return FrameRef(static_cast<Residue&>(residue).chain().frame());
          },
          FrameRefPolicy,
          "Parent frame")

      .def(
          "__eq__",
          [](ResidueRef& a, ResidueRef& b) -> bool {
            return &static_cast<Residue&>(a) == &static_cast<Residue&>(b);
          })

      .def(
          "__getitem__",
          [](ResidueRef& residue, AtomName& name) -> AtomRef {
            return AtomRef(static_cast<Residue&>(residue)[name]);
          },
          AtomRefPolicy,
          "Returns first atom in residue with given name. If no atoms with such name exception raised.")

      .def_property(
          "id",
          [](ResidueRef& residue) { return static_cast<Residue&>(residue).id(); },
          [](ResidueRef& residue, ResidueId& rid) { static_cast<Residue&>(residue).set_id(rid); },
          "Residue id")

      .def_property(
          "rId",
          [](ResidueRef& residue) { return static_cast<Residue&>(residue).id(); },
          [](ResidueRef& residue, ResidueId& rid) { static_cast<Residue&>(residue).set_id(rid); },
          "Residue id")

      .def_property(
          "name",
          [](ResidueRef& residue) { return static_cast<Residue&>(residue).name(); },
          [](ResidueRef& residue, ResidueName value){ static_cast<Residue&>(residue).set_name(value); },
          ResidueRefPolicy,
          "Residue name")

      .def_property(
          "rName",
          [](ResidueRef& residue) { return static_cast<Residue&>(residue).name(); },
          [](ResidueRef& residue, ResidueName value){ static_cast<Residue&>(residue).set_name(value); },
          ResidueRefPolicy,
          "Residue name")

      .def_property_readonly(
          "cIndex",
          [](ResidueRef& residue) { return static_cast<Residue&>(residue).chain().index(); },
          "Chain index")

      .def_property_readonly(
          "cName",
          [](ResidueRef& residue) { return static_cast<Residue&>(residue).chain().name(); },
          "Chain name (chainID in PDB nomenclature)"
      )

      .def_property_readonly(
          "fIndex",
          [](ResidueRef& residue) { return static_cast<Residue&>(residue).chain().frame().index(); },
          "Frame index")

      .def_property_readonly(
          "asAtoms",
          [](ResidueRef& residue) { return static_cast<Residue&>(residue).asAtoms(); },
          "Returns selection of child atoms"
      )

      .def(
          "emplace",
          [](ResidueRef& residue, AtomName name, atomId_t id, XYZ xyz) -> AtomRef {
            return AtomRef(static_cast<Residue&>(residue).emplace(name, id, xyz));
          },
          AtomRefPolicy,
          py::arg("name"),
          py::arg("id"),
          py::arg("r"),
          "Adds new Atom to the end of residue"
      )

      .def(
          "emplace",
          [](ResidueRef& residue, AtomRef& atom) -> AtomRef {
            return AtomRef(static_cast<Residue&>(residue).emplace(static_cast<Atom&>(atom)));
          },
          AtomRefPolicy,
          py::arg("atom"),
          "Adds a copy of given Atom to the end of residue")

      .def(
          "delete",
          [](ResidueRef& residue) { static_cast<Residue&>(residue).set_deleted(); },
          "Marks residue as deleted. Further access to deleted residue is illegal"
      )
      .def("to_pdb",
           [](ResidueRef& c, std::string& path, xmol::pdb::basic_PdbRecords& db) {
             std::ofstream out(path);
             if (out.fail()) {
               throw IOError("Can't open file `" + path + "` for writing");
             }
             xmol::pdb::PdbWriter writer(out);
             writer.write(static_cast<Residue&>(c), db);
           },
           py::arg("path_or_buf"),
           py::arg_v("db",std::ref(xmol::pdb::StandardPdbRecords::instance()),"pyxmolpp2.pdb.StandardPdbRecords.instance()"),
           "Writes residue atoms to file using `db` as non-standard PDB records"
      )
      .def("to_pdb",
           [](ResidueRef& c, py::object fileHandle, xmol::pdb::basic_PdbRecords& db) {

             if (!(py::hasattr(fileHandle,"write") &&
                 py::hasattr(fileHandle,"flush") )){
               throw py::type_error("Residue.to_pdb(file): incompatible function argument:  `file` must be a file-like object, but `"
                                        +(std::string)(py::repr(fileHandle))+"` provided"
               );
             }
             py::detail::pythonbuf buf(fileHandle);
             std::ostream stream(&buf);
             xmol::pdb::PdbWriter writer(stream);
             writer.write(static_cast<Residue&>(c), db);
           },
           py::arg("path_or_buf"),
           py::arg_v("db",std::ref(xmol::pdb::StandardPdbRecords::instance()),"pyxmolpp2.pdb.StandardPdbRecords.instance()"),
           "Writes residue atoms to `file` using `db` as non-standard PDB records"
      )

      .def(
          "__repr__",
          [](ResidueRef& residue) {
            auto& r = static_cast<Residue&>(residue);
            return "<pyxmolpp2.polymer.Residue id="
                + std::to_string(r.id().serial)
                + r.id().iCode.str()
                + " name=\""
                + r.name().str()
                + "\" at 0x"
                + xmol::utils::string::int2hex((uint64_t)(std::addressof(r)))
                + ">";
          });
  ;
}
