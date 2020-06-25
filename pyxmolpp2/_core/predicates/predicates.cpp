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

  auto&& pyAtomPredicate = py::class_<AtomPredicate>(polymer, "AtomPredicate");
  auto&& pyResiduePredicate = py::class_<ResiduePredicate>(polymer, "ResiduePredicate");
  auto&& pyMoleculePredicate = py::class_<MoleculePredicate>(polymer, "MoleculePredicate");

  auto&& pyAtomNamePredicateGenerator = py::class_<AtomNamePredicateGenerator>(polymer, "AtomNamePredicateGenerator");
  auto&& pyResidueNamePredicateGenerator = py::class_<ResidueNamePredicateGenerator>(polymer, "ResidueNamePredicateGenerator");
  auto&& pyMoleculeNamePredicateGenerator = py::class_<MoleculeNamePredicateGenerator>(polymer, "MoleculeNamePredicateGenerator");

  auto&& pyAtomIdPredicateGenerator = py::class_<AtomIdPredicateGenerator>(polymer, "AtomIdPredicateGenerator");
  auto&& pyResidueIdPredicateGenerator = py::class_<ResidueIdPredicateGenerator>(polymer, "ResidueIdPredicateGenerator");
//  auto&& pyMoleculeIndexPredicateGenerator = py::class_<MoleculeIndexPredicateGenerator>(polymer, "MoleculeIndexPredicateGenerator");

  pyAtomPredicate.def(py::init([](std::function<bool(AtomSmartRef)>& predicate) {
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

  // here we do not expose (const char*) overloads since it would be unreacheable from python side
  pyAtomNamePredicateGenerator
    .def(py::self==std::string{})
    .def(py::self!=std::string{})
    .def("is_in",overload_cast<AtomPredicate,AtomNamePredicateGenerator,std::set<std::string>>(&AtomNamePredicateGenerator::is_in))
  ;

  pyResidueNamePredicateGenerator
    .def(py::self==std::string{})
    .def(py::self!=std::string{})
    .def("is_in",overload_cast<ResiduePredicate,ResidueNamePredicateGenerator,std::set<std::string>>(&ResidueNamePredicateGenerator::is_in))
  ;

  pyMoleculeNamePredicateGenerator
    .def(py::self==std::string{})
    .def(py::self!=std::string{})
    .def("is_in",overload_cast<MoleculePredicate,MoleculeNamePredicateGenerator,std::set<std::string>>(&MoleculeNamePredicateGenerator::is_in))
  ;

  pyAtomIdPredicateGenerator
    .def(py::self==AtomId{})
    .def(py::self!=AtomId{})
    .def(py::self<AtomId{})
    .def(py::self>AtomId{})
    .def(py::self<=AtomId{})
    .def(py::self>=AtomId{})
    .def("is_in",&AtomIdPredicateGenerator::is_in)
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
    ;

  polymer.attr("aName") = aName;
  polymer.attr("rName") = rName;
  polymer.attr("mName") = mName;

  polymer.attr("aId") = aId;
  polymer.attr("rId") = rId;

}