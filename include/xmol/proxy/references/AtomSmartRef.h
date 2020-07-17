#pragma once

#include "AtomRef.h"
#include "xmol/proxy/FrameObserver.h"

namespace xmol::proxy {

/// Smart Atom reference proxy
class AtomSmartRef : public FrameObserver<AtomSmartRef> {
public:
  explicit AtomSmartRef(AtomRef atom);

  [[nodiscard]] const XYZ& r() const {
    check_invariants("r");
    return m_ref.r();
  }

  [[maybe_unused]] const AtomRef& r(const XYZ& value) & {
    check_invariants("r");
    return m_ref.r(value);
  }

  [[maybe_unused]] AtomRef r(const XYZ& value) && {
    check_invariants("r");
    return m_ref.r(value);
  }

  [[nodiscard]] constexpr AtomId id() const {
    check_invariants("id");
    return m_ref.id();
  }

  [[maybe_unused]] constexpr const AtomRef& id(const AtomId& value) const& {
    check_invariants("id");
    return m_ref.id(value);
  }

  [[maybe_unused]] constexpr AtomRef id(const AtomId& value) const&& {
    check_invariants("id");
    return m_ref.id(value);
  }

  [[nodiscard]] constexpr float mass() const {
    check_invariants("mass");
    return m_ref.mass();
  }

  [[maybe_unused]] constexpr const AtomRef& mass(const float& value) const& {
    check_invariants("mass");
    return m_ref.mass(value);
  }

  [[maybe_unused]] constexpr AtomRef mass(const float& value) const&& {
    check_invariants("mass");
    return m_ref.mass(value);
  }

  [[nodiscard]] constexpr float vdw_radius() const {
    check_invariants("vdw_radius");
    return m_ref.vdw_radius();
  }

  [[maybe_unused]] constexpr const AtomRef& vdw_radius(const float& value) const& {
    check_invariants("vdw_radius");
    return m_ref.vdw_radius(value);
  }

  [[maybe_unused]] constexpr AtomRef vdw_radius(const float& value) const&& {
    check_invariants("vdw_radius");
    return m_ref.vdw_radius(value);
  }

  [[nodiscard]] constexpr AtomName name() const {
    check_invariants("name");
    return m_ref.name();
  }

  template <int N>[[maybe_unused]] constexpr const AtomRef& name(const char (&value)[N]) const& {
    check_invariants("name");
    return m_ref.name(value);
  }

  template <int N>[[maybe_unused]] constexpr AtomRef name(const char (&value)[N]) const&& {
    check_invariants("name");
    return m_ref.name(value);
  }

  [[maybe_unused]] const AtomRef& name(const std::string& value) const& {
    check_invariants("name");
    return m_ref.name(value);
  }

  [[maybe_unused]] AtomRef name(const std::string& value) const&& {
    check_invariants("name");
    return m_ref.name(value);
  }

  [[maybe_unused]] constexpr const AtomRef& name(const AtomName& value) const& {
    check_invariants("name");
    return m_ref.name(value);
  }

  [[maybe_unused]] constexpr AtomRef name(const AtomName& value) const&& {
    check_invariants("name");
    return m_ref.name(value);
  }

  [[nodiscard]] AtomIndex index() const {
    check_invariants("index");
    return m_ref.index();
  }

  [[nodiscard]] constexpr ResidueRef residue() const {
    check_invariants("residue");
    return m_ref.residue();
  }

  [[nodiscard]] constexpr MoleculeRef molecule() const {
    check_invariants("molecule");
    return m_ref.molecule();
  }

  [[nodiscard]] constexpr Frame& frame() const {
    check_invariants("frame");
    return m_ref.frame();
  }

  /// Check if references point to same data
  bool operator!=(const AtomRef& rhs) const {
    check_invariants("operator!=()");
    return m_ref != rhs;
  }

  /// Check if references point to same data
  bool operator==(const AtomRef& rhs) const {
    check_invariants("operator==()");
    return m_ref == rhs;
  }

  operator AtomRef() const&& {
    check_invariants("operator AtomRef()");
    return m_ref;
  }
  operator const AtomRef&() const& {
    check_invariants("operator const AtomRef&()");
    return m_ref;
  }

private:
  AtomRef m_ref;
  friend Frame;

  void on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin);
  void on_base_atoms_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin);

  constexpr void check_invariants(const char* func_name) const {
    if (!is_bound_to_frame()) {
      throw DeadFrameAccessError(std::string("AtomSmartRef::") + func_name);
    }
  }
};
} // namespace xmol::proxy