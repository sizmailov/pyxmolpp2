#pragma once

#include "xmol/proxy/proxy.h"
#include <type_traits>

namespace xmol::predicates {

using namespace xmol::proxy;

class MoleculePredicate;
class ResiduePredicate;
class AtomPredicate;

class MoleculePredicate {
public:
  template <typename Pred> explicit MoleculePredicate(Pred&& predicate) : m_predicate(std::forward<Pred>(predicate)) {
    static_assert(std::is_same<typename std::result_of<Pred(const MoleculeRef&)>::type, bool>::value);
  };
  MoleculePredicate(const MoleculePredicate&) = default;
  MoleculePredicate(MoleculePredicate&&) = default;
  MoleculePredicate& operator=(const MoleculePredicate&) = default;
  MoleculePredicate& operator=(MoleculePredicate&&) = default;

  bool operator()(MoleculeRef& MoleculeRef) const { return m_predicate(MoleculeRef); }

  bool operator()(ResidueRef& ResidueRef) const { return m_predicate(ResidueRef.molecule()); }

  bool operator()(AtomRef& AtomRef) const { return m_predicate(AtomRef.molecule()); };

  MoleculePredicate operator!() const {
    auto pred = m_predicate;
    return MoleculePredicate([pred](const MoleculeRef& c) -> bool { return !pred(c); });
  }

  MoleculePredicate operator&&(const MoleculePredicate& rhs) const;
  MoleculePredicate operator||(const MoleculePredicate& rhs) const;
  MoleculePredicate operator^(const MoleculePredicate& rhs) const;

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

  ResiduePredicate operator&&(const MoleculePredicate& rhs) const;
  ResiduePredicate operator||(const MoleculePredicate& rhs) const;
  ResiduePredicate operator^(const MoleculePredicate& rhs) const;

  ResiduePredicate operator&&(const ResiduePredicate& rhs) const;
  ResiduePredicate operator||(const ResiduePredicate& rhs) const;
  ResiduePredicate operator^(const ResiduePredicate& rhs) const;

  AtomPredicate operator&&(const AtomPredicate& rhs) const;
  AtomPredicate operator||(const AtomPredicate& rhs) const;
  AtomPredicate operator^(const AtomPredicate& rhs) const;

private:
  friend class AtomPredicate;
  friend class MoleculePredicate;
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

  AtomPredicate operator&&(const MoleculePredicate& rhs) const;
  AtomPredicate operator||(const MoleculePredicate& rhs) const;
  AtomPredicate operator^(const MoleculePredicate& rhs) const;

  AtomPredicate operator&&(const ResiduePredicate& rhs) const;
  AtomPredicate operator||(const ResiduePredicate& rhs) const;
  AtomPredicate operator^(const ResiduePredicate& rhs) const;

  AtomPredicate operator&&(const AtomPredicate& rhs) const;
  AtomPredicate operator||(const AtomPredicate& rhs) const;
  AtomPredicate operator^(const AtomPredicate& rhs) const;

private:
  friend class ResiduePredicate;
  friend class MoleculePredicate;
  std::function<bool(const AtomRef&)> m_predicate;
};

} // namespace xmol::predicates