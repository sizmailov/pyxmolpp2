#pragma once

#include "xmol/polymer/predicates.h"
#include <type_traits>

namespace xmol{
namespace polymer {

class AtomNamePredicateGenerator{
public:
  constexpr AtomNamePredicateGenerator()=default;

  AtomPredicate operator==(const AtomName& name) const {
    return AtomPredicate([name](const Atom& a){ return a.name() == name;});
  }

  AtomPredicate operator==(const char* char_name) const {
    auto name = AtomName(char_name);
    return AtomPredicate([name](const Atom& a){ return a.name() == name;});
  }

  AtomPredicate operator==(const std::string& string_name) const {
    auto name = AtomName(string_name);
    return AtomPredicate([name](const Atom& a){ return a.name() == name;});
  }

  AtomPredicate operator!=(const AtomName& name) const {
    return AtomPredicate([name](const Atom& a){ return a.name() != name;});
  }

  AtomPredicate operator!=(const char* char_name) const {
    auto name = AtomName(char_name);
    return AtomPredicate([name](const Atom& a){ return a.name() != name;});
  }

  AtomPredicate operator!=(const std::string& string_name) const {
    auto name = AtomName(string_name);
    return AtomPredicate([name](const Atom& a){ return a.name() != name;});
  }

  AtomPredicate is_in(const std::set<AtomName>& names) const {
    return AtomPredicate([names](const Atom& a){ return names.count(a.name())==1 ;});
  }

  AtomPredicate is_in(const std::set<const char*>& char_names) const {
    std::set<AtomName> names;
    for (auto& name: char_names){
      names.insert(AtomName(name));
    }
    return AtomPredicate([names](const Atom& a){ return names.count(a.name())==1 ;});
  }

  AtomPredicate is_in(const std::set<std::string>& char_names) const {
    std::set<AtomName> names;
    for (auto& name: char_names){
      names.insert(AtomName(name));
    }
    return AtomPredicate([names](const Atom& a){ return names.count(a.name())==1 ;});
  }
};


class ResidueNamePredicateGenerator{
public:
  constexpr ResidueNamePredicateGenerator()=default;

  ResiduePredicate operator==(const ResidueName& name) const {
    return ResiduePredicate([name](const Residue& a){ return a.name() == name;});
  }

  ResiduePredicate operator==(const char* char_name) const {
    auto name = ResidueName(char_name);
    return ResiduePredicate([name](const Residue& a){ return a.name() == name;});
  }

  ResiduePredicate operator==(const std::string& string_name) const {
    auto name = ResidueName(string_name);
    return ResiduePredicate([name](const Residue& a){ return a.name() == name;});
  }


  ResiduePredicate operator!=(const ResidueName& name) const {
    return ResiduePredicate([name](const Residue& a){ return a.name() != name;});
  }

  ResiduePredicate operator!=(const char* char_name) const {
    auto name = ResidueName(char_name);
    return ResiduePredicate([name](const Residue& a){ return a.name() != name;});
  }

  ResiduePredicate operator!=(const std::string& string_name) const {
    auto name = ResidueName(string_name);
    return ResiduePredicate([name](const Residue& a){ return a.name() != name;});
  }

  ResiduePredicate is_in(const std::set<ResidueName>& names) const {
    return ResiduePredicate([names](const Residue& r){ return names.count(r.name())==1 ;});
  }

  ResiduePredicate is_in(const std::set<const char*>& char_names) const {
    std::set<ResidueName> names;
    for (auto& name: char_names){
      names.insert(ResidueName(name));
    }
    return ResiduePredicate([names](const Residue& r){ return names.count(r.name())==1 ;});
  }

  ResiduePredicate is_in(const std::set<std::string>& string_names) const {
    std::set<ResidueName> names;
    for (auto& name: string_names){
      names.insert(ResidueName(name));
    }
    return ResiduePredicate([names](const Residue& r){ return names.count(r.name())==1 ;});
  }
};


class ChainNamePredicateGenerator{
public:
  constexpr ChainNamePredicateGenerator()=default;

  ChainPredicate operator==(const ChainName& name) const {
    return ChainPredicate([name](const Chain& a){ return a.name() == name;});
  }

  ChainPredicate operator==(const char* char_name) const {
    auto name = ChainName(char_name);
    return ChainPredicate([name](const Chain& a){ return a.name() == name;});
  }

  ChainPredicate operator==(const std::string& string_name) const {
    auto name = ChainName(string_name);
    return ChainPredicate([name](const Chain& a){ return a.name() == name;});
  }


  ChainPredicate operator!=(const ChainName& name) const {
    return ChainPredicate([name](const Chain& a){ return a.name() != name;});
  }

  ChainPredicate operator!=(const char* char_name) const {
    auto name = ChainName(char_name);
    return ChainPredicate([name](const Chain& a){ return a.name() != name;});
  }

  ChainPredicate operator!=(const std::string& string_name) const {
    auto name = ChainName(string_name);
    return ChainPredicate([name](const Chain& a){ return a.name() != name;});
  }

  ChainPredicate is_in(const std::set<ChainName>& names) const {
    return ChainPredicate([names](const Chain& a){ return names.count(a.name())==1 ;});
  }

  ChainPredicate is_in(const std::set<const char*>& char_names) const {
    std::set<ChainName> names;
    for (auto& name: char_names){
      names.insert(ChainName(name));
    }
    return ChainPredicate([names](const Chain& a){ return names.count(a.name())==1 ;});
  }

  ChainPredicate is_in(const std::set<std::string>& string_names) const {
    std::set<ChainName> names;
    for (auto& name: string_names){
      names.insert(ChainName(name));
    }
    return ChainPredicate([names](const Chain& a){ return names.count(a.name())==1 ;});
  }
};


class AtomIdPredicateGenerator{
public:
  constexpr AtomIdPredicateGenerator()=default;

  AtomPredicate operator==(const atomId_t& id) const {
    return AtomPredicate([id](const Atom& a){ return a.id() == id;});
  }

  AtomPredicate operator!=(const atomId_t& id) const {
    return AtomPredicate([id](const Atom& a){ return a.id() != id;});
  }

  AtomPredicate operator<=(const atomId_t& id) const {
    return AtomPredicate([id](const Atom& a){ return a.id() <= id;});
  }

  AtomPredicate operator<(const atomId_t& id) const {
    return AtomPredicate([id](const Atom& a){ return a.id() < id;});
  }

  AtomPredicate operator>=(const atomId_t& id) const {
    return AtomPredicate([id](const Atom& a){ return a.id() >= id;});
  }

  AtomPredicate operator>(const atomId_t& id) const {
    return AtomPredicate([id](const Atom& a){ return a.id() > id;});
  }

  AtomPredicate is_in(const std::set<atomId_t>& ids) const {
    return AtomPredicate([ids](const Atom& a){ return ids.count(a.id())==1 ;});
  }

};


class ResidueIdPredicateGenerator{
public:
  constexpr ResidueIdPredicateGenerator()=default;

  ResiduePredicate operator==(const residueId_t& id) const {
    return ResiduePredicate([id](const Residue& r){ return r.id() == id;});
  }

  ResiduePredicate operator!=(const residueId_t& id) const {
    return ResiduePredicate([id](const Residue& r){ return r.id() != id;});
  }

  ResiduePredicate operator<=(const residueId_t& id) const {
    return ResiduePredicate([id](const Residue& r){ return r.id() <= id;});
  }

  ResiduePredicate operator<(const residueId_t& id) const {
    return ResiduePredicate([id](const Residue& r){ return r.id() < id;});
  }

  ResiduePredicate operator>=(const residueId_t& id) const {
    return ResiduePredicate([id](const Residue& r){ return r.id() >= id;});
  }

  ResiduePredicate operator>(const residueId_t& id) const {
    return ResiduePredicate([id](const Residue& r){ return r.id() > id;});
  }

  ResiduePredicate is_in(const std::set<residueId_t>& ids) const {
    return ResiduePredicate([ids](const Residue& r){ return ids.count(r.id())==1 ;});
  }

  ResiduePredicate operator==(const residueSerial_t& id) const {
    return ResiduePredicate([id](const Residue& r){ return r.id() == id;});
  }

  ResiduePredicate operator!=(const residueSerial_t& id) const {
    return ResiduePredicate([id](const Residue& r){ return r.id() != id;});
  }

  ResiduePredicate operator<=(const residueSerial_t& id) const {
    return ResiduePredicate([id](const Residue& r){ return r.id() <= id;});
  }

  ResiduePredicate operator<(const residueSerial_t& id) const {
    return ResiduePredicate([id](const Residue& r){ return r.id() < id;});
  }

  ResiduePredicate operator>=(const residueSerial_t& id) const {
    return ResiduePredicate([id](const Residue& r){ return r.id() >= id;});
  }

  ResiduePredicate operator>(const residueSerial_t& id) const {
    return ResiduePredicate([id](const Residue& r){ return r.id() > id;});
  }

  ResiduePredicate is_in(const std::set<residueSerial_t>& ids) const {
    return ResiduePredicate([ids](const Residue& r){ return r.id().iCode.value()==0 && ids.count(r.id().serial)==1 ;});
  }

};


class ChainIndexPredicateGenerator{
public:
  constexpr ChainIndexPredicateGenerator()=default;

  ChainPredicate operator==(const chainIndex_t& index) const {
    return ChainPredicate([index](const Chain& c){ return c.index() == index;});
  }

  ChainPredicate operator!=(const chainIndex_t& index) const {
    return ChainPredicate([index](const Chain& c){ return c.index() != index;});
  }

  ChainPredicate operator<=(const chainIndex_t& index) const {
    return ChainPredicate([index](const Chain& c){ return c.index() <= index;});
  }

  ChainPredicate operator<(const chainIndex_t& index) const {
    return ChainPredicate([index](const Chain& c){ return c.index() < index;});
  }

  ChainPredicate operator>=(const chainIndex_t& index) const {
    return ChainPredicate([index](const Chain& c){ return c.index() >= index;});
  }

  ChainPredicate operator>(const chainIndex_t& index) const {
    return ChainPredicate([index](const Chain& c){ return c.index() > index;});
  }

  ChainPredicate is_in(const std::set<chainIndex_t>& ids) const {
    return ChainPredicate([ids](const Chain& c){ return ids.count(c.index())==1 ;});
  }

};


constexpr auto aName = AtomNamePredicateGenerator{};
constexpr auto rName = ResidueNamePredicateGenerator{};
constexpr auto cName = ChainNamePredicateGenerator{};


constexpr auto aId = AtomIdPredicateGenerator{};
constexpr auto rId = ResidueIdPredicateGenerator{};
constexpr auto cIndex = ChainIndexPredicateGenerator{};

}
}