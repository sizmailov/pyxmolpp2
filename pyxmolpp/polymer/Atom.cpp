#include "init.h"

#include "../geometry/init.h"

#include "pybind11/functional.h"
#include "pybind11/operators.h"
#include "pybind11/stl.h"
#include "pybind11/eigen.h"

#include "xmol/pdb/PdbRecord.h"
#include "xmol/geometry/Transformation3d.h"
#include "xmol/geometry/alignment.h"
#include "xmol/polymer/Atom.h"
#include "xmol/utils/string.h"

void pyxmolpp::polymer::init_Atom(pybind11::module& polymer) {

  using namespace xmol::polymer;
  namespace py = pybind11;

  auto&& pyAtomName = py::class_<AtomName>(polymer, "AtomName");
  auto&& pyResidueName = py::class_<ResidueName>(polymer, "ResidueName");
  auto&& pyChainName = py::class_<ChainName>(polymer, "ChainName");

  using AtomType = ElementReference<Atom>;
  using ResidueType = ElementReference<Residue>;
  using ChainType = ElementReference<Chain>;
//  using FrameType = Frame;

  using FrameRef = Frame&;

  constexpr py::return_value_policy AtomRefPolicy = py::return_value_policy::reference;
  constexpr py::return_value_policy ResidueRefPolicy = py::return_value_policy::reference;
  constexpr py::return_value_policy ChainRefPolicy = py::return_value_policy::reference;
  constexpr py::return_value_policy FrameRefPolicy = py::return_value_policy::reference;

  auto&& pyAtom = py::class_<AtomType>(polymer, "Atom");
  auto&& pyResidue = py::class_<ResidueType>(polymer, "Residue");
  auto&& pyChain = py::class_<ChainType>(polymer, "Chain");
  auto&& pyFrame = py::class_<Frame>(polymer, "Frame");

  auto&& pyAtomSelection = py::class_<AtomSelection>(polymer, "AtomSelection", R"pydoc(
Ordered list of atoms.

Order of atoms within residue match their construction order.
Atoms from different residues are ordered as parent residues, see :py:class:`ResidueSelection` for details.

Order is preserved across manipulations with :py:class:`AtomSelection`

)pydoc");
  auto&& pyResidueSelection = py::class_<ResidueSelection>(polymer, "ResidueSelection", R"pydoc(
Ordered list of residues.

Order of residues within chain match their construction order.
Residues from different chains are ordered as parent chains, see :py:class:`ChainSelection` for details.

Order is preserved across manipulations with :py:class:`ResidueSelection`

)pydoc");
  auto&& pyChainSelection = py::class_<ChainSelection>(polymer, "ChainSelection", R"pydoc(
Ordered list of chains.

Order of chains within frame matches their construction order.
Chains from different frames are ordered as parent frames.

Frames are ordered by their :py:attr:`~Frame.index`.

.. warning: If two frames have identical index, order is undefined
    Generally it's a *bad* idea to have :py:class:`Atom`/:py:class:`Residue`/:py:class:`Chain` selection with elements from two frames with same index.

Order is preserved across manipulations with :py:class:`ChainSelection`

)pydoc");

  auto&& pyAtomSelectionRange = py::class_<xmol::selection::SelectionRange<Atom>>(polymer, "AtomSelectionRange");
  auto&& pyResidueSelectionRange =
      py::class_<xmol::selection::SelectionRange<Residue>>(polymer, "ResidueSelectionRange");
  auto&& pyChainSelectionRange = py::class_<xmol::selection::SelectionRange<Chain>>(polymer, "ChainSelectionRange");

  py::class_<ResidueInsertionCode>(polymer, "ResidueInsertionCode")
      .def(py::init<const std::string&>())
      .def_property_readonly("str", &ResidueInsertionCode::str)
      .def("__hash__", &ResidueInsertionCode::value)
      .def("__str__", [](ResidueInsertionCode& name) { return name.str(); })
      .def("__repr__", [](ResidueInsertionCode& name) {
        return "<pyxmolpp2.polymer.ResidueInsertionCode\"" + name.str() + "\" at 0x" +
               xmol::utils::string::int2hex((uint64_t)(std::addressof(name))) + ">";
      });

  py::class_<ResidueId>(polymer, "ResidueId")
      .def(py::init<>())
      .def(py::init<residueSerial_t>())
      .def(py::init<residueSerial_t, const ResidueInsertionCode&>())
      .def(py::self==py::self)
      .def(py::self!=py::self)
      .def(py::self<=py::self)
      .def(py::self>=py::self)
      .def(py::self<py::self)
      .def(py::self>py::self)
      .def(py::self==residueSerial_t{})
      .def(py::self!=residueSerial_t{})
      .def(py::self<=residueSerial_t{})
      .def(py::self>=residueSerial_t{})
      .def(py::self<residueSerial_t{})
      .def(py::self>residueSerial_t{})
      .def(residueSerial_t{}==py::self)
      .def(residueSerial_t{}!=py::self)
      .def(residueSerial_t{}<=py::self)
      .def(residueSerial_t{}>=py::self)
      .def(residueSerial_t{}<py::self)
      .def(residueSerial_t{}>py::self)
      .def_readwrite("serial", &ResidueId::serial)
      .def_readwrite("iCode", &ResidueId::iCode)
      .def("__str__", [](ResidueId& rid) { return std::to_string(rid.serial) + rid.iCode.str(); })
      .def("__hash__", [](ResidueId& rid) { return std::hash<ResidueId>()(rid); })
      .def("__repr__", [](ResidueId& rid) {
        return "<pyxmolpp2.polymer.ResidueId\"" + std::to_string(rid.serial) + rid.iCode.str() + "\" at 0x" +
               xmol::utils::string::int2hex((uint64_t)(std::addressof(rid))) + ">";
      });

  pyAtomName.def(py::init<const std::string&>(),py::arg("name"))
      .def_property_readonly("str", &AtomName::str)
      .def("__hash__", &AtomName::value)
      .def("__str__", [](AtomName& name) { return name.str(); })
      .def(py::self==py::self)
      .def(py::self!=py::self)
      .def("__repr__", [](AtomName& name) {
        return "<pyxmolpp2.polymer.AtomName \"" + name.str() + "\" at 0x" +
               xmol::utils::string::int2hex((uint64_t)(std::addressof(name))) + ">";
      });

  pyResidueName.def(py::init<const std::string&>(),py::arg("name"))
      .def_property_readonly("str", &ResidueName::str)
      .def("__hash__", &ResidueName::value)
      .def(py::self==py::self)
      .def(py::self!=py::self)
      .def("__str__", [](ResidueName& name) { return name.str(); })
      .def("__repr__", [](ResidueName& name) {
        return "<pyxmolpp2.polymer.ResidueName \"" + name.str() + "\" at 0x" +
               xmol::utils::string::int2hex((uint64_t)(std::addressof(name))) + ">";
      });

  pyChainName.def(py::init<const std::string&>(),py::arg("name"))
      .def_property_readonly("str", &ChainName::str)
      .def("__hash__", &ChainName::value)
      .def(py::self==py::self)
      .def(py::self!=py::self)
      .def("__str__", [](ChainName& name) { return  name.str() ; })
      .def("__repr__", [](ChainName& name) {
        return "<pyxmolpp2.polymer.ChainName \"" + name.str() + "\" at 0x" +
               xmol::utils::string::int2hex((uint64_t)(std::addressof(name))) + ">";
      });

  pyAtom
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

  pyResidue
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
  static_assert(AtomRefPolicy == ResidueRefPolicy, "");
  static_assert(AtomRefPolicy == ChainRefPolicy, "");
  pyChain
      .def_property_readonly(
          "frame",
          [](ChainRef& chain) -> FrameRef { return FrameRef(static_cast<Chain&>(chain).frame()); },
          FrameRefPolicy,
          "Parent frame")

      .def(
          "__len__",
          [](ChainRef& chain) { return static_cast<Chain&>(chain).size(); },
          "Returns number of residues in chain")

      .def(
          "__eq__",
          [](ChainRef& a, ChainRef& b) -> bool {
              return &static_cast<Chain&>(a) == &static_cast<Chain&>(b);
          })

      .def(
          "__getitem__",
          [](ChainRef& chain, ResidueId& rid) { return ResidueRef(static_cast<Chain&>(chain)[rid]); })

      .def(
          "__getitem__",
          [](ChainRef& chain, residueSerial_t& rid) {
            return ResidueRef(static_cast<Chain&>(chain)[ResidueId(rid)]);
          })

      .def_property_readonly(
          "size",
          [](ChainRef& chain) { return static_cast<Chain&>(chain).size(); },
          "Returns number of residues in chain")

      .def_property_readonly(
          "index",
          [](ChainRef& chain) { return static_cast<Chain&>(chain).index(); },
          "Chain index, starts from 0"
          )

      .def_property_readonly(
          "cIndex",
          [](ChainRef& chain) { return static_cast<Chain&>(chain).index(); },
          "Chain index, starts from 0"
          )

      .def_property(
          "name",
          [](ChainRef& chain) { return static_cast<Chain&>(chain).name(); },
          [](ChainRef& chain, ChainName value) { static_cast<Chain&>(chain).set_name(value); },
          "Chain name (chainID in PDB nomenclature)")

      .def_property("cName",
          [](ChainRef& chain) { return static_cast<Chain&>(chain).name(); },
          [](ChainRef& chain, ChainName value) { static_cast<Chain&>(chain).set_name(value); },
          "Chain name (chainID in PDB nomenclature)")

      .def_property_readonly(
          "fIndex",
          [](ChainRef& chain) { return static_cast<Chain&>(chain).frame().index(); },
          "Frame index"
          )

      .def_property_readonly(
          "asResidues",
          [](ChainRef& chain) { return static_cast<Chain&>(chain).asResidues(); },
          "Returns selection of child residues")

      .def_property_readonly(
          "asAtoms",
          [](ChainRef& chain) { return static_cast<Chain&>(chain).asAtoms(); },
          "Returns selection of child atoms")

      .def(
          "emplace",
           [](ChainRef& chain, ResidueName name, residueId_t id, int reserve) -> ResidueRef {
             return ResidueRef(static_cast<Chain&>(chain).emplace(name, id, reserve));
           },
           ResidueRefPolicy,
           py::arg("name"),
           py::arg("id"),
           py::arg("reserve") = 0,
           "Adds an empty Residue to the end of chain")

      .def("emplace",
           [](ChainRef& chain, ResidueRef& r) -> ResidueRef {
             return ResidueRef(static_cast<Chain&>(chain).emplace(static_cast<Residue&>(r)));
           },
           ResidueRefPolicy,
           py::arg("residue"),
           "Adds a copy of given Residue to the end of chain")

      .def("delete",
          [](ChainRef& chain) { static_cast<Chain&>(chain).set_deleted(); },
          "Marks chain as deleted. Further access to deleted chain is illegal")

      .def(
          "__repr__",
          [](ChainRef& chain) {
              return "<pyxmolpp2.polymer.Chain index="
              + std::to_string(static_cast<Chain&>(chain).index())
              + " name=\""
              + static_cast<Chain&>(chain).name().str()
              + "\" at 0x"
              + xmol::utils::string::int2hex((uint64_t)(std::addressof(static_cast<Chain&>(chain))))
              + ">";
      });
  ;

  pyFrame.def(py::init<frameIndex_t>(),py::arg("frame_index"))
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
      .def("to_pdb", [](Frame& frame, std::string& filename) { frame.to_pdb(filename); },
           py::arg("output_filename"), "Writes frame to file using standard PDB records")
      .def("to_pdb", [](Frame& frame, std::string& output_filename,
                        xmol::pdb::basic_PdbRecords& db) { frame.to_pdb(output_filename, db); },
           py::arg("output_filename"), py::arg("db"), "Writes frame to file using db as non-standard PDB records")
      .def("copy",[](Frame& frame) -> Frame{
        return frame;
      }, py::return_value_policy::move)
      .def("__repr__", [](Frame& frame) {
        return "<pyxmolpp2.polymer.Frame index=" + std::to_string(frame.index()) + " at 0x" +
               xmol::utils::string::int2hex((uint64_t)(std::addressof(frame))) + ">";
      });
  ;

  //   .def("__iter__", [](xmol::selection::SelectionRange<Atom>& rng) -> sel& {
  //   return rng; })
  pyAtomSelectionRange.def("__next__",
                           [](xmol::selection::SelectionRange<Atom>& rng) -> AtomRef {
                             if (rng != rng) {
                               Atom& a = *rng;
                               ++rng;
                               return AtomRef(a);
                             }
                             throw py::stop_iteration();
                           },
                           AtomRefPolicy, py::keep_alive<0, 1>());

  pyResidueSelectionRange.def("__next__",
                              [](xmol::selection::SelectionRange<Residue>& rng) -> ResidueRef {
                                if (rng != rng) {
                                  Residue& a = *rng;
                                  ++rng;
                                  return ResidueRef(a);
                                }
                                throw py::stop_iteration();
                              },
                              ResidueRefPolicy, py::keep_alive<0, 1>());

  pyChainSelectionRange.def("__next__",
                            [](xmol::selection::SelectionRange<Chain>& rng) -> ChainRef {
                              if (rng != rng) {
                                Chain& a = *rng;
                                ++rng;
                                return ChainRef(a);
                              }
                              throw py::stop_iteration();
                            },
                            ChainRefPolicy, py::keep_alive<0, 1>());

  pyAtomSelection
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

  pyResidueSelection
      .def(py::init([](py::iterable residue_list){
            std::vector<Residue*> elements;
            auto iter = py::iter(residue_list);
            while (iter != py::iterator::sentinel())
            {
              ResidueRef ref = py::cast<ResidueRef>(*iter);
              elements.push_back(std::addressof(static_cast<Residue&>(ref)));
              ++iter;
            }
            return ResidueSelection(elements.begin(),elements.end());
          }),
          py::arg("residue_list"),
          "Construct ResidueSelection from iterable with Residue elements")

      .def(
          "filter",
          [](ResidueSelection& sel, std::function<bool(ResidueRef)>& predicate) {
            return sel.filter([&](const Residue& a) { return predicate(ResidueRef(const_cast<Residue&>(a))); });
          },
          py::arg("predicate"),
          "Keeps in selection only elements that match predicate")

      .def(
          "for_each",
          [](ResidueSelection& sel, std::function<void(ResidueRef)>& func) {
            return sel.for_each([&](Residue& a) { func(ResidueRef(a)); });
          },
          py::arg("transformation"), "Apply (mutating) transformation to selection")

      .def(
          "__len__",
          &ResidueSelection::size,
          "Returns number of elements")

      .def_property_readonly(
          "size",
          &ResidueSelection::size,
          "Returns number of elements")

      .def_property_readonly(
          "asChains",
          [](ResidueSelection& rSel) { return rSel.asChains(); },
          "Returns selection of parent chains")

      .def_property_readonly(
          "asAtoms",
          [](ResidueSelection& rSel) { return rSel.asAtoms(); },
          "Returns selection of child atoms")

      .def(
          "__iter__",
          [](ResidueSelection& sel) { return sel.begin(); },
          py::keep_alive<0, 1>())

      .def(
          "__getitem__",
          [](ResidueSelection& sel, int index) -> ResidueRef { return ResidueRef(sel[index]); },
          ResidueRefPolicy, py::arg("n"),
          "Access n'th element")

      .def(
          "__getitem__",
          [](ResidueSelection& sel, py::slice slice) {
            ssize_t start, stop, step, slicelength;
            if (!slice.compute(sel.size(),
                              reinterpret_cast<size_t*>(&start), reinterpret_cast<size_t*>(&stop),
                              reinterpret_cast<size_t*>(&step), reinterpret_cast<size_t*>(&slicelength)
                              )) {
                  throw py::error_already_set();
            }
            return sel.slice(start, stop, step, xmol::selection::SlicingScheme::USE_INDICES_AS_IS);
           },
           py::arg("slice"),
           "Slice overload")

      .def("__repr__",
           [](ResidueSelection& selection) {
             return "<pyxmolpp2.polymer.ResidueSelection size=" + std::to_string(selection.size()) + " at 0x" +
                    xmol::utils::string::int2hex((uint64_t)(std::addressof(selection))) + ">";
           })

      .def(py::self *= py::self)
      .def(py::self += py::self)
      .def(py::self -= py::self)
      .def(py::self != py::self)
      .def(py::self == py::self)
      .def(py::self * py::self)
      .def(py::self + py::self)
      .def(py::self - py::self);

  pyChainSelection
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

      .def(
          "__iter__",
          [](ChainSelection& sel) { return sel.begin(); }, py::keep_alive<0, 1>())

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
