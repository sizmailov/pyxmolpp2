#pragma once

#include "ConstAtomRef.h"
#include "MoleculeRef.h"
#include "ResidueRef.h"

namespace xmol::proxy {

/** @brief Lightweight atom reference
 *
 * Provides access to @ref XYZ and @ref BaseAtom parts of atom
 *
 * For ref-counting reference see @ref smart::AtomSmartRef
 * */
class AtomRef : public api::AtomAPI<AtomRef> {
public:
  constexpr AtomRef(const AtomRef& rhs) = default;
  constexpr AtomRef(AtomRef&& rhs) noexcept = default;
  constexpr AtomRef& operator=(const AtomRef& rhs) = default;
  constexpr AtomRef& operator=(AtomRef&& rhs) noexcept = default;

  /// Create smart reference from this
  [[nodiscard]] AtomSmartRef smart() const;

private:
  ConstAtomRef m_cref;

  [[nodiscard]] constexpr XYZ* coord_ptr() const { return m_cref.m_coord; }
  [[nodiscard]] constexpr BaseAtom* atom_ptr() const { return m_cref.m_atom; }

  constexpr void check_invariants(const char*) const {};

  friend AtomSpan;
  friend AtomSelection;
  friend Frame;
  friend MoleculeSelection;
  friend ProxySpan<AtomRef, BaseAtom>;
  friend ResidueRef;
  friend ResidueSelection;
  friend Selection<proxy::AtomRef>::LessThanComparator;
  friend AtomSmartRef;
  friend smart::AtomSmartSelection;
  friend smart::AtomSmartSpan;
  friend api::AtomAPI<AtomRef>;
  friend api::ConstAtomAPI<AtomRef>;

  template<typename, typename>
  friend class api::ResidueAPI;

  explicit AtomRef(BaseAtom& atom) : m_cref(atom) {}

  AtomRef(BaseAtom* ptr, BaseAtom* end) : m_cref(ptr, end) {}
  void advance() { m_cref.advance(); }
  void rebase(BaseAtom* from, BaseAtom* to) { m_cref.rebase(from, to); }
  void rebase(XYZ* from, XYZ* to) { m_cref.rebase(from, to); }
  constexpr AtomRef() = default; // constructs object in invalid state (with nullptrs)
};

std::string to_string(const AtomRef& atom);

template <> struct Selection<proxy::AtomRef>::LessThanComparator {
  bool operator()(const proxy::AtomRef& p1, const proxy::AtomRef& p2) { return p1.atom_ptr() < p2.atom_ptr(); }
};

} // namespace xmol::proxy