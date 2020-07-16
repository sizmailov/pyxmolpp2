#pragma once

#include "ConstAtomRef.h"
#include "ResidueRef.h"

namespace xmol::proxy {

/** @brief Lightweight atom reference
 *
 * Provides access to @ref XYZ and @ref BaseAtom parts of atom
 *
 * For ref-counting reference see @ref smart::AtomSmartRef
 * */
class AtomRef : public AtomSettersMixin<AtomRef> {
public:
  constexpr AtomRef(const AtomRef& rhs) = default;
  constexpr AtomRef(AtomRef&& rhs) noexcept = default;
  constexpr AtomRef& operator=(const AtomRef& rhs) = default;
  constexpr AtomRef& operator=(AtomRef&& rhs) noexcept = default;

  /// Index of the atom in frame (0-based)
  [[nodiscard]] AtomIndex index() const noexcept;

  /// Create smart reference from this
  smart::AtomSmartRef smart();

  const ConstAtomRef& const_() const& { return m_cref; }
  const ConstAtomRef&& const_() const&& { return std::move(m_cref); };

  /// Check if references point to same data
  constexpr bool operator!=(const AtomRef& rhs) const {
    return atom_ptr() != rhs.atom_ptr(); // comparing only one pair of pointers since they always must be in sync
  }

  /// Check if references point to same data
  constexpr bool operator==(const AtomRef& rhs) const {
    return atom_ptr() == rhs.atom_ptr(); // comparing only one pair of pointers since they always must be in sync
  }

private:
  ConstAtomRef m_cref;

  [[nodiscard]] constexpr XYZ* coord_ptr() const { return m_cref.m_coord; }
  [[nodiscard]] constexpr XYZ*& coord_ptr() { return m_cref.m_coord; }
  [[nodiscard]] constexpr BaseAtom* atom_ptr() const { return m_cref.m_atom; }
  [[nodiscard]] constexpr BaseAtom*& atom_ptr() { return m_cref.m_atom; }
  constexpr void check_invariants(const char*) const {};

  friend AtomGettersMixin<AtomRef>;
  friend AtomSettersMixin<AtomRef>;
  friend AtomSpan;
  friend AtomSelection;
  friend Frame;
  friend MoleculeSelection;
  friend ProxySpan<AtomRef, BaseAtom>;
  friend ResidueRef;
  friend ResidueSelection;
  friend Selection<proxy::AtomRef>::LessThanComparator;
  friend smart::AtomSmartRef;
  friend smart::AtomSmartSelection;
  friend smart::AtomSmartSpan;
  explicit AtomRef(BaseAtom& atom) : m_cref(atom) {}

  AtomRef(BaseAtom* ptr, BaseAtom* end) : m_cref(ptr, end) {}
  void advance() { m_cref.advance(); }
  constexpr AtomRef() = default; // constructs object in invalid state (with nullptrs)
};

std::string to_string(const AtomRef& atom);

template <> struct Selection<proxy::AtomRef>::LessThanComparator {
  bool operator()(const proxy::AtomRef& p1, const proxy::AtomRef& p2) { return p1.atom_ptr() < p2.atom_ptr(); }
};

} // namespace xmol::proxy