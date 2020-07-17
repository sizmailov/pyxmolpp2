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
class AtomRef {
public:
  constexpr AtomRef(const AtomRef& rhs) = default;
  constexpr AtomRef(AtomRef&& rhs) noexcept = default;
  constexpr AtomRef& operator=(const AtomRef& rhs) = default;
  constexpr AtomRef& operator=(AtomRef&& rhs) noexcept = default;

  [[nodiscard]] const XYZ& r() const {
    check_invariants("r");
    return *coord_ptr();
  }

  [[maybe_unused]] const AtomRef& r(const XYZ& value) const& {
    check_invariants("r");
    *coord_ptr() = value;
    return *this;
  }

  [[maybe_unused]] AtomRef r(const XYZ& value) const&& {
    check_invariants("r");
    *coord_ptr() = value;
    return *this;
  }

  [[nodiscard]] constexpr AtomId id() const {
    check_invariants("id");
    return atom_ptr()->id;
  }

  [[maybe_unused]] constexpr const AtomRef& id(const AtomId& value) const& {
    check_invariants("id");
    atom_ptr()->id = value;
    return *this;
  }

  [[maybe_unused]] constexpr AtomRef id(const AtomId& value) const&& {
    check_invariants("id");
    atom_ptr()->id = value;
    return *this;
  }

  [[nodiscard]] constexpr float mass() const {
    check_invariants("mass");
    return atom_ptr()->mass;
  }

  [[maybe_unused]] constexpr const AtomRef& mass(const float& value) const& {
    check_invariants("mass");
    atom_ptr()->mass = value;
    return *this;
  }

  [[maybe_unused]] constexpr AtomRef mass(const float& value) const&& {
    check_invariants("mass");
    atom_ptr()->mass = value;
    return *this;
  }

  [[nodiscard]] constexpr float vdw_radius() const {
    check_invariants("vdw_radius");
    return atom_ptr()->vdw_radius;
  }

  [[maybe_unused]] constexpr const AtomRef& vdw_radius(const float& value) const& {
    check_invariants("vdw_radius");
    atom_ptr()->vdw_radius = value;
    return *this;
  }

  [[maybe_unused]] constexpr AtomRef vdw_radius(const float& value) const&& {
    check_invariants("vdw_radius");
    atom_ptr()->vdw_radius = value;
    return *this;
  }

  [[nodiscard]] constexpr AtomName name() const {
    check_invariants("name");
    return atom_ptr()->name;
  }

  template <int N>[[maybe_unused]] constexpr const AtomRef& name(const char (&value)[N]) const& {
    check_invariants("name");
    atom_ptr()->name = AtomName(value);
    return *this;
  }

  template <int N>[[maybe_unused]] constexpr AtomRef name(const char (&value)[N]) const&& {
    check_invariants("name");
    atom_ptr()->name = AtomName(value);
    return *this;
  }

  [[maybe_unused]] const AtomRef& name(const std::string& value) const& {
    check_invariants("name");
    atom_ptr()->name = AtomName(value);
    return *this;
  }

  [[maybe_unused]] AtomRef name(const std::string& value) const&& {
    check_invariants("name");
    atom_ptr()->name = AtomName(value);
    return *this;
  }

  [[maybe_unused]] constexpr const AtomRef& name(const AtomName& value) const& {
    check_invariants("name");
    atom_ptr()->name = value;
    return *this;
  }

  [[maybe_unused]] constexpr AtomRef name(const AtomName& value) const&& {
    check_invariants("name");
    atom_ptr()->name = value;
    return *this;
  }

  [[nodiscard]] AtomIndex index() const;

  [[nodiscard]] constexpr ResidueRef residue() const {
    check_invariants("residue");
    return ResidueRef(*atom_ptr()->residue);
  }

  [[nodiscard]] constexpr MoleculeRef molecule() const {
    check_invariants("molecule");
    return MoleculeRef(*atom_ptr()->residue->molecule);
  }

  [[nodiscard]] constexpr Frame& frame() const {
    check_invariants("frame");
    return *atom_ptr()->residue->molecule->frame;
  }

  /// Create smart reference from this
  [[nodiscard]] AtomSmartRef smart() const;

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