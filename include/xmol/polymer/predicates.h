#pragma once

#include "xmol/polymer/Atom.h"
#include <type_traits>

namespace xmol{
namespace polymer{


class ChainPredicate;
class ResiduePredicate;
class AtomPredicate;



class ChainPredicate{
public:
  template<typename Pred>
  explicit ChainPredicate(Pred&& predicate): m_predicate(std::forward<Pred>(predicate)){
    static_assert( std::is_same<typename std::result_of<Pred(const Chain&)>::type, bool>::value,"");
  };
  ChainPredicate(const ChainPredicate& ) = default;
  ChainPredicate(ChainPredicate&& )= default;
  ChainPredicate&operator=(const ChainPredicate& ) = default;
  ChainPredicate&operator=(ChainPredicate&& ) = default;

  bool operator()(const Chain& chain) const {
    return m_predicate(chain);
  }

  bool operator()(const Residue& residue) const {
    return m_predicate(residue.chain());
  }

  bool operator()(const Atom& atom) const{
    return m_predicate(atom.residue().chain());
  }

  ChainPredicate operator!() const{
    auto pred = m_predicate;
    return ChainPredicate([pred](const Chain& c)->bool { return !pred(c);});
  }

  ChainPredicate operator&&(const ChainPredicate& rhs) const;
  ChainPredicate operator||(const ChainPredicate& rhs) const;
  ChainPredicate operator^(const ChainPredicate& rhs) const;

  ResiduePredicate operator&&(const ResiduePredicate& rhs) const;
  ResiduePredicate operator||(const ResiduePredicate& rhs) const;
  ResiduePredicate operator^(const ResiduePredicate& rhs) const;

  AtomPredicate operator&&(const AtomPredicate& rhs) const;
  AtomPredicate operator||(const AtomPredicate& rhs) const;
  AtomPredicate operator^(const AtomPredicate& rhs) const;

private:
  friend class AtomPredicate;
  friend class ResiduePredicate;
  std::function<bool(const Chain&)> m_predicate;
};

class ResiduePredicate{
public:
  template<typename Pred>
  explicit ResiduePredicate(Pred&& predicate): m_predicate(std::forward<Pred>(predicate)){
    static_assert( std::is_same<typename std::result_of<Pred(const Residue&)>::type, bool>::value,"");
  };
  ResiduePredicate(const ResiduePredicate& ) = default;
  ResiduePredicate(ResiduePredicate&& )= default;
  ResiduePredicate&operator=(const ResiduePredicate& ) = default;
  ResiduePredicate&operator=(ResiduePredicate&& ) = default;

  bool operator()(const Residue& residue) const {
    return m_predicate(residue);
  }

  bool operator()(const Atom& atom) const{
    return m_predicate(atom.residue());
  }

  ResiduePredicate operator!() const{
    auto pred = m_predicate;
    return ResiduePredicate([pred](const Residue& r)->bool { return !pred(r);});
  }

  ResiduePredicate operator&&(const ChainPredicate& rhs) const;
  ResiduePredicate operator||(const ChainPredicate& rhs) const;
  ResiduePredicate operator^(const ChainPredicate& rhs) const;

  ResiduePredicate operator&&(const ResiduePredicate& rhs) const;
  ResiduePredicate operator||(const ResiduePredicate& rhs) const;
  ResiduePredicate operator^(const ResiduePredicate& rhs) const;

  AtomPredicate operator&&(const AtomPredicate& rhs) const;
  AtomPredicate operator||(const AtomPredicate& rhs) const;
  AtomPredicate operator^(const AtomPredicate& rhs) const;

private:
  friend class AtomPredicate;
  friend class ChainPredicate;
  std::function<bool(const Residue&)> m_predicate;
};

class AtomPredicate{
public:
  template<typename Pred>
  explicit AtomPredicate(Pred&& predicate): m_predicate(std::forward<Pred>(predicate)){
    static_assert( std::is_same<typename std::result_of<Pred(const Atom&)>::type, bool>::value,"");
  };
  AtomPredicate(const AtomPredicate& ) = default;
  AtomPredicate(AtomPredicate&& )= default;
  AtomPredicate&operator=(const AtomPredicate& ) = default;
  AtomPredicate&operator=(AtomPredicate&& ) = default;

  bool operator()(const Atom& atom) const{
    return m_predicate(atom);
  }

  AtomPredicate operator!() const{
    auto pred = m_predicate;
    return AtomPredicate([pred](const Atom& a)->bool { return !pred(a);});
  }

  AtomPredicate operator&&(const ChainPredicate& rhs) const;
  AtomPredicate operator||(const ChainPredicate& rhs) const;
  AtomPredicate operator^(const ChainPredicate& rhs) const;

  AtomPredicate operator&&(const ResiduePredicate& rhs) const;
  AtomPredicate operator||(const ResiduePredicate& rhs) const;
  AtomPredicate operator^(const ResiduePredicate& rhs) const;

  AtomPredicate operator&&(const AtomPredicate& rhs) const;
  AtomPredicate operator||(const AtomPredicate& rhs) const;
  AtomPredicate operator^(const AtomPredicate& rhs) const;

private:
  friend class ResiduePredicate;
  friend class ChainPredicate;
  std::function<bool(const Atom&)> m_predicate;
};



}
}