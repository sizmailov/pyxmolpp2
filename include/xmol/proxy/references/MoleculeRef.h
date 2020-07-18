#pragma once

#include "ConstMoleculeRef.h"

namespace xmol::proxy {

/** @brief Lightweight molecule reference
 *
 * For ref-counting reference see @ref smart::MoleculeSmartRef
 * */
class MoleculeRef : public api::MoleculeAPI<MoleculeRef>{
public:

  /// Create smart reference from this
  [[nodiscard]] MoleculeSmartRef smart() const;
private:
  ConstMoleculeRef m_cref;

  constexpr void check_invariants(const char*) const {};
  [[nodiscard]] constexpr BaseMolecule* mol_ptr() const { return m_cref.m_molecule; }

  friend AtomRef;
  friend AtomSelection;
  friend Frame;
  friend MoleculeSpan;
  friend MoleculeSelection;
  friend ProxySpan<MoleculeRef, BaseMolecule>;
  friend ResidueRef;
  friend ResidueSelection;
  friend Selection<MoleculeRef>::LessThanComparator;

  friend class api::MoleculeAPI<MoleculeRef>;
  friend class api::ConstMoleculeAPI<MoleculeRef>;

  template<typename, typename>
  friend class proxy::api::AtomAPI;
  template<typename, typename>
  friend class proxy::api::ResidueAPI;

  friend MoleculeSmartRef;
  friend smart::MoleculeSmartSelection;

  constexpr explicit MoleculeRef(BaseMolecule& molecule) : m_cref(molecule){};
  constexpr MoleculeRef(BaseMolecule* ptr, BaseMolecule*) : m_cref(*ptr){};
  constexpr void advance() { m_cref.advance(); };
  void rebase(BaseMolecule* from, BaseMolecule* to) { m_cref.rebase(from, to); }
  constexpr MoleculeRef() = default; // constructs object in invalid state (with nullptrs)
};

inline std::string to_string(const MoleculeRef& mol) { return mol.name().str(); }

template <> struct Selection<proxy::MoleculeRef>::LessThanComparator {
  bool operator()(const proxy::MoleculeRef& p1, const proxy::MoleculeRef& p2) { return p1.mol_ptr() < p2.mol_ptr(); }
};

} // namespace xmol::proxy