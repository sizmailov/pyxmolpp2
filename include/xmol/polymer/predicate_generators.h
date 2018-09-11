#pragma once

#include "xmol/polymer/predicates.h"
#include <type_traits>

namespace xmol{
namespace polymer {

class AtomNamePredicateGenerator{
public:
  constexpr AtomNamePredicateGenerator()=default;

  GenericAtomPredicate operator==(const AtomName& name) const {
    return GenericAtomPredicate([name](const Atom& a){ return a.name() == name;});
  }

  GenericAtomPredicate operator==(const char* char_name) const {
    auto name = AtomName(char_name);
    return GenericAtomPredicate([name](const Atom& a){ return a.name() == name;});
  }

  GenericAtomPredicate operator==(const std::string& string_name) const {
    auto name = AtomName(string_name);
    return GenericAtomPredicate([name](const Atom& a){ return a.name() == name;});
  }

  GenericAtomPredicate operator!=(const AtomName& name) const {
    return GenericAtomPredicate([name](const Atom& a){ return a.name() != name;});
  }

  GenericAtomPredicate operator!=(const char* char_name) const {
    auto name = AtomName(char_name);
    return GenericAtomPredicate([name](const Atom& a){ return a.name() != name;});
  }

  GenericAtomPredicate operator!=(const std::string& string_name) const {
    auto name = AtomName(string_name);
    return GenericAtomPredicate([name](const Atom& a){ return a.name() != name;});
  }

  GenericAtomPredicate is_in(const std::set<AtomName>& names) const {
    return GenericAtomPredicate([names](const Atom& a){ return names.count(a.name())==1 ;});
  }

  GenericAtomPredicate is_in(const std::set<const char*>& char_names) const {
    std::set<AtomName> names;
    for (auto& name: char_names){
      names.insert(AtomName(name));
    }
    return GenericAtomPredicate([names](const Atom& a){ return names.count(a.name())==1 ;});
  }

  GenericAtomPredicate is_in(const std::set<std::string>& char_names) const {
    std::set<AtomName> names;
    for (auto& name: char_names){
      names.insert(AtomName(name));
    }
    return GenericAtomPredicate([names](const Atom& a){ return names.count(a.name())==1 ;});
  }
};


class ResidueNamePredicateGenerator{
public:
  constexpr ResidueNamePredicateGenerator()=default;

  GenericResiduePredicate operator==(const ResidueName& name) const {
    return GenericResiduePredicate([name](const Residue& a){ return a.name() == name;});
  }

  GenericResiduePredicate operator==(const char* char_name) const {
    auto name = ResidueName(char_name);
    return GenericResiduePredicate([name](const Residue& a){ return a.name() == name;});
  }

  GenericResiduePredicate operator==(const std::string& string_name) const {
    auto name = ResidueName(string_name);
    return GenericResiduePredicate([name](const Residue& a){ return a.name() == name;});
  }


  GenericResiduePredicate operator!=(const ResidueName& name) const {
    return GenericResiduePredicate([name](const Residue& a){ return a.name() != name;});
  }

  GenericResiduePredicate operator!=(const char* char_name) const {
    auto name = ResidueName(char_name);
    return GenericResiduePredicate([name](const Residue& a){ return a.name() != name;});
  }

  GenericResiduePredicate operator!=(const std::string& string_name) const {
    auto name = ResidueName(string_name);
    return GenericResiduePredicate([name](const Residue& a){ return a.name() != name;});
  }

  GenericResiduePredicate is_in(const std::set<ResidueName>& names) const {
    return GenericResiduePredicate([names](const Residue& r){ return names.count(r.name())==1 ;});
  }

  GenericResiduePredicate is_in(const std::set<const char*>& char_names) const {
    std::set<ResidueName> names;
    for (auto& name: char_names){
      names.insert(ResidueName(name));
    }
    return GenericResiduePredicate([names](const Residue& r){ return names.count(r.name())==1 ;});
  }

  GenericResiduePredicate is_in(const std::set<std::string>& string_names) const {
    std::set<ResidueName> names;
    for (auto& name: string_names){
      names.insert(ResidueName(name));
    }
    return GenericResiduePredicate([names](const Residue& r){ return names.count(r.name())==1 ;});
  }
};


class ChainNamePredicateGenerator{
public:
  constexpr ChainNamePredicateGenerator()=default;

  GenericChainPredicate operator==(const ChainName& name) const {
    return GenericChainPredicate([name](const Chain& a){ return a.name() == name;});
  }

  GenericChainPredicate operator==(const char* char_name) const {
    auto name = ChainName(char_name);
    return GenericChainPredicate([name](const Chain& a){ return a.name() == name;});
  }

  GenericChainPredicate operator==(const std::string& string_name) const {
    auto name = ChainName(string_name);
    return GenericChainPredicate([name](const Chain& a){ return a.name() == name;});
  }


  GenericChainPredicate operator!=(const ChainName& name) const {
    return GenericChainPredicate([name](const Chain& a){ return a.name() != name;});
  }

  GenericChainPredicate operator!=(const char* char_name) const {
    auto name = ChainName(char_name);
    return GenericChainPredicate([name](const Chain& a){ return a.name() != name;});
  }

  GenericChainPredicate operator!=(const std::string& string_name) const {
    auto name = ChainName(string_name);
    return GenericChainPredicate([name](const Chain& a){ return a.name() != name;});
  }

  GenericChainPredicate is_in(const std::set<ChainName>& names) const {
    return GenericChainPredicate([names](const Chain& a){ return names.count(a.name())==1 ;});
  }

  GenericChainPredicate is_in(const std::set<const char*>& char_names) const {
    std::set<ChainName> names;
    for (auto& name: char_names){
      names.insert(ChainName(name));
    }
    return GenericChainPredicate([names](const Chain& a){ return names.count(a.name())==1 ;});
  }

  GenericChainPredicate is_in(const std::set<std::string>& string_names) const {
    std::set<ChainName> names;
    for (auto& name: string_names){
      names.insert(ChainName(name));
    }
    return GenericChainPredicate([names](const Chain& a){ return names.count(a.name())==1 ;});
  }
};


class AtomIdPredicateGenerator{
public:
  constexpr AtomIdPredicateGenerator()=default;

  GenericAtomPredicate operator==(const atomId_t& id) const {
    return GenericAtomPredicate([id](const Atom& a){ return a.id() == id;});
  }

  GenericAtomPredicate operator!=(const atomId_t& id) const {
    return GenericAtomPredicate([id](const Atom& a){ return a.id() != id;});
  }

  GenericAtomPredicate operator<=(const atomId_t& id) const {
    return GenericAtomPredicate([id](const Atom& a){ return a.id() <= id;});
  }

  GenericAtomPredicate operator<(const atomId_t& id) const {
    return GenericAtomPredicate([id](const Atom& a){ return a.id() < id;});
  }

  GenericAtomPredicate operator>=(const atomId_t& id) const {
    return GenericAtomPredicate([id](const Atom& a){ return a.id() >= id;});
  }

  GenericAtomPredicate operator>(const atomId_t& id) const {
    return GenericAtomPredicate([id](const Atom& a){ return a.id() > id;});
  }

  GenericAtomPredicate is_in(const std::set<atomId_t>& ids) const {
    return GenericAtomPredicate([ids](const Atom& a){ return ids.count(a.id())==1 ;});
  }

};


class ResidueIdPredicateGenerator{
public:
  constexpr ResidueIdPredicateGenerator()=default;

  GenericResiduePredicate operator==(const residueId_t& id) const {
    return GenericResiduePredicate([id](const Residue& r){ return r.id() == id;});
  }

  GenericResiduePredicate operator!=(const residueId_t& id) const {
    return GenericResiduePredicate([id](const Residue& r){ return r.id() != id;});
  }

  GenericResiduePredicate operator<=(const residueId_t& id) const {
    return GenericResiduePredicate([id](const Residue& r){ return r.id() <= id;});
  }

  GenericResiduePredicate operator<(const residueId_t& id) const {
    return GenericResiduePredicate([id](const Residue& r){ return r.id() < id;});
  }

  GenericResiduePredicate operator>=(const residueId_t& id) const {
    return GenericResiduePredicate([id](const Residue& r){ return r.id() >= id;});
  }

  GenericResiduePredicate operator>(const residueId_t& id) const {
    return GenericResiduePredicate([id](const Residue& r){ return r.id() > id;});
  }

  GenericResiduePredicate is_in(const std::set<residueId_t>& ids) const {
    return GenericResiduePredicate([ids](const Residue& r){ return ids.count(r.id())==1 ;});
  }

};


class ChainIndexPredicateGenerator{
public:
  constexpr ChainIndexPredicateGenerator()=default;

  GenericChainPredicate operator==(const chainIndex_t& index) const {
    return GenericChainPredicate([index](const Chain& c){ return c.index() == index;});
  }

  GenericChainPredicate operator!=(const chainIndex_t& index) const {
    return GenericChainPredicate([index](const Chain& c){ return c.index() != index;});
  }

  GenericChainPredicate operator<=(const chainIndex_t& index) const {
    return GenericChainPredicate([index](const Chain& c){ return c.index() <= index;});
  }

  GenericChainPredicate operator<(const chainIndex_t& index) const {
    return GenericChainPredicate([index](const Chain& c){ return c.index() < index;});
  }

  GenericChainPredicate operator>=(const chainIndex_t& index) const {
    return GenericChainPredicate([index](const Chain& c){ return c.index() >= index;});
  }

  GenericChainPredicate operator>(const chainIndex_t& index) const {
    return GenericChainPredicate([index](const Chain& c){ return c.index() > index;});
  }

  GenericChainPredicate is_in(const std::set<chainIndex_t>& ids) const {
    return GenericChainPredicate([ids](const Chain& c){ return ids.count(c.index())==1 ;});
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