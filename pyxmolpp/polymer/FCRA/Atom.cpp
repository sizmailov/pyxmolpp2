#include "../FCRA.h"
#include "xmol/polymer/Atom.h"
#include "xmol/utils/string.h"

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