#pragma once

#include "xmol/polymer/Atom.h"
#include <type_traits>

namespace xmol{
namespace polymer{


class GenericChainPredicate;
class GenericResiduePredicate;
class GenericAtomPredicate;



class GenericChainPredicate{
public:
  template<typename Pred>
  explicit GenericChainPredicate(Pred&& predicate): m_predicate(std::forward<Pred>(predicate)){
    static_assert( std::is_same<typename std::result_of<Pred(const Chain&)>::type, bool>::value,"");
  };
  GenericChainPredicate(const GenericChainPredicate& ) = default;
  GenericChainPredicate(GenericChainPredicate&& )= default;
  GenericChainPredicate&operator=(const GenericChainPredicate& ) = default;
  GenericChainPredicate&operator=(GenericChainPredicate&& ) = default;

  bool operator()(const Chain& chain) const {
    return m_predicate(chain);
  }

  bool operator()(const Residue& residue) const {
    return m_predicate(residue.chain());
  }

  bool operator()(const Atom& atom) const{
    return m_predicate(atom.residue().chain());
  }

  GenericChainPredicate operator!() const{
    auto pred = m_predicate;
    return GenericChainPredicate([pred](const Chain& c)->bool { return !pred(c);});
  }

  GenericChainPredicate operator&&(const GenericChainPredicate& rhs) const;
  GenericChainPredicate operator||(const GenericChainPredicate& rhs) const;
  GenericChainPredicate operator^(const GenericChainPredicate& rhs) const;

  GenericResiduePredicate operator&&(const GenericResiduePredicate& rhs) const;
  GenericResiduePredicate operator||(const GenericResiduePredicate& rhs) const;
  GenericResiduePredicate operator^(const GenericResiduePredicate& rhs) const;

  GenericAtomPredicate operator&&(const GenericAtomPredicate& rhs) const;
  GenericAtomPredicate operator||(const GenericAtomPredicate& rhs) const;
  GenericAtomPredicate operator^(const GenericAtomPredicate& rhs) const;

private:
  friend class GenericAtomPredicate;
  friend class GenericResiduePredicate;
  std::function<bool(const Chain&)> m_predicate;
};

class GenericResiduePredicate{
public:
  template<typename Pred>
  explicit GenericResiduePredicate(Pred&& predicate): m_predicate(std::forward<Pred>(predicate)){
    static_assert( std::is_same<typename std::result_of<Pred(const Residue&)>::type, bool>::value,"");
  };
  GenericResiduePredicate(const GenericResiduePredicate& ) = default;
  GenericResiduePredicate(GenericResiduePredicate&& )= default;
  GenericResiduePredicate&operator=(const GenericResiduePredicate& ) = default;
  GenericResiduePredicate&operator=(GenericResiduePredicate&& ) = default;

  bool operator()(const Residue& residue) const {
    return m_predicate(residue);
  }

  bool operator()(const Atom& atom) const{
    return m_predicate(atom.residue());
  }

  GenericResiduePredicate operator!() const{
    auto pred = m_predicate;
    return GenericResiduePredicate([pred](const Residue& r)->bool { return !pred(r);});
  }

  GenericResiduePredicate operator&&(const GenericChainPredicate& rhs) const;
  GenericResiduePredicate operator||(const GenericChainPredicate& rhs) const;
  GenericResiduePredicate operator^(const GenericChainPredicate& rhs) const;

  GenericResiduePredicate operator&&(const GenericResiduePredicate& rhs) const;
  GenericResiduePredicate operator||(const GenericResiduePredicate& rhs) const;
  GenericResiduePredicate operator^(const GenericResiduePredicate& rhs) const;

  GenericAtomPredicate operator&&(const GenericAtomPredicate& rhs) const;
  GenericAtomPredicate operator||(const GenericAtomPredicate& rhs) const;
  GenericAtomPredicate operator^(const GenericAtomPredicate& rhs) const;

private:
  friend class GenericAtomPredicate;
  friend class GenericChainPredicate;
  std::function<bool(const Residue&)> m_predicate;
};

class GenericAtomPredicate{
public:
  template<typename Pred>
  explicit GenericAtomPredicate(Pred&& predicate): m_predicate(std::forward<Pred>(predicate)){
    static_assert( std::is_same<typename std::result_of<Pred(const Atom&)>::type, bool>::value,"");
  };
  GenericAtomPredicate(const GenericAtomPredicate& ) = default;
  GenericAtomPredicate(GenericAtomPredicate&& )= default;
  GenericAtomPredicate&operator=(const GenericAtomPredicate& ) = default;
  GenericAtomPredicate&operator=(GenericAtomPredicate&& ) = default;

  bool operator()(const Atom& atom) const{
    return m_predicate(atom);
  }

  GenericAtomPredicate operator!() const{
    auto pred = m_predicate;
    return GenericAtomPredicate([pred](const Atom& a)->bool { return !pred(a);});
  }

  GenericAtomPredicate operator&&(const GenericChainPredicate& rhs) const;
  GenericAtomPredicate operator||(const GenericChainPredicate& rhs) const;
  GenericAtomPredicate operator^(const GenericChainPredicate& rhs) const;

  GenericAtomPredicate operator&&(const GenericResiduePredicate& rhs) const;
  GenericAtomPredicate operator||(const GenericResiduePredicate& rhs) const;
  GenericAtomPredicate operator^(const GenericResiduePredicate& rhs) const;

  GenericAtomPredicate operator&&(const GenericAtomPredicate& rhs) const;
  GenericAtomPredicate operator||(const GenericAtomPredicate& rhs) const;
  GenericAtomPredicate operator^(const GenericAtomPredicate& rhs) const;

private:
  friend class GenericResiduePredicate;
  friend class GenericChainPredicate;
  std::function<bool(const Atom&)> m_predicate;
};



}
}