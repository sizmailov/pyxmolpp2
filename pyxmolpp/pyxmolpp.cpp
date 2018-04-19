#include "pyxmolpp.h"

#include "xmol/polymer/Atom.h"
#include "xmol/utils/string.h"

namespace py = pybind11;

PYBIND11_MODULE(pyxmolpp2, m) {
  m.doc() = "pyxmolpp2 module";


  {
    pybind11::module geometry = m.def_submodule("geometry", "pyxmolpp2.geometry module");
    pyxmolpp::init_geometry(geometry);
  }

  {
    pybind11::module utils = m.def_submodule("utils", "pyxmolpp2.utils module");
    using namespace xmol::utils;
  }

  {
    pybind11::module polymer =
        m.def_submodule("polymer", "pyxmolpp2.polymer module");
    using namespace xmol::polymer;
//    py::class_<XYZ>(polymer, "XYZ")
//        .def(py::init<double, double, double>())
//        .def_property("x", &XYZ::x, py::cpp_function(&XYZ::set_x,py::return_value_policy::reference))
//        .def_property("y", &XYZ::y, py::cpp_function(&XYZ::set_y,py::return_value_policy::reference))
//        .def_property("z", &XYZ::z, py::cpp_function(&XYZ::set_z,py::return_value_policy::reference))
//        .def("__str__",
//             [](const XYZ& r) {
//               return "[" + std::to_string(r.x()) + ", " + std::to_string(r.y()) +
//                      ", " + std::to_string(r.z()) + "]";
//             })
//        .def("__repr__", [](const XYZ& r) {
//          return "<pyxmolpp2.polymer.XYZ \" [" + std::to_string(r.x()) + ", " +
//                 std::to_string(r.y()) + ", " + std::to_string(r.z()) +
//                 "] \" at 0x" +
//                 xmol::utils::string::int2hex((uint64_t)(std::addressof(r))) +
//                 ">";
//        });
    py::class_<AtomName>(polymer, "AtomName")
        .def(py::init<const std::string&>())
        .def_property_readonly("str", &AtomName::str)
        .def_property_readonly("value", &AtomName::value)
        .def("__str__",
             [](const AtomName& name) { return '"' + name.str() + '"'; })
        .def("__repr__", [](const AtomName& name) {
          return "<pyxmolpp2.polymer.AtomName \"" + name.str() + "\" at 0x" +
                 xmol::utils::string::int2hex(
                     (uint64_t)(std::addressof(name))) +
                 ">";
        });

    py::class_<ResidueName>(polymer, "ResidueName")
        .def(py::init<const std::string&>())
        .def_property_readonly("str", &ResidueName::str)
        .def_property_readonly("value", &ResidueName::value)
        .def("__str__",
             [](const ResidueName& name) { return '"' + name.str() + '"'; })
        .def("__repr__", [](const ResidueName& name) {
          return "<pyxmolpp2.polymer.ResidueName \"" + name.str() + "\" at 0x" +
                 xmol::utils::string::int2hex(
                     (uint64_t)(std::addressof(name))) +
                 ">";
        });

    py::class_<ChainName>(polymer, "ChainName")
        .def(py::init<const std::string&>())
        .def_property_readonly("str", &ChainName::str)
        .def_property_readonly("value", &ChainName::value)
        .def("__str__",
             [](const ChainName& name) { return '"' + name.str() + '"'; })
        .def("__repr__", [](const ChainName& name) {
          return "<pyxmolpp2.polymer.ChainName \"" + name.str() + "\" at 0x" +
                 xmol::utils::string::int2hex(
                     (uint64_t)(std::addressof(name))) +
                 ">";
        });

    py::class_<Atom>(polymer, "Atom")
        .def_property_readonly(
            "residue", static_cast<Residue& (Atom::*)()>(&Atom::residue),
            py::return_value_policy::reference)
        .def_property("r", &Atom::r, &Atom::set_r,
                      py::return_value_policy::reference_internal)
        .def_property_readonly("id", &Atom::id)
        .def_property_readonly("aId", &Atom::id)
        .def_property_readonly("name", &Atom::name)
        .def_property_readonly("aName", &Atom::name)
        .def_property_readonly("rId", [](Atom& a) { return a.residue().id(); })
        .def_property_readonly("rName",
                               [](Atom& a) { return a.residue().name(); })
        .def_property_readonly(
            "cIndex", [](Atom& a) { return a.residue().chain().index(); })
        .def_property_readonly(
            "cName", [](Atom& a) { return a.residue().chain().name(); })
        .def_property_readonly(
            "fIndex",
            [](Atom& a) { return a.residue().chain().frame().index(); })
        .def_property_readonly(
            "chain", [](Atom& a) -> Chain& { return a.residue().chain(); },
            py::return_value_policy::reference)
        .def_property_readonly(
            "frame",
            [](Atom& a) -> Frame& { return a.residue().chain().frame(); },
            py::return_value_policy::reference)
        .def("__repr__", [](const Atom& atom) {
          return "<pyxmolpp2.polymer.Atom id=" + std::to_string(atom.id()) +
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
            "frame", [](Residue& r) -> Frame& { return r.chain().frame(); },
            py::return_value_policy::reference)
        .def_property_readonly("id", &Residue::id)
        .def_property_readonly("rId", &Residue::id)
        .def_property_readonly("name", &Residue::name)
        .def_property_readonly("rName", &Residue::name)
        .def_property_readonly("cIndex",
                               [](Residue& r) { return r.chain().index(); })
        .def_property_readonly("cName",
                               [](Residue& r) { return r.chain().name(); })
        .def_property_readonly(
            "fIndex", [](Residue& r) { return r.chain().frame().index(); })
        .def_property_readonly(
            "asAtoms", [](Residue& residue) { return residue.asAtoms(); })
        .def("emplace", &Residue::emplace, py::return_value_policy::reference,
             py::arg("name"), py::arg("id"), py::arg("r"))
        .def("__repr__", [](const Residue& residue) {
          return "<pyxmolpp2.polymer.Residue id=" +
                 std::to_string(residue.id()) + " name=\"" +
                 residue.name().str() + "\" at 0x" +
                 xmol::utils::string::int2hex(
                     (uint64_t)(std::addressof(residue))) +
                 ">";
        });
    ;

    py::class_<Chain>(polymer, "Chain")
        .def_property_readonly("frame",
                               static_cast<Frame& (Chain::*)()>(&Chain::frame),
                               py::return_value_policy::reference)
        .def_property_readonly("index", &Chain::index)
        .def_property_readonly("cIndex", &Chain::index)
        .def_property_readonly("name", &Chain::name)
        .def_property_readonly("cName", &Chain::name)
        .def_property_readonly("fIndex",
                               [](Chain& c) { return c.frame().index(); })
        .def_property_readonly("asResidues",
                               [](Chain& chain) { return chain.asResidues(); })
        .def_property_readonly("asAtoms",
                               [](Chain& chain) { return chain.asAtoms(); })
        .def("emplace", &Chain::emplace, py::return_value_policy::reference,
             py::arg("name"), py::arg("id"), py::arg("reserve") = 0)
        .def("__repr__", [](const Chain& chain) {
          return "<pyxmolpp2.polymer.Chain index=" +
                 std::to_string(chain.index()) + " name=\"" +
                 chain.name().str() + "\" at 0x" +
                 xmol::utils::string::int2hex(
                     (uint64_t)(std::addressof(chain))) +
                 ">";
        });
    ;

    py::class_<Frame>(polymer, "Frame")
        .def(py::init<frameIndex_t>())
        .def_property_readonly("index", &Frame::index)
        .def_property_readonly("asChains",
                               [](Frame& frame) { return frame.asChains(); })
        .def_property_readonly("asResidues",
                               [](Frame& frame) { return frame.asResidues(); })
        .def_property_readonly("asAtoms",
                               [](Frame& frame) { return frame.asAtoms(); })
        .def("emplace", &Frame::emplace, py::return_value_policy::reference,
             py::arg("index"), py::arg("reserve") = 0)
        .def("__repr__", [](const Frame& frame) {
          return "<pyxmolpp2.polymer.Frame index=" +
                 std::to_string(frame.index()) + " at 0x" +
                 xmol::utils::string::int2hex(
                     (uint64_t)(std::addressof(frame))) +
                 ">";
        });
    ;

    py::class_<AtomSelection>(polymer, "AtomSelection")
        .def("__len__", [](AtomSelection& asel) { return asel.size(); })
        .def_property_readonly("size",
                               [](AtomSelection& asel) { return asel.size(); })
        .def_property_readonly(
            "asChains", [](AtomSelection& aSel) { return aSel.asChains(); })
        .def_property_readonly(
            "asResidues", [](AtomSelection& aSel) { return aSel.asResidues(); })
        .def("__iter__",
             [](AtomSelection& sel) {
               return py::make_iterator(sel.begin(), sel.end());
             },
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

    py::class_<ResidueSelection>(polymer, "ResidueSelection")
        .def("__len__", &ResidueSelection::size)
        .def_property_readonly("size", &AtomSelection::size)
        .def_property_readonly(
            "asChains", [](ResidueSelection& rSel) { return rSel.asChains(); })
        .def_property_readonly(
            "asAtoms", [](ResidueSelection& rSel) { return rSel.asAtoms(); })
        .def("__iter__",
             [](ResidueSelection& sel) {
               return py::make_iterator(sel.begin(), sel.end());
             },
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

    py::class_<ChainSelection>(polymer, "ChainSelection")
        .def("__len__", &ChainSelection::size)
        .def_property_readonly("size", &AtomSelection::size)
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
