#include "init.h"

#include "pybind11/functional.h"

#include "xmol/polymer/Atom.h"
#include "xmol/utils/string.h"
#include "xmol/pdb/PdbRecord.h"

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

  auto&& pyAtomSelection = py::class_<AtomSelection>(polymer, "AtomSelection");
  auto&& pyResidueSelection =
      py::class_<ResidueSelection>(polymer, "ResidueSelection");
  auto&& pyChainSelection =
      py::class_<ChainSelection>(polymer, "ChainSelection");

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
                             py::return_value_policy::reference)
      .def_property("r", &Atom::r, &Atom::set_r,
                    py::return_value_policy::reference_internal)
      .def_property_readonly("id", &Atom::id)
      .def_property_readonly("aId", &Atom::id)
      .def_property("name", &Atom::name, &Atom::set_name)
      .def_property_readonly("aName", &Atom::name)
      .def_property_readonly("rId", [](Atom& a) { return a.residue().id(); })
      .def_property_readonly("rName",
                             [](Atom& a) { return a.residue().name(); })
      .def_property_readonly(
          "cIndex", [](Atom& a) { return a.residue().chain().index(); })
      .def_property_readonly("cName",
                             [](Atom& a) { return a.residue().chain().name(); })
      .def_property_readonly(
          "fIndex", [](Atom& a) { return a.residue().chain().frame().index(); })
      .def_property_readonly(
          "chain", [](Atom& a) -> Chain& { return a.residue().chain(); },
          py::return_value_policy::reference)
      .def_property_readonly(
          "frame",
          [](Atom& a) -> Frame& { return a.residue().chain().frame(); },
          py::return_value_policy::reference)
      .def("delete", &Atom::set_deleted)
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
      .def("__len__", [](Residue& residue) { return residue.size(); })
      .def_property_readonly("size",
                             [](Residue& residue) { return residue.size(); })
      .def_property_readonly(
          "frame", [](Residue& r) -> Frame& { return r.chain().frame(); },
          py::return_value_policy::reference)
      .def("__getitem__",
           [](Residue& r, AtomName& name) -> Atom& { return r[name]; },
           py::return_value_policy::reference)
      .def_property_readonly("id", &Residue::id)
      .def_property_readonly("rId", &Residue::id)
      .def_property("name", &Residue::name, &Residue::set_name)
      .def_property_readonly("rName", &Residue::name)
      .def_property_readonly("cIndex",
                             [](Residue& r) { return r.chain().index(); })
      .def_property_readonly("cName",
                             [](Residue& r) { return r.chain().name(); })
      .def_property_readonly(
          "fIndex", [](Residue& r) { return r.chain().frame().index(); })
      .def_property_readonly("asAtoms",
                             [](Residue& residue) { return residue.asAtoms(); })
      .def("emplace", static_cast<Atom& (Residue::*)(AtomName, atomId_t, XYZ)>(
                          &Residue::emplace),
           py::return_value_policy::reference, py::arg("name"), py::arg("id"),
           py::arg("r"))
      .def("emplace",
           static_cast<Atom& (Residue::*)(const Atom&)>(&Residue::emplace),
           py::return_value_policy::reference, py::arg("atom"))
      .def("delete", &Residue::set_deleted)
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
                             py::return_value_policy::reference)
      .def("__len__", [](Chain& chain) { return chain.size(); })
      .def_property_readonly("size", [](Chain& chain) { return chain.size(); })
      .def_property_readonly("index", &Chain::index)
      .def_property_readonly("cIndex", &Chain::index)
      .def_property("name", &Chain::name, &Chain::set_name)
      .def_property_readonly("cName", &Chain::name)
      .def_property_readonly("fIndex",
                             [](Chain& c) { return c.frame().index(); })
      .def_property_readonly("asResidues",
                             [](Chain& chain) { return chain.asResidues(); })
      .def_property_readonly("asAtoms",
                             [](Chain& chain) { return chain.asAtoms(); })
      .def("emplace", (Residue & (Chain::*)(ResidueName, residueId_t, int)) &
                          Chain::emplace,
           py::return_value_policy::reference, py::arg("name"), py::arg("id"),
           py::arg("reserve") = 0)
      .def("emplace", (Residue & (Chain::*)(const Residue&)) & Chain::emplace,
           py::return_value_policy::reference, py::arg("residue"))
      .def("delete", &Chain::set_deleted)
      .def("__repr__", [](const Chain& chain) {
        return "<pyxmolpp2.polymer.Chain index=" +
               std::to_string(chain.index()) + " name=\"" + chain.name().str() +
               "\" at 0x" +
               xmol::utils::string::int2hex((uint64_t)(std::addressof(chain))) +
               ">";
      });
  ;

  pyFrame.def(py::init<frameIndex_t>())
      .def("__len__", &Frame::size)
      .def_property_readonly("size", &Frame::size)
      .def_property_readonly("index", &Frame::index)
      .def_property_readonly("asChains",
                             [](Frame& frame) { return frame.asChains(); })
      .def_property_readonly("asResidues",
                             [](Frame& frame) { return frame.asResidues(); })
      .def_property_readonly("asAtoms",
                             [](Frame& frame) { return frame.asAtoms(); })
      .def("emplace",
           static_cast<Chain& (Frame::*)(ChainName, int)>(&Frame::emplace),
           py::return_value_policy::reference, py::arg("index"),
           py::arg("reserve") = 0)
      .def("emplace",
           static_cast<Chain& (Frame::*)(const Chain&)>(&Frame::emplace),
           py::return_value_policy::reference, py::arg("chain"))
      .def("to_pdb", static_cast<void (Frame::*)(const std::string&) const>(
                         &Frame::to_pdb))
      .def("to_pdb",
           static_cast<void (Frame::*)(
               const std::string&, const xmol::pdb::basic_PdbRecords&) const>(
               &Frame::to_pdb))
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
      .def("filter",
           [](AtomSelection& sel, std::function<bool(const Atom&)>& predicate) {
             return sel.filter(predicate);
           })
      .def("for_each",
           [](AtomSelection& sel, std::function<void(Atom&)>& func) {
             return sel.for_each(func);
           })
      .def("__len__", [](AtomSelection& asel) { return asel.size(); })
      .def_property_readonly("size",
                             [](AtomSelection& asel) { return asel.size(); })
      .def_property_readonly(
          "toCoords", [](AtomSelection& aSel) { return aSel.toCoords(); })
      .def_property_readonly(
          "asChains", [](AtomSelection& aSel) { return aSel.asChains(); })
      .def_property_readonly(
          "asResidues", [](AtomSelection& aSel) { return aSel.asResidues(); })
      .def("__iter__", [](AtomSelection& sel) { return sel.begin(); },
           py::keep_alive<0, 1>())
      .def("__getitem__",
           [](AtomSelection& sel, int index) -> Atom& { return sel[index]; },
           py::return_value_policy::reference)
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
              std::function<bool(const Residue&)>& predicate) {
             return sel.filter(predicate);
           })
      .def("for_each",
           [](ResidueSelection& sel, std::function<void(Residue&)>& func) {
             return sel.for_each(func);
           })
      .def("__len__", &ResidueSelection::size)
      .def_property_readonly("size", &ResidueSelection::size)
      .def_property_readonly(
          "asChains", [](ResidueSelection& rSel) { return rSel.asChains(); })
      .def_property_readonly(
          "asAtoms", [](ResidueSelection& rSel) { return rSel.asAtoms(); })
      .def("__iter__", [](ResidueSelection& sel) { return sel.begin(); },
           py::keep_alive<0, 1>())
      .def("__getitem__", [](ResidueSelection& sel,
                             int index) -> Residue& { return sel[index]; },
           py::return_value_policy::reference)
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
              std::function<bool(const Chain&)>& predicate) {
             return sel.filter(predicate);
           })

      .def("for_each",
           [](ChainSelection& sel, std::function<void(Chain&)>& func) {
             return sel.for_each(func);
           })
      .def("__len__", &ChainSelection::size)
      .def_property_readonly("size", &ChainSelection::size)
      .def_property_readonly(
          "asResidues", [](ChainSelection& cSel) { return cSel.asResidues(); })
      .def_property_readonly(
          "asAtoms", [](ChainSelection& cSel) { return cSel.asAtoms(); })
      .def("__iter__", [](ChainSelection& sel) { return sel.begin(); },
           py::keep_alive<0, 1>())
      .def("__getitem__",
           [](ChainSelection& sel, int index) -> Chain& { return sel[index]; },
           py::return_value_policy::reference)
      .def("__repr__", [](const ChainSelection& selection) {
        return "<pyxmolpp2.polymer.ChainSelection size=" +
               std::to_string(selection.size()) + " at 0x" +
               xmol::utils::string::int2hex(
                   (uint64_t)(std::addressof(selection))) +
               ">";
      });
}
