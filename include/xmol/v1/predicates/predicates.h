#pragma once

#include "xmol/v1/proxy/proxy.h"
#include <type_traits>

namespace xmol::v1::predicates {

using namespace xmol::v1::proxy;

class ChainPredicate;
class ResiduePredicate;
class AtomPredicate;

class ChainPredicate {
public:
  template <typename Pred> explicit ChainPredicate(Pred&& predicate) : m_predicate(std::forward<Pred>(predicate)) {
    static_assert(std::is_same<typename std::result_of<Pred(const MoleculeRef&)>::type, bool>::value);
  };
  ChainPredicate(const ChainPredicate&) = default;
  ChainPredicate(ChainPredicate&&) = default;
  ChainPredicate& operator=(const ChainPredicate&) = default;
  ChainPredicate& operator=(ChainPredicate&&) = default;

  bool operator()(MoleculeRef& MoleculeRef) const { return m_predicate(MoleculeRef); }

  bool operator()(ResidueRef& ResidueRef) const { return m_predicate(ResidueRef.molecule()); }

  bool operator()(AtomRef& AtomRef) const { return m_predicate(AtomRef.molecule()); };


  ChainPredicate operator!() const {
    auto pred = m_predicate;
    return ChainPredicate([pred](const MoleculeRef& c) -> bool { return !pred(c); });
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
  std::function<bool(const MoleculeRef&)> m_predicate;
};

class ResiduePredicate {
public:
  template <typename Pred> explicit ResiduePredicate(Pred&& predicate) : m_predicate(std::forward<Pred>(predicate)) {
    static_assert(std::is_same<typename std::result_of<Pred(const ResidueRef&)>::type, bool>::value);
  };
  ResiduePredicate(const ResiduePredicate&) = default;
  ResiduePredicate(ResiduePredicate&&) = default;
  ResiduePredicate& operator=(const ResiduePredicate&) = default;
  ResiduePredicate& operator=(ResiduePredicate&&) = default;

  bool operator()(ResidueRef& ResidueRef) const { return m_predicate(ResidueRef); }

  bool operator()(AtomRef& AtomRef) const { return m_predicate(AtomRef.residue()); }

  ResiduePredicate operator!() const {
    auto pred = m_predicate;
    return ResiduePredicate([pred](const ResidueRef& r) -> bool { return !pred(r); });
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
  std::function<bool(const ResidueRef&)> m_predicate;
};

class AtomPredicate {
public:
  template <typename Pred> explicit AtomPredicate(Pred&& predicate) : m_predicate(std::forward<Pred>(predicate)) {
    static_assert(std::is_same<typename std::result_of<Pred(const AtomRef&)>::type, bool>::value);
  };
  AtomPredicate(const AtomPredicate&) = default;
  AtomPredicate(AtomPredicate&&) = default;
  AtomPredicate& operator=(const AtomPredicate&) = default;
  AtomPredicate& operator=(AtomPredicate&&) = default;

  bool operator()(const AtomRef& AtomRef) const { return m_predicate(AtomRef); }

  AtomPredicate operator!() const {
    auto pred = m_predicate;
    return AtomPredicate([pred](const AtomRef& a) -> bool { return !pred(a); });
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
  std::function<bool(const AtomRef&)> m_predicate;
};

} // namespace xmol::v1::predicates