#include <pybind11/pybind11.h>

#include "xmol/polymer/Atom.h"
#include "xmol/utils/string.h"

namespace py = pybind11;

PYBIND11_MODULE(pyxmolpp2, m) {
  m.doc() = "pyxmolpp2 module";

  {
    pybind11::module utils = m.def_submodule("utils", "pyxmolpp2.utils module");
    using namespace xmol::utils;
  }

  {
    pybind11::module polymer = m.def_submodule("polymer", "pyxmolpp2.polymer module");
    using namespace xmol::polymer;

    py::class_<AtomName>(polymer, "AtomName")
        .def_property_readonly("str", &AtomName::str)
        .def_property_readonly("value", &AtomName::value)
        .def("__str__",
             [](const AtomName& name) { return '"' + name.str() + '"'; })
        .def("__repr__", [](const AtomName& name) {
          return "<pyxmolpp2.pdb.AtomName \"" + name.str() + "\" at 0x" +
                 xmol::utils::string::int2hex(
                     (uint64_t)(std::addressof(name))) +
                 ">";
        });

    py::class_<ResidueName>(polymer, "ResidueName")
        .def_property_readonly("str", &ResidueName::str)
        .def_property_readonly("value", &ResidueName::value)
        .def("__str__",
             [](const ResidueName& name) { return '"' + name.str() + '"'; })
        .def("__repr__", [](const ResidueName& name) {
          return "<pyxmolpp2.pdb.ResidueName \"" + name.str() + "\" at 0x" +
                 xmol::utils::string::int2hex(
                     (uint64_t)(std::addressof(name))) +
                 ">";
        });

    py::class_<ChainName>(polymer, "ChainName")
        .def_property_readonly("str", &ChainName::str)
        .def_property_readonly("value", &ChainName::value)
        .def("__str__",
             [](const ChainName& name) { return '"' + name.str() + '"'; })
        .def("__repr__", [](const ChainName& name) {
          return "<pyxmolpp2.pdb.ChainName \"" + name.str() + "\" at 0x" +
                 xmol::utils::string::int2hex(
                     (uint64_t)(std::addressof(name))) +
                 ">";
        });

    py::class_<Atom>(polymer, "Atom")
        .def_property_readonly(
            "residue", static_cast<Residue& (Atom::*)()>(&Atom::residue),
            py::return_value_policy::reference)
        .def_property_readonly("id", &Atom::id)
        .def_property_readonly("aId", &Atom::id)
        .def_property_readonly("name", &Atom::name)
        .def_property_readonly("aName", &Atom::name)
        .def_property_readonly("rId", [](Atom& a){return a.residue().id();})
        .def_property_readonly("rName", [](Atom& a){return a.residue().name();})
        .def_property_readonly("cIndex", [](Atom& a){return a.residue().chain().index();})
        .def_property_readonly("cName", [](Atom& a){return a.residue().chain().name();})
        .def_property_readonly("fIndex", [](Atom& a){return a.residue().chain().frame().index();})
        .def("__repr__", [](const Atom& atom) {
          return "<pyxmolpp2.pdb.Atom id=" + std::to_string(atom.id()) +
                 " name=\"" + atom.name().str() + "\" at 0x" +
                 xmol::utils::string::int2hex(
                     (uint64_t)(std::addressof(atom))) +
                 ">";
        });
    ;

    py::class_<Residue>(polymer, "Residue")
        .def_property_readonly(
            "chain", static_cast<Chain& (Residue::*)()>(&Residue::chain),
            py::return_value_policy::reference)
        .def_property_readonly(
            "frame", [](Residue& r){return r.chain().frame();},
            py::return_value_policy::reference)
        .def_property_readonly("id", &Residue::id)
        .def_property_readonly("rId", &Residue::id)
        .def_property_readonly("name", &Residue::name)
        .def_property_readonly("rName", &Residue::name)
        .def_property_readonly("cIndex", [](Residue& r){return r.chain().index();})
        .def_property_readonly("cName", [](Residue& r){return r.chain().name();})
        .def_property_readonly("fIndex", [](Residue& r){return r.chain().frame().index();})
        .def_property_readonly(
            "asAtoms", [](Residue& residue) { return residue.asAtoms(); })
        .def("__repr__", [](const Residue& residue) {
          return "<pyxmolpp2.pdb.Residue id=" +
                 std::to_string(residue.id()) + " name=\"" +
                 residue.name().str() + "\" at 0x" +
                 xmol::utils::string::int2hex(
                     (uint64_t)(std::addressof(residue))) +
                 ">";
        });
    ;

    py::class_<Chain>(polymer, "Chain")
        .def_property_readonly(
            "frame", static_cast<Frame& (Chain::*)()>(&Chain::frame),
            py::return_value_policy::reference)
        .def_property_readonly("index", &Chain::index)
        .def_property_readonly("cIndex", &Chain::index)
        .def_property_readonly("name", &Chain::name)
        .def_property_readonly("cName", &Chain::name)
        .def_property_readonly("fIndex", [](Chain& c){return c.frame().index();})
        .def_property_readonly("asResidues",
                               [](Chain& chain) { return chain.asResidues(); })
        .def_property_readonly("asAtoms",
                               [](Chain& chain) { return chain.asAtoms(); })
        .def("__repr__", [](const Chain& chain) {
          return "<pyxmolpp2.pdb.Chain index=" +
                 std::to_string(chain.index()) + " name=\"" +
                 chain.name().str() + "\" at 0x" +
                 xmol::utils::string::int2hex(
                     (uint64_t)(std::addressof(chain))) +
                 ">";
        });
    ;

    py::class_<Frame>(polymer, "Frame")
        .def_property_readonly("index", &Frame::index)
        .def_property_readonly("asChains",
                               [](Frame& frame) { return frame.asChains(); })
        .def_property_readonly("asResidues",
                               [](Frame& frame) { return frame.asResidues(); })
        .def_property_readonly("asAtoms",
                               [](Frame& frame) { return frame.asAtoms(); })
        .def("__repr__", [](const Frame& frame) {
          return "<pyxmolpp2.pdb.Frame index=" +
                 std::to_string(frame.index()) + " at 0x" +
                 xmol::utils::string::int2hex(
                     (uint64_t)(std::addressof(frame))) +
                 ">";
        });
    ;

    py::class_<AtomSelection>(polymer, "AtomSelection")
        .def("__len__", &AtomSelection::size)
        .def_property_readonly(
            "asChains", [](AtomSelection& aSel) { return aSel.asChains(); })
        .def_property_readonly(
            "asResidues",
            [](AtomSelection& aSel) { return aSel.asResidues(); })
        .def("__iter__",
             [](AtomSelection& sel) {
               return py::make_iterator(sel.begin(), sel.end());
             },
             py::keep_alive<0, 1>())
        .def("__repr__", [](const AtomSelection& selection) {
          return "<pyxmolpp2.pdb.AtomSelection size=" +
                 std::to_string(selection.size()) + " at 0x" +
                 xmol::utils::string::int2hex(
                     (uint64_t)(std::addressof(selection))) +
                 ">";
        });

    py::class_<ResidueSelection>(polymer, "ResidueSelection")
        .def("__len__", &ResidueSelection::size)
        .def_property_readonly(
            "asChains", [](ResidueSelection& rSel) { return rSel.asChains(); })
        .def_property_readonly(
            "asAtoms", [](ResidueSelection& rSel) { return rSel.asAtoms(); })
        .def("__iter__",
             [](ResidueSelection& sel) {
               return py::make_iterator(sel.begin(), sel.end());
             },
             py::keep_alive<0, 1>())
        .def("__repr__", [](const ResidueSelection& selection) {
          return "<pyxmolpp2.pdb.ResidueSelection size=" +
                 std::to_string(selection.size()) + " at 0x" +
                 xmol::utils::string::int2hex(
                     (uint64_t)(std::addressof(selection))) +
                 ">";
        });

    py::class_<ChainSelection>(polymer, "ChainSelection")
        .def("__len__", &ChainSelection::size)
        .def_property_readonly(
            "asResidues",
            [](ChainSelection& cSel) { return cSel.asResidues(); })
        .def_property_readonly(
            "asAtoms", [](ChainSelection& cSel) { return cSel.asAtoms(); })
        .def("__iter__",
             [](ChainSelection& sel) {
               return py::make_iterator(sel.begin(), sel.end());
             },
             py::keep_alive<0, 1>())
        .def("__repr__", [](const ChainSelection& selection) {
          return "<pyxmolpp2.pdb.ChainSelection size=" +
                 std::to_string(selection.size()) + " at 0x" +
                 xmol::utils::string::int2hex(
                     (uint64_t)(std::addressof(selection))) +
                 ">";
        });

//    pdb.def("load_first", [](py::str filename) {
//      return xmol::pdb::D;
//    });
//
//    pdb.def("load", [](py::str filename) {
//      py::list list;
//      for (Frame& x : entry::PDBEntry::readAllModels(filename)) {
//        list.append(std::move(x));
//      }
//      return list;
//    });
  }
}
