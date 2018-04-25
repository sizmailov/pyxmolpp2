#include "init.h"

#include "pybind11/functional.h"

#include "xmol/pdb/PdbRecord.h"
#include "xmol/polymer/Atom.h"
#include "xmol/utils/string.h"

void pyxmolpp::polymer::init_Atom(pybind11::module& polymer) {

  using namespace xmol::polymer;
  namespace py = pybind11;

  auto&& pyAtomName = py::class_<AtomName>(polymer, "AtomName");
  auto&& pyResidueName = py::class_<ResidueName>(polymer, "ResidueName");
  auto&& pyChainName = py::class_<ChainName>(polymer, "ChainName");

  auto&& pyAtom = py::class_<Atom>(polymer, "Atom");
  auto&& pyResidue = py::class_<Residue>(polymer, "Residue");
  auto&& pyChain = py::class_<Chain>(polymer, "Chain");
  auto&& pyFrame = py::class_<Frame>(polymer, "Frame");

  auto&& pyAtomSelection =
      py::class_<AtomSelection>(polymer, "AtomSelection", R"pydoc(
Ordered list of atoms.

Order of atoms within single Residue matches their construction order.
Atoms from different residues are ordered as parent residues, see ResidueSelection for details.

Order is preserved across manipulations with AtomSelection

)pydoc");
  auto&& pyResidueSelection =
      py::class_<ResidueSelection>(polymer, "ResidueSelection", R"pydoc(
Ordered list of residues.

Order of residues within single Chain matches their construction order.
Residues from different chains are ordered as parent chains, see ChainSelection for details.

Order is preserved across manipulations with ResidueSelection

)pydoc");
  auto&& pyChainSelection =
      py::class_<ChainSelection>(polymer, "ChainSelection", R"pydoc(
Ordered list of chains.

Order of chains within single Frame matches their construction order.
Chains from different frames are ordered as parent frames.

Frames are ordered by Frame.index. If two frames have identical index, order is undefined.
Generally it's a *bad* idea to have Atom/Residue/Chain selection with elements from two frames with same index.

Order is preserved across manipulations with ChainSelection

)pydoc");

  auto&& pyAtomSelectionRange =
      py::class_<xmol::selection::SelectionRange<Atom>>(polymer,
                                                        "AtomSelectionRange");
  auto&& pyResidueSelectionRange =
      py::class_<xmol::selection::SelectionRange<Residue>>(
          polymer, "ResidueSelectionRange");
  auto&& pyChainSelectionRange =
      py::class_<xmol::selection::SelectionRange<Chain>>(polymer,
                                                         "ChainSelectionRange");

  pyAtomName.def(py::init<const std::string&>())
      .def_property_readonly("str", &AtomName::str)
      .def("__hash__", &AtomName::value)
      .def("__str__",
           [](const AtomName& name) { return '"' + name.str() + '"'; })
      .def("__repr__", [](const AtomName& name) {
        return "<pyxmolpp2.polymer.AtomName \"" + name.str() + "\" at 0x" +
               xmol::utils::string::int2hex((uint64_t)(std::addressof(name))) +
               ">";
      });

  pyResidueName.def(py::init<const std::string&>())
      .def_property_readonly("str", &ResidueName::str)
      .def("__hash__", &ResidueName::value)
      .def("__str__",
           [](const ResidueName& name) { return '"' + name.str() + '"'; })
      .def("__repr__", [](const ResidueName& name) {
        return "<pyxmolpp2.polymer.ResidueName \"" + name.str() + "\" at 0x" +
               xmol::utils::string::int2hex((uint64_t)(std::addressof(name))) +
               ">";
      });

  pyChainName.def(py::init<const std::string&>())
      .def_property_readonly("str", &ChainName::str)
      .def("__hash__", &ChainName::value)
      .def("__str__",
           [](const ChainName& name) { return '"' + name.str() + '"'; })
      .def("__repr__", [](const ChainName& name) {
        return "<pyxmolpp2.polymer.ChainName \"" + name.str() + "\" at 0x" +
               xmol::utils::string::int2hex((uint64_t)(std::addressof(name))) +
               ">";
      });

  pyAtom
      .def_property_readonly("residue",
                             static_cast<Residue& (Atom::*)()>(&Atom::residue),
                             py::return_value_policy::reference,
                             "Parent residue, guarantied to be not None")
      .def_property("r", &Atom::r, &Atom::set_r,
                    py::return_value_policy::reference_internal,
                    "Atom coordinates")
      .def_property_readonly("id", &Atom::id, "Atom id")
      .def_property_readonly("aId", &Atom::id, "Atom id")
      .def_property("name", &Atom::name, &Atom::set_name, "Atom name")
      .def_property_readonly("aName", &Atom::name, "Atom name")
      .def_property_readonly("rId", [](Atom& a) { return a.residue().id(); },
                             "Residue id")
      .def_property_readonly(
          "rName", [](Atom& a) { return a.residue().name(); }, "Residue name")
      .def_property_readonly(
          "cIndex", [](Atom& a) { return a.residue().chain().index(); },
          "Chain index")
      .def_property_readonly("cName",
                             [](Atom& a) { return a.residue().chain().name(); },
                             "Chain name (chainID in PDB nomenclature)")
      .def_property_readonly(
          "fIndex", [](Atom& a) { return a.residue().chain().frame().index(); },
          "Frame index")
      .def_property_readonly(
          "chain", [](Atom& a) -> Chain& { return a.residue().chain(); },
          py::return_value_policy::reference, "Parent chain")
      .def_property_readonly(
          "frame",
          [](Atom& a) -> Frame& { return a.residue().chain().frame(); },
          py::return_value_policy::reference, "Parent frame")
      .def("delete", &Atom::set_deleted,
           "Mark atom as deleted. Further access to deleted atom is illegal.")
      .def("__repr__", [](const Atom& atom) {
        return "<pyxmolpp2.polymer.Atom id=" + std::to_string(atom.id()) +
               " name=\"" + atom.name().str() + "\" at 0x" +
               xmol::utils::string::int2hex((uint64_t)(std::addressof(atom))) +
               ">";
      });
  ;

  pyResidue
      .def_property_readonly(
          "chain", static_cast<Chain& (Residue::*)()>(&Residue::chain),
          py::return_value_policy::reference)
      .def("__len__", [](Residue& residue) { return residue.size(); },
           "Returns number of atoms in residues")
      .def_property_readonly("size",
                             [](Residue& residue) { return residue.size(); },
                             "Returns number of atoms in residues")
      .def_property_readonly(
          "frame", [](Residue& r) -> Frame& { return r.chain().frame(); },
          py::return_value_policy::reference, "Parent frame")
      .def("__getitem__",
           [](Residue& r, AtomName& name) -> Atom& { return r[name]; },
           py::return_value_policy::reference,
           "Returns first atom in residue with given name. If no atoms with "
           "such name exception raised.")
      .def_property_readonly("id", &Residue::id, "Residue id")
      .def_property_readonly("rId", &Residue::id, "Residue id")
      .def_property("name", &Residue::name, &Residue::set_name, "Residue name")
      .def_property_readonly("rName", &Residue::name, "Residue name")
      .def_property_readonly(
          "cIndex", [](Residue& r) { return r.chain().index(); }, "Chain index")
      .def_property_readonly("cName",
                             [](Residue& r) { return r.chain().name(); },
                             "Chain name (chainID in PDB nomenclature)")
      .def_property_readonly(
          "fIndex", [](Residue& r) { return r.chain().frame().index(); },
          "Frame index")
      .def_property_readonly("asAtoms",
                             [](Residue& residue) { return residue.asAtoms(); },
                             "Returns selection of child atoms")
      .def("emplace", static_cast<Atom& (Residue::*)(AtomName, atomId_t, XYZ)>(
                          &Residue::emplace),
           py::return_value_policy::reference, py::arg("name"), py::arg("id"),
           py::arg("r"), "Adds new Atom to the end of residue")
      .def("emplace",
           static_cast<Atom& (Residue::*)(const Atom&)>(&Residue::emplace),
           py::return_value_policy::reference, py::arg("atom"),
           "Adds a copy of given Atom to the end of residue")
      .def("delete", &Residue::set_deleted, "Marks residue as deleted. Further "
                                            "access to deleted residue is "
                                            "illegal")
      .def("__repr__", [](const Residue& residue) {
        return "<pyxmolpp2.polymer.Residue id=" + std::to_string(residue.id()) +
               " name=\"" + residue.name().str() + "\" at 0x" +
               xmol::utils::string::int2hex(
                   (uint64_t)(std::addressof(residue))) +
               ">";
      });
  ;

  pyChain
      .def_property_readonly("frame",
                             static_cast<Frame& (Chain::*)()>(&Chain::frame),
                             py::return_value_policy::reference, "Parent frame")
      .def("__len__", [](Chain& chain) { return chain.size(); },
           "Returns number of residues in chain")
      .def_property_readonly("size", [](Chain& chain) { return chain.size(); },
                             "Returns number of residues in chain")
      .def_property_readonly("index", &Chain::index,
                             "Chain index, starts from 0")
      .def_property_readonly("cIndex", &Chain::index,
                             "Chain index, starts from 0")
      .def_property("name", &Chain::name, &Chain::set_name,
                    "Chain name (chainID in PDB nomenclature)")
      .def_property_readonly("cName", &Chain::name,
                             "Chain name (chainID in PDB nomenclature)")
      .def_property_readonly(
          "fIndex", [](Chain& c) { return c.frame().index(); }, "Frame index")
      .def_property_readonly("asResidues",
                             [](Chain& chain) { return chain.asResidues(); },
                             "Returns selection of child residues")
      .def_property_readonly("asAtoms",
                             [](Chain& chain) { return chain.asAtoms(); },
                             "Returns selection of child atoms")
      .def("emplace", (Residue & (Chain::*)(ResidueName, residueId_t, int)) &
                          Chain::emplace,
           py::return_value_policy::reference, py::arg("name"), py::arg("id"),
           py::arg("reserve") = 0, "Adds an empty Residue to the end of chain")
      .def("emplace", (Residue & (Chain::*)(const Residue&)) & Chain::emplace,
           py::return_value_policy::reference, py::arg("residue"),
           "Adds a copy of given Residue to the end of chain")
      .def("delete", &Chain::set_deleted,
           "Marks chain as deleted. Further access to deleted chain is illegal")
      .def("__repr__", [](const Chain& chain) {
        return "<pyxmolpp2.polymer.Chain index=" +
               std::to_string(chain.index()) + " name=\"" + chain.name().str() +
               "\" at 0x" +
               xmol::utils::string::int2hex((uint64_t)(std::addressof(chain))) +
               ">";
      });
  ;

  pyFrame.def(py::init<frameIndex_t>())
      .def("__len__", &Frame::size, "Returns number of chains")
      .def_property_readonly("size", &Frame::size, "Number of chains")
      .def_property_readonly("index", &Frame::index, "Frame index")
      .def_property_readonly("asChains",
                             [](Frame& frame) { return frame.asChains(); },
                             "Returns selection of child chains")
      .def_property_readonly("asResidues",
                             [](Frame& frame) { return frame.asResidues(); },
                             "Returns selection of child residues")
      .def_property_readonly("asAtoms",
                             [](Frame& frame) { return frame.asAtoms(); },
                             "Returns selection of child atoms")
      .def("emplace",
           static_cast<Chain& (Frame::*)(ChainName, int)>(&Frame::emplace),
           py::return_value_policy::reference, py::arg("index"),
           py::arg("reserve") = 0, "Adds an empty Chain to the end of frame")
      .def("emplace",
           static_cast<Chain& (Frame::*)(const Chain&)>(&Frame::emplace),
           py::return_value_policy::reference, py::arg("chain"),
           "Adds a copy of given Chain to the end of frame")
      .def("to_pdb", static_cast<void (Frame::*)(const std::string&) const>(
                         &Frame::to_pdb),
           py::arg("output_filename"),
           "Writes frame to file using standard PDB records")
      .def("to_pdb",
           static_cast<void (Frame::*)(
               const std::string&, const xmol::pdb::basic_PdbRecords&) const>(
               &Frame::to_pdb),
           py::arg("output_filename"), py::arg("db"),
           "Writes frame to file using db as non-standard PDB records")
      .def("__repr__", [](const Frame& frame) {
        return "<pyxmolpp2.polymer.Frame index=" +
               std::to_string(frame.index()) + " at 0x" +
               xmol::utils::string::int2hex((uint64_t)(std::addressof(frame))) +
               ">";
      });
  ;

  //   .def("__iter__", [](xmol::selection::SelectionRange<Atom>& rng) -> sel& {
  //   return rng; })
  pyAtomSelectionRange.def(
      "__next__",
      [](xmol::selection::SelectionRange<Atom>& rng) -> Atom& {
        if (rng != rng) {
          Atom& a = *rng;
          ++rng;
          return a;
        }
        throw py::stop_iteration();
      },
      py::return_value_policy::reference);

  pyResidueSelectionRange.def(
      "__next__",
      [](xmol::selection::SelectionRange<Residue>& rng) -> Residue& {
        if (rng != rng) {
          Residue& a = *rng;
          ++rng;
          return a;
        }
        throw py::stop_iteration();
      },
      py::return_value_policy::reference);

  pyChainSelectionRange.def(
      "__next__",
      [](xmol::selection::SelectionRange<Chain>& rng) -> Chain& {
        if (rng != rng) {
          Chain& a = *rng;
          ++rng;
          return a;
        }
        throw py::stop_iteration();
      },
      py::return_value_policy::reference);

  pyAtomSelection
      .def("filter", [](AtomSelection& sel,
                        std::function<bool(std::reference_wrapper<const Atom>)>&
                            predicate) { return sel.filter(predicate); },
           py::arg("predicate"),
           "Keeps in selection only elements that match predicate")
      .def("for_each",
           [](AtomSelection& sel,
              std::function<void(std::reference_wrapper<Atom>)>& func) {
             return sel.for_each(func);
           },
           py::arg("transformation"),
           "Applies (mutating) transformation to atoms")
      .def("__len__", [](AtomSelection& asel) { return asel.size(); },
           "Returns number of elements")
      .def_property_readonly("size",
                             [](AtomSelection& asel) { return asel.size(); },
                             "Returns number of elements")
      .def_property_readonly(
          "toCoords", [](AtomSelection& aSel) { return aSel.toCoords(); },
          "Returns copy of atom coordinates")
      .def_property_readonly(
          "asChains", [](AtomSelection& aSel) { return aSel.asChains(); },
          "Returns selection of parent chains")
      .def_property_readonly(
          "asResidues", [](AtomSelection& aSel) { return aSel.asResidues(); },
          "Returns selection of parent residues")
      .def("__iter__", [](AtomSelection& sel) { return sel.begin(); },
           py::keep_alive<0, 1>())
      .def("__getitem__",
           [](AtomSelection& sel, int index) -> Atom& { return sel[index]; },
           py::return_value_policy::reference, py::arg("n"), "Get n'th element")
      .def("__repr__", [](const AtomSelection& selection) {
        return "<pyxmolpp2.polymer.AtomSelection size=" +
               std::to_string(selection.size()) + " at 0x" +
               xmol::utils::string::int2hex(
                   (uint64_t)(std::addressof(selection))) +
               ">";
      });

  pyResidueSelection
      .def("filter",
           [](ResidueSelection& sel,
              std::function<bool(std::reference_wrapper<const Residue>)>&
                  predicate) { return sel.filter(predicate); },
           py::arg("predicate"),
           "Keeps in selection only elements that match predicate")
      .def("for_each",
           [](ResidueSelection& sel,
              std::function<void(std::reference_wrapper<Residue>)>& func) {
             return sel.for_each(func);
           },
           py::arg("transformation"),
           "Apply (mutating) transformation to selection")
      .def("__len__", &ResidueSelection::size, "Returns number of elements")
      .def_property_readonly("size", &ResidueSelection::size,
                             "Returns number of elements")
      .def_property_readonly(
          "asChains", [](ResidueSelection& rSel) { return rSel.asChains(); },
          "Returns selection of parent chains")
      .def_property_readonly(
          "asAtoms", [](ResidueSelection& rSel) { return rSel.asAtoms(); },
          "Returns selection of child atoms")
      .def("__iter__", [](ResidueSelection& sel) { return sel.begin(); },
           py::keep_alive<0, 1>())
      .def("__getitem__", [](ResidueSelection& sel,
                             int index) -> Residue& { return sel[index]; },
           py::return_value_policy::reference, py::arg("n"), "Get n'th element")
      .def("__repr__", [](const ResidueSelection& selection) {
        return "<pyxmolpp2.polymer.ResidueSelection size=" +
               std::to_string(selection.size()) + " at 0x" +
               xmol::utils::string::int2hex(
                   (uint64_t)(std::addressof(selection))) +
               ">";
      });

  pyChainSelection
      .def("filter",
           [](ChainSelection& sel,
              std::function<bool(std::reference_wrapper<const Chain>)>&
                  predicate) { return sel.filter(predicate); },
           py::arg("predicate"),
           "Keeps in selection only elements that match predicate")

      .def("for_each",
           [](ChainSelection& sel,
              std::function<void(std::reference_wrapper<Chain>)>& func) {
             return sel.for_each(func);
           },
           py::arg("transformation"),
           "Apply (mutating) transformation to selection")
      .def("__len__", &ChainSelection::size)
      .def_property_readonly("size", &ChainSelection::size)
      .def_property_readonly(
          "asResidues", [](ChainSelection& cSel) { return cSel.asResidues(); },
          "Returns selection of child residues")
      .def_property_readonly(
          "asAtoms", [](ChainSelection& cSel) { return cSel.asAtoms(); },
          "Returns selection of child chains")
      .def("__iter__", [](ChainSelection& sel) { return sel.begin(); },
           py::keep_alive<0, 1>())
      .def("__getitem__",
           [](ChainSelection& sel, int index) -> Chain& { return sel[index]; },
           py::return_value_policy::reference, py::arg("n"), "Get n'th element")
      .def("__repr__", [](const ChainSelection& selection) {
        return "<pyxmolpp2.polymer.ChainSelection size=" +
               std::to_string(selection.size()) + " at 0x" +
               xmol::utils::string::int2hex(
                   (uint64_t)(std::addressof(selection))) +
               ">";
      });
}
