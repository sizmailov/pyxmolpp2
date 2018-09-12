#include <xmol/geometry/AngleValue.h>
#include "init.h"

#include "pybind11/functional.h"
#include "pybind11/operators.h"
#include "pybind11/stl.h"

#include "xmol/polymer/predicate_generators.h"

namespace {

template<typename Ret, typename Left, typename Right>
using overload_cast = Ret (Left::*)(const Right&) const;

}


void pyxmolpp::polymer::init_predicates(pybind11::module& polymer) {

  using namespace xmol::polymer;
  namespace py = pybind11;

  auto&& pyAtomPredicate = py::class_<AtomPredicate>(polymer, "AtomPredicate");
  auto&& pyResiduePredicate = py::class_<ResiduePredicate>(polymer, "ResiduePredicate");
  auto&& pyChainPredicate = py::class_<ChainPredicate>(polymer, "ChainPredicate");

  auto&& pyAtomNamePredicateGenerator = py::class_<AtomNamePredicateGenerator>(polymer, "AtomNamePredicateGenerator");
  auto&& pyResidueNamePredicateGenerator = py::class_<ResidueNamePredicateGenerator>(polymer, "ResidueNamePredicateGenerator");
  auto&& pyChainNamePredicateGenerator = py::class_<ChainNamePredicateGenerator>(polymer, "ChainNamePredicateGenerator");

  auto&& pyAtomIdPredicateGenerator = py::class_<AtomIdPredicateGenerator>(polymer, "AtomIdPredicateGenerator");
  auto&& pyResidueIdPredicateGenerator = py::class_<ResidueIdPredicateGenerator>(polymer, "ResidueIdPredicateGenerator");
  auto&& pyChainIndexPredicateGenerator = py::class_<ChainIndexPredicateGenerator>(polymer, "ChainIndexPredicateGenerator");

  pyAtomPredicate.def(py::init([](std::function<bool(AtomRef)>& predicate) {
    return AtomPredicate([predicate](const Atom& a) { return predicate(AtomRef(const_cast<Atom&>(a))); });
  }))

  .def("__call__",[](AtomPredicate& pred, AtomRef& ref){ return pred(static_cast<Atom&>(ref));  })
  .def("__invert__",&AtomPredicate::operator!)

  .def("__and__",overload_cast<AtomPredicate,AtomPredicate,AtomPredicate>(&AtomPredicate::operator&&))
  .def("__xor__",overload_cast<AtomPredicate,AtomPredicate,AtomPredicate>(&AtomPredicate::operator^))
  .def("__or__",overload_cast<AtomPredicate,AtomPredicate,AtomPredicate>(&AtomPredicate::operator||))

  .def("__and__",overload_cast<AtomPredicate,AtomPredicate,ResiduePredicate>(&AtomPredicate::operator&&))
  .def("__xor__",overload_cast<AtomPredicate,AtomPredicate,ResiduePredicate>(&AtomPredicate::operator^))
  .def("__or__",overload_cast<AtomPredicate,AtomPredicate,ResiduePredicate>(&AtomPredicate::operator||))

  .def("__and__",overload_cast<AtomPredicate,AtomPredicate,ChainPredicate>(&AtomPredicate::operator&&))
  .def("__or__",overload_cast<AtomPredicate,AtomPredicate,ChainPredicate>(&AtomPredicate::operator^))
  .def("__ror__",overload_cast<AtomPredicate,AtomPredicate,ChainPredicate>(&AtomPredicate::operator||))
  ;

  pyResiduePredicate.def(py::init([](std::function<bool(ResidueRef)>& predicate) {
    return ResiduePredicate([predicate](const Residue& a) { return predicate(ResidueRef(const_cast<Residue&>(a))); });
  }))

  .def("__call__",[](ResiduePredicate& pred, ResidueRef& ref){ return pred(static_cast<Residue&>(ref));  })
  .def("__call__",[](ResiduePredicate& pred, AtomRef& ref){ return pred(static_cast<Atom&>(ref));  })

  .def("__invert__",&ResiduePredicate::operator!)

  .def("__and__",overload_cast<AtomPredicate,ResiduePredicate,AtomPredicate>(&ResiduePredicate::operator&&))
  .def("__xor__",overload_cast<AtomPredicate,ResiduePredicate,AtomPredicate>(&ResiduePredicate::operator^))
  .def("__or__",overload_cast<AtomPredicate,ResiduePredicate,AtomPredicate>(&ResiduePredicate::operator||))

  .def("__and__",overload_cast<ResiduePredicate,ResiduePredicate,ResiduePredicate>(&ResiduePredicate::operator&&))
  .def("__xor__",overload_cast<ResiduePredicate,ResiduePredicate,ResiduePredicate>(&ResiduePredicate::operator^))
  .def("__or__",overload_cast<ResiduePredicate,ResiduePredicate,ResiduePredicate>(&ResiduePredicate::operator||))

  .def("__and__",overload_cast<ResiduePredicate,ResiduePredicate,ChainPredicate>(&ResiduePredicate::operator&&))
  .def("__xor__",overload_cast<ResiduePredicate,ResiduePredicate,ChainPredicate>(&ResiduePredicate::operator^))
  .def("__or__",overload_cast<ResiduePredicate,ResiduePredicate,ChainPredicate>(&ResiduePredicate::operator||))
  ;


  pyChainPredicate.def(py::init([](std::function<bool(ChainRef)>& predicate) {
    return ChainPredicate([predicate](const Chain& a) { return predicate(ChainRef(const_cast<Chain&>(a))); });
  }))

  .def("__call__",[](ChainPredicate& pred, ChainRef& ref){ return pred(static_cast<Chain&>(ref));  })
  .def("__call__",[](ChainPredicate& pred, ResidueRef& ref){ return pred(static_cast<Residue&>(ref));  })
  .def("__call__",[](ChainPredicate& pred, AtomRef& ref){ return pred(static_cast<Atom&>(ref));  })

  .def("__invert__",&ChainPredicate::operator!)

  .def("__and__",overload_cast<AtomPredicate,ChainPredicate,AtomPredicate>(&ChainPredicate::operator&&))
  .def("__xor__",overload_cast<AtomPredicate,ChainPredicate,AtomPredicate>(&ChainPredicate::operator^))
  .def("__or__",overload_cast<AtomPredicate,ChainPredicate,AtomPredicate>(&ChainPredicate::operator||))

  .def("__and__",overload_cast<ResiduePredicate,ChainPredicate,ResiduePredicate>(&ChainPredicate::operator&&))
  .def("__xor__",overload_cast<ResiduePredicate,ChainPredicate,ResiduePredicate>(&ChainPredicate::operator^))
  .def("__or__",overload_cast<ResiduePredicate,ChainPredicate,ResiduePredicate>(&ChainPredicate::operator||))

  .def("__and__",overload_cast<ChainPredicate,ChainPredicate,ChainPredicate>(&ChainPredicate::operator&&))
  .def("__xor__",overload_cast<ChainPredicate,ChainPredicate,ChainPredicate>(&ChainPredicate::operator^))
  .def("__or__",overload_cast<ChainPredicate,ChainPredicate,ChainPredicate>(&ChainPredicate::operator||))
  ;

  // here we do not expose (const char*) overloads since it would be unreacheable from python side
  pyAtomNamePredicateGenerator
    .def(py::self==AtomName{})
    .def(py::self==std::string{})
    .def(py::self!=AtomName{})
    .def(py::self!=std::string{})
    .def("is_in",overload_cast<AtomPredicate,AtomNamePredicateGenerator,std::set<AtomName>>(&AtomNamePredicateGenerator::is_in))
    .def("is_in",overload_cast<AtomPredicate,AtomNamePredicateGenerator,std::set<std::string>>(&AtomNamePredicateGenerator::is_in))
  ;

  pyResidueNamePredicateGenerator
    .def(py::self==ResidueName{})
    .def(py::self==std::string{})
    .def(py::self!=ResidueName{})
    .def(py::self!=std::string{})
    .def("is_in",overload_cast<ResiduePredicate,ResidueNamePredicateGenerator,std::set<ResidueName>>(&ResidueNamePredicateGenerator::is_in))
    .def("is_in",overload_cast<ResiduePredicate,ResidueNamePredicateGenerator,std::set<std::string>>(&ResidueNamePredicateGenerator::is_in))
  ;

  pyChainNamePredicateGenerator
    .def(py::self==ChainName{})
    .def(py::self==std::string{})
    .def(py::self!=ChainName{})
    .def(py::self!=std::string{})
    .def("is_in",overload_cast<ChainPredicate,ChainNamePredicateGenerator,std::set<ChainName>>(&ChainNamePredicateGenerator::is_in))
    .def("is_in",overload_cast<ChainPredicate,ChainNamePredicateGenerator,std::set<std::string>>(&ChainNamePredicateGenerator::is_in))
  ;

  pyAtomIdPredicateGenerator
    .def(py::self==atomId_t{})
    .def(py::self!=atomId_t{})
    .def(py::self<atomId_t{})
    .def(py::self>atomId_t{})
    .def(py::self<=atomId_t{})
    .def(py::self>=atomId_t{})
    .def("is_in",&AtomIdPredicateGenerator::is_in)
    ;

  pyResidueIdPredicateGenerator
    .def(py::self==residueId_t{})
    .def(py::self!=residueId_t{})
    .def(py::self<residueId_t{})
    .def(py::self>residueId_t{})
    .def(py::self<=residueId_t{})
    .def(py::self>=residueId_t{})
    .def(py::self==residueSerial_t{})
    .def(py::self!=residueSerial_t{})
    .def(py::self<residueSerial_t{})
    .def(py::self>residueSerial_t{})
    .def(py::self<=residueSerial_t{})
    .def(py::self>=residueSerial_t{})
    .def("is_in",overload_cast<ResiduePredicate,ResidueIdPredicateGenerator,std::set<residueSerial_t>>(&ResidueIdPredicateGenerator::is_in))
    .def("is_in",overload_cast<ResiduePredicate,ResidueIdPredicateGenerator,std::set<residueId_t>>(&ResidueIdPredicateGenerator::is_in))
    ;

  pyChainIndexPredicateGenerator
      .def(py::self==chainIndex_t{})
      .def(py::self!=chainIndex_t{})
      .def(py::self<chainIndex_t{})
      .def(py::self>chainIndex_t{})
      .def(py::self<=chainIndex_t{})
      .def(py::self>=chainIndex_t{})
      .def("is_in",&ChainIndexPredicateGenerator::is_in)
      ;


  polymer.attr("aName") = aName;
  polymer.attr("rName") = rName;
  polymer.attr("cName") = cName;

  polymer.attr("aId") = aId;
  polymer.attr("rId") = rId;
  polymer.attr("cIndex") = cIndex;

}