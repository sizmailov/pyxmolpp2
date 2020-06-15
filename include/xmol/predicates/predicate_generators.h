#pragma once

#include "predicates.h"
#include <type_traits>

namespace xmol::predicates {

class AtomNamePredicateGenerator {
public:
  constexpr AtomNamePredicateGenerator() = default;

  AtomPredicate operator==(const AtomName& name) const {
    return AtomPredicate([name](const AtomRef& a) { return a.name() == name; });
  }

  AtomPredicate operator==(const char* char_name) const {
    auto name = AtomName(char_name);
    return AtomPredicate([name](const AtomRef& a){ return a.name() == name;});
  }

  AtomPredicate operator==(const std::string& string_name) const {
    auto name = AtomName(string_name);
    return AtomPredicate([name](const AtomRef& a){ return a.name() == name;});
  }

  AtomPredicate operator!=(const AtomName& name) const {
    return AtomPredicate([name](const AtomRef& a){ return a.name() != name;});
  }

  AtomPredicate operator!=(const char* char_name) const {
    auto name = AtomName(char_name);
    return AtomPredicate([name](const AtomRef& a){ return a.name() != name;});
  }

  AtomPredicate operator!=(const std::string& string_name) const {
    auto name = AtomName(string_name);
    return AtomPredicate([name](const AtomRef& a){ return a.name() != name;});
  }

  AtomPredicate is_in(const std::set<AtomName>& names) const {
    return AtomPredicate([names](const AtomRef& a){ return names.count(a.name())==1 ;});
  }

  AtomPredicate is_in(const std::set<const char*>& char_names) const {
    std::set<AtomName> names;
    for (auto& name: char_names){
      names.insert(AtomName(name));
    }
    return AtomPredicate([names](const AtomRef& a){ return names.count(a.name())==1 ;});
  }

  AtomPredicate is_in(const std::set<std::string>& char_names) const {
    std::set<AtomName> names;
    for (auto& name: char_names){
      names.insert(AtomName(name));
    }
    return AtomPredicate([names](const AtomRef& a){ return names.count(a.name())==1 ;});
  }
};


class ResidueNamePredicateGenerator{
public:
  constexpr ResidueNamePredicateGenerator()=default;

  ResiduePredicate operator==(const ResidueName& name) const {
    return ResiduePredicate([name](const ResidueRef& a){ return a.name() == name;});
  }

  ResiduePredicate operator==(const char* char_name) const {
    auto name = ResidueName(char_name);
    return ResiduePredicate([name](const ResidueRef& a){ return a.name() == name;});
  }

  ResiduePredicate operator==(const std::string& string_name) const {
    auto name = ResidueName(string_name);
    return ResiduePredicate([name](const ResidueRef& a){ return a.name() == name;});
  }


  ResiduePredicate operator!=(const ResidueName& name) const {
    return ResiduePredicate([name](const ResidueRef& a){ return a.name() != name;});
  }

  ResiduePredicate operator!=(const char* char_name) const {
    auto name = ResidueName(char_name);
    return ResiduePredicate([name](const ResidueRef& a){ return a.name() != name;});
  }

  ResiduePredicate operator!=(const std::string& string_name) const {
    auto name = ResidueName(string_name);
    return ResiduePredicate([name](const ResidueRef& a){ return a.name() != name;});
  }

  ResiduePredicate is_in(const std::set<ResidueName>& names) const {
    return ResiduePredicate([names](const ResidueRef& r){ return names.count(r.name())==1 ;});
  }

  ResiduePredicate is_in(const std::set<const char*>& char_names) const {
    std::set<ResidueName> names;
    for (auto& name: char_names){
      names.insert(ResidueName(name));
    }
    return ResiduePredicate([names](const ResidueRef& r){ return names.count(r.name())==1 ;});
  }

  ResiduePredicate is_in(const std::set<std::string>& string_names) const {
    std::set<ResidueName> names;
    for (auto& name: string_names){
      names.insert(ResidueName(name));
    }
    return ResiduePredicate([names](const ResidueRef& r){ return names.count(r.name())==1 ;});
  }
};


class MoleculeNamePredicateGenerator{
public:
  constexpr MoleculeNamePredicateGenerator()=default;

  MoleculePredicate operator==(const MoleculeName& name) const {
    return MoleculePredicate([name](const MoleculeRef& a){ return a.name() == name;});
  }

  MoleculePredicate operator==(const char* char_name) const {
    auto name = MoleculeName(char_name);
    return MoleculePredicate([name](const MoleculeRef& a){ return a.name() == name;});
  }

  MoleculePredicate operator==(const std::string& string_name) const {
    auto name = MoleculeName(string_name);
    return MoleculePredicate([name](const MoleculeRef& a){ return a.name() == name;});
  }

  MoleculePredicate operator!=(const MoleculeName& name) const {
    return MoleculePredicate([name](const MoleculeRef& a){ return a.name() != name;});
  }

  MoleculePredicate operator!=(const char* char_name) const {
    auto name = MoleculeName(char_name);
    return MoleculePredicate([name](const MoleculeRef& a){ return a.name() != name;});
  }

  MoleculePredicate operator!=(const std::string& string_name) const {
    auto name = MoleculeName(string_name);
    return MoleculePredicate([name](const MoleculeRef& a){ return a.name() != name;});
  }

  MoleculePredicate is_in(const std::set<MoleculeName>& names) const {
    return MoleculePredicate([names](const MoleculeRef& a){ return names.count(a.name())==1 ;});
  }

  MoleculePredicate is_in(const std::set<const char*>& char_names) const {
    std::set<MoleculeName> names;
    for (auto& name: char_names){
      names.insert(MoleculeName(name));
    }
    return MoleculePredicate([names](const MoleculeRef& a){ return names.count(a.name())==1 ;});
  }

  MoleculePredicate is_in(const std::set<std::string>& string_names) const {
    std::set<MoleculeName> names;
    for (auto& name: string_names){
      names.insert(MoleculeName(name));
    }
    return MoleculePredicate([names](const MoleculeRef& a){ return names.count(a.name())==1 ;});
  }
};


class AtomIdPredicateGenerator{
public:
  constexpr AtomIdPredicateGenerator()=default;

  AtomPredicate operator==(const AtomId& id) const {
    return AtomPredicate([id](const AtomRef& a){ return a.id() == id;});
  }

  AtomPredicate operator!=(const AtomId& id) const {
    return AtomPredicate([id](const AtomRef& a){ return a.id() != id;});
  }

  AtomPredicate operator<=(const AtomId& id) const {
    return AtomPredicate([id](const AtomRef& a){ return a.id() <= id;});
  }

  AtomPredicate operator<(const AtomId& id) const {
    return AtomPredicate([id](const AtomRef& a){ return a.id() < id;});
  }

  AtomPredicate operator>=(const AtomId& id) const {
    return AtomPredicate([id](const AtomRef& a){ return a.id() >= id;});
  }

  AtomPredicate operator>(const AtomId& id) const {
    return AtomPredicate([id](const AtomRef& a){ return a.id() > id;});
  }

  AtomPredicate is_in(const std::set<AtomId>& ids) const {
    return AtomPredicate([ids](const AtomRef& a){ return ids.count(a.id())==1 ;});
  }

};


class ResidueIdPredicateGenerator{
public:
  constexpr ResidueIdPredicateGenerator()=default;

  ResiduePredicate operator==(const ResidueId& id) const {
    return ResiduePredicate([id](const ResidueRef& r){ return r.id() == id;});
  }

  ResiduePredicate operator!=(const ResidueId& id) const {
    return ResiduePredicate([id](const ResidueRef& r){ return r.id() != id;});
  }

  ResiduePredicate operator<=(const ResidueId& id) const {
    return ResiduePredicate([id](const ResidueRef& r){ return r.id() <= id;});
  }

  ResiduePredicate operator<(const ResidueId& id) const {
    return ResiduePredicate([id](const ResidueRef& r){ return r.id() < id;});
  }

  ResiduePredicate operator>=(const ResidueId& id) const {
    return ResiduePredicate([id](const ResidueRef& r){ return r.id() >= id;});
  }

  ResiduePredicate operator>(const ResidueId& id) const {
    return ResiduePredicate([id](const ResidueRef& r){ return r.id() > id;});
  }

  ResiduePredicate is_in(const std::set<ResidueId>& ids) const {
    return ResiduePredicate([ids](const ResidueRef& r){ return ids.count(r.id())==1 ;});
  }

  ResiduePredicate operator==(const residueSerial_t& id) const {
    return ResiduePredicate([id](const ResidueRef& r){ return r.id() == id;});
  }

  ResiduePredicate operator!=(const residueSerial_t& id) const {
    return ResiduePredicate([id](const ResidueRef& r){ return r.id() != id;});
  }

  ResiduePredicate operator<=(const residueSerial_t& id) const {
    return ResiduePredicate([id](const ResidueRef& r){ return r.id() <= id;});
  }

  ResiduePredicate operator<(const residueSerial_t& id) const {
    return ResiduePredicate([id](const ResidueRef& r){ return r.id() < id;});
  }

  ResiduePredicate operator>=(const residueSerial_t& id) const {
    return ResiduePredicate([id](const ResidueRef& r){ return r.id() >= id;});
  }

  ResiduePredicate operator>(const residueSerial_t& id) const {
    return ResiduePredicate([id](const ResidueRef& r){ return r.id() > id;});
  }

  ResiduePredicate is_in(const std::set<residueSerial_t>& ids) const {
    return ResiduePredicate([ids](const ResidueRef& r){ return r.id().iCode.value()==0 && ids.count(r.id().serial)==1 ;});
  }

};

[[maybe_unused]] constexpr auto aName = AtomNamePredicateGenerator{};
[[maybe_unused]] constexpr auto rName = ResidueNamePredicateGenerator{};
[[maybe_unused]] constexpr auto mName = MoleculeNamePredicateGenerator{};

[[maybe_unused]] constexpr auto aId = AtomIdPredicateGenerator{};
[[maybe_unused]] constexpr auto rId = ResidueIdPredicateGenerator{};

}