#include "predicates.h"

#include "pybind11/functional.h"
#include "pybind11/operators.h"
#include "pybind11/stl.h"

#include "xmol/proxy/smart/references.h"
#include "xmol/predicates/predicates.h"
#include "xmol/predicates/predicate_generators.h"

namespace {

template<typename Ret, typename Left, typename Right>
using overload_cast = Ret (Left::*)(const Right&) const;

}

void pyxmolpp::v1::init_predicates(pybind11::module& polymer) {

  using namespace xmol::predicates;
  using namespace xmol::proxy::smart;
  using namespace xmol;
  namespace py = pybind11;

  auto&& pyAtomPredicate = py::class_<AtomPredicate>(polymer, "AtomPredicate", "Atom Preidcate");
  auto&& pyResiduePredicate = py::class_<ResiduePredicate>(polymer, "ResiduePredicate", "Residue Predicate");
  auto&& pyMoleculePredicate = py::class_<MoleculePredicate>(polymer, "MoleculePredicate", "Molecule Predicate");

  auto&& pyAtomNamePredicateGenerator = py::class_<AtomNamePredicateGenerator>(polymer, "AtomNamePredicateGenerator");
  auto&& pyResidueNamePredicateGenerator =
      py::class_<ResidueNamePredicateGenerator>(polymer, "ResidueNamePredicateGenerator");
  auto&& pyMoleculeNamePredicateGenerator =
      py::class_<MoleculeNamePredicateGenerator>(polymer, "MoleculeNamePredicateGenerator");

  auto&& pyAtomIdPredicateGenerator = py::class_<AtomIdPredicateGenerator>(polymer, "AtomIdPredicateGenerator");
  auto&& pyResidueIdPredicateGenerator =
      py::class_<ResidueIdPredicateGenerator>(polymer, "ResidueIdPredicateGenerator");
  //  auto&& pyMoleculeIndexPredicateGenerator = py::class_<MoleculeIndexPredicateGenerator>(polymer,
  //  "MoleculeIndexPredicateGenerator");

  pyAtomPredicate
      .def(py::init([](std::function<bool(AtomSmartRef)>& predicate) {
        return AtomPredicate([predicate](const AtomSmartRef& a) { return predicate(AtomSmartRef(const_cast<AtomSmartRef&>(a))); });
  }))

  .def("__call__",[](AtomPredicate& pred, AtomSmartRef& ref){ return pred((ref));  })
  .def("__invert__",&AtomPredicate::operator!)

  .def("__and__",overload_cast<AtomPredicate,AtomPredicate,AtomPredicate>(&AtomPredicate::operator&&))
  .def("__xor__",overload_cast<AtomPredicate,AtomPredicate,AtomPredicate>(&AtomPredicate::operator^))
  .def("__or__",overload_cast<AtomPredicate,AtomPredicate,AtomPredicate>(&AtomPredicate::operator||))

  .def("__and__",overload_cast<AtomPredicate,AtomPredicate,ResiduePredicate>(&AtomPredicate::operator&&))
  .def("__xor__",overload_cast<AtomPredicate,AtomPredicate,ResiduePredicate>(&AtomPredicate::operator^))
  .def("__or__",overload_cast<AtomPredicate,AtomPredicate,ResiduePredicate>(&AtomPredicate::operator||))

  .def("__and__",overload_cast<AtomPredicate,AtomPredicate,MoleculePredicate>(&AtomPredicate::operator&&))
  .def("__or__",overload_cast<AtomPredicate,AtomPredicate,MoleculePredicate>(&AtomPredicate::operator^))
  .def("__ror__",overload_cast<AtomPredicate,AtomPredicate,MoleculePredicate>(&AtomPredicate::operator||))
  ;

  pyResiduePredicate.def(py::init([](std::function<bool(ResidueSmartRef)>& predicate) {
    return ResiduePredicate([predicate](const ResidueSmartRef& a) { return predicate(ResidueSmartRef(const_cast<ResidueSmartRef&>(a))); });
  }))

  .def("__call__",[](ResiduePredicate& pred, ResidueSmartRef& ref){ return pred(ref);  })
  .def("__call__",[](ResiduePredicate& pred, AtomSmartRef& ref){ return pred(ref);  })

  .def("__invert__",&ResiduePredicate::operator!)

  .def("__and__",overload_cast<AtomPredicate,ResiduePredicate,AtomPredicate>(&ResiduePredicate::operator&&))
  .def("__xor__",overload_cast<AtomPredicate,ResiduePredicate,AtomPredicate>(&ResiduePredicate::operator^))
  .def("__or__",overload_cast<AtomPredicate,ResiduePredicate,AtomPredicate>(&ResiduePredicate::operator||))

  .def("__and__",overload_cast<ResiduePredicate,ResiduePredicate,ResiduePredicate>(&ResiduePredicate::operator&&))
  .def("__xor__",overload_cast<ResiduePredicate,ResiduePredicate,ResiduePredicate>(&ResiduePredicate::operator^))
  .def("__or__",overload_cast<ResiduePredicate,ResiduePredicate,ResiduePredicate>(&ResiduePredicate::operator||))

  .def("__and__",overload_cast<ResiduePredicate,ResiduePredicate,MoleculePredicate>(&ResiduePredicate::operator&&))
  .def("__xor__",overload_cast<ResiduePredicate,ResiduePredicate,MoleculePredicate>(&ResiduePredicate::operator^))
  .def("__or__",overload_cast<ResiduePredicate,ResiduePredicate,MoleculePredicate>(&ResiduePredicate::operator||))
  ;


  pyMoleculePredicate.def(py::init([](std::function<bool(MoleculeSmartRef)>& predicate) {
    return MoleculePredicate([predicate](const MoleculeSmartRef& a) { return predicate(MoleculeSmartRef(const_cast<MoleculeSmartRef&>(a))); });
  }))

  .def("__call__",[](MoleculePredicate& pred, MoleculeSmartRef& ref){ return pred((ref));  })
  .def("__call__",[](MoleculePredicate& pred, ResidueSmartRef& ref){ return pred((ref));  })
  .def("__call__",[](MoleculePredicate& pred, AtomSmartRef& ref){ return pred((ref));  })

  .def("__invert__",&MoleculePredicate::operator!)

  .def("__and__",overload_cast<AtomPredicate,MoleculePredicate,AtomPredicate>(&MoleculePredicate::operator&&))
  .def("__xor__",overload_cast<AtomPredicate,MoleculePredicate,AtomPredicate>(&MoleculePredicate::operator^))
  .def("__or__",overload_cast<AtomPredicate,MoleculePredicate,AtomPredicate>(&MoleculePredicate::operator||))

  .def("__and__",overload_cast<ResiduePredicate,MoleculePredicate,ResiduePredicate>(&MoleculePredicate::operator&&))
  .def("__xor__",overload_cast<ResiduePredicate,MoleculePredicate,ResiduePredicate>(&MoleculePredicate::operator^))
  .def("__or__",overload_cast<ResiduePredicate,MoleculePredicate,ResiduePredicate>(&MoleculePredicate::operator||))

  .def("__and__",overload_cast<MoleculePredicate,MoleculePredicate,MoleculePredicate>(&MoleculePredicate::operator&&))
  .def("__xor__",overload_cast<MoleculePredicate,MoleculePredicate,MoleculePredicate>(&MoleculePredicate::operator^))
  .def("__or__",overload_cast<MoleculePredicate,MoleculePredicate,MoleculePredicate>(&MoleculePredicate::operator||))
  ;

  pyAtomNamePredicateGenerator
    .def(py::self==std::string{})
    .def(py::self!=std::string{})
    .def("is_in",overload_cast<AtomPredicate,AtomNamePredicateGenerator,std::set<std::string>>(&AtomNamePredicateGenerator::is_in))
    .def("is_in",[](AtomNamePredicateGenerator& g, py::args& names){
        std::set<AtomName> _names;
        for (auto name: names){
          _names.emplace(py::cast<std::string>(name));
        }
        return g.is_in(_names);
      })
  ;

  pyResidueNamePredicateGenerator
    .def(py::self==std::string{})
    .def(py::self!=std::string{})
    .def("is_in",overload_cast<ResiduePredicate,ResidueNamePredicateGenerator,std::set<std::string>>(&ResidueNamePredicateGenerator::is_in))
    .def("is_in",[](ResidueNamePredicateGenerator& g, py::args& names){
      std::set<ResidueName> _names;
      for (auto name: names){
        _names.emplace(py::cast<std::string>(name));
      }
      return g.is_in(_names);
    })
  ;

  pyMoleculeNamePredicateGenerator
    .def(py::self==std::string{})
    .def(py::self!=std::string{})
    .def("is_in",overload_cast<MoleculePredicate,MoleculeNamePredicateGenerator,std::set<std::string>>(&MoleculeNamePredicateGenerator::is_in))
    .def("is_in",[](MoleculeNamePredicateGenerator& g, py::args& names){
      std::set<MoleculeName> _names;
      for (auto name: names){
        _names.emplace(py::cast<std::string>(name));
      }
      return g.is_in(_names);
    })
  ;

  pyAtomIdPredicateGenerator
    .def(py::self==AtomId{})
    .def(py::self!=AtomId{})
    .def(py::self<AtomId{})
    .def(py::self>AtomId{})
    .def(py::self<=AtomId{})
    .def(py::self>=AtomId{})
    .def("is_in",&AtomIdPredicateGenerator::is_in)
    .def("is_in",[](AtomIdPredicateGenerator& g, py::args& ids){
      std::set<AtomId> _ids;
      for (auto id : ids){
        _ids.emplace(py::cast<AtomId>(id));
      }
      return g.is_in(_ids);
    })
    ;

  pyResidueIdPredicateGenerator
    .def(py::self==ResidueId{})
    .def(py::self!=ResidueId{})
    .def(py::self<ResidueId{})
    .def(py::self>ResidueId{})
    .def(py::self<=ResidueId{})
    .def(py::self>=ResidueId{})
    .def(py::self==residueSerial_t{})
    .def(py::self!=residueSerial_t{})
    .def(py::self<residueSerial_t{})
    .def(py::self>residueSerial_t{})
    .def(py::self<=residueSerial_t{})
    .def(py::self>=residueSerial_t{})
    .def("is_in",overload_cast<ResiduePredicate,ResidueIdPredicateGenerator,std::set<residueSerial_t>>(&ResidueIdPredicateGenerator::is_in))
    .def("is_in",overload_cast<ResiduePredicate,ResidueIdPredicateGenerator,std::set<ResidueId>>(&ResidueIdPredicateGenerator::is_in))
    .def("is_in",[](ResidueIdPredicateGenerator& g, py::args& ids){
      std::set<ResidueId> _ids;
      for (auto id : ids){
        if (py::isinstance<ResidueId>(id)){
          _ids.emplace(py::cast<ResidueId>(id));
        }else{
          _ids.emplace(py::cast<residueSerial_t>(id));
        }
      }
      return g.is_in(_ids);
    })
    ;

  polymer.attr("aName") = aName;
  polymer.attr("rName") = rName;
  polymer.attr("mName") = mName;

  polymer.attr("aId") = aId;
  polymer.attr("rId") = rId;

}