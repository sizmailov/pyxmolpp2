#pragma once

#include "xmol/base.h"
#include "xmol/proxy/references/MoleculeRef.h"
#include "xmol/proxy/references/ResidueRef.h"

namespace xmol::proxy::api {

template <typename Derived> class ConstAtomAPI {
private:
  [[nodiscard]] constexpr const Derived& derived() const { return static_cast<const Derived&>(*this); }

public:
  [[nodiscard]] const XYZ& r() const {
    derived().check_invariants("r");
    return *derived().coord_ptr();
  }

  [[nodiscard]] constexpr AtomId id() const {
    derived().check_invariants("id");
    return derived().atom_ptr()->id;
  }

  [[nodiscard]] constexpr float mass() const {
    derived().check_invariants("mass");
    return derived().atom_ptr()->mass;
  }

  [[nodiscard]] constexpr float vdw_radius() const {
    derived().check_invariants("vdw_radius");
    return derived().atom_ptr()->vdw_radius;
  }

  [[nodiscard]] constexpr AtomName name() const {
    derived().check_invariants("name");
    return derived().atom_ptr()->name;
  }

  [[nodiscard]] AtomIndex index() const;

  [[nodiscard]] constexpr ConstResidueRef residue() const {
    derived().check_invariants("residue");
    return ConstResidueRef(*derived().atom_ptr()->residue);
  }

  [[nodiscard]] constexpr ConstMoleculeRef molecule() const {
    derived().check_invariants("molecule");
    return ConstMoleculeRef(*derived().atom_ptr()->residue->molecule);
  }

  [[nodiscard]] constexpr const Frame& frame() const {
    derived().check_invariants("frame");
    return *derived().atom_ptr()->residue->molecule->frame;
  }

  constexpr bool operator!=(const ConstAtomAPI& rhs) const {
    derived().check_invariants("operator!=");
    return derived().atom_ptr() !=
           rhs.derived().atom_ptr(); // comparing only one pair of pointers since they always must be in sync
  }

  constexpr bool operator==(const ConstAtomAPI& rhs) const {
    derived().check_invariants("operator==");
    return derived().atom_ptr() ==
           rhs.derived().atom_ptr(); // comparing only one pair of pointers since they always must be in sync
  }
};

template <typename Derived, typename ARef> class AtomAPI : public ConstAtomAPI<Derived> {
private:
  [[nodiscard]] constexpr const Derived& derived() const { return static_cast<const Derived&>(*this); }

public:
  using ConstAtomAPI<Derived>::r;
  using ConstAtomAPI<Derived>::mass;
  using ConstAtomAPI<Derived>::vdw_radius;
  using ConstAtomAPI<Derived>::name;
  using ConstAtomAPI<Derived>::index;
  using ConstAtomAPI<Derived>::id;

  [[maybe_unused]] const ARef& r(const XYZ& value) const& {
    derived().check_invariants("r");
    *derived().coord_ptr() = value;
    return static_cast<const ARef&>(derived());
  }

  [[maybe_unused]] ARef r(const XYZ& value) const&& {
    derived().check_invariants("r");
    *derived().coord_ptr() = value;
    return static_cast<const ARef&>(derived());
  }

  [[maybe_unused]] constexpr const ARef& id(const AtomId& value) const& {
    derived().check_invariants("id");
    derived().atom_ptr()->id = value;
    return static_cast<const ARef&>(derived());
  }

  [[maybe_unused]] constexpr ARef id(const AtomId& value) const&& {
    derived().check_invariants("id");
    derived().atom_ptr()->id = value;
    return static_cast<const ARef&>(derived());
  }

  [[maybe_unused]] constexpr const ARef& mass(const float& value) const& {
    derived().check_invariants("mass");
    derived().atom_ptr()->mass = value;
    return static_cast<const ARef&>(derived());
  }

  [[maybe_unused]] constexpr ARef mass(const float& value) const&& {
    derived().check_invariants("mass");
    derived().atom_ptr()->mass = value;
    return static_cast<const ARef&>(derived());
  }

  [[maybe_unused]] constexpr const ARef& vdw_radius(const float& value) const& {
    derived().check_invariants("vdw_radius");
    derived().atom_ptr()->vdw_radius = value;
    return static_cast<const ARef&>(derived());
  }

  [[maybe_unused]] constexpr ARef vdw_radius(const float& value) const&& {
    derived().check_invariants("vdw_radius");
    derived().atom_ptr()->vdw_radius = value;
    return static_cast<const ARef&>(derived());
  }

  template <int N>[[maybe_unused]] constexpr const ARef& name(const char (&value)[N]) const& {
    derived().check_invariants("name");
    derived().atom_ptr()->name = AtomName(value);
    return static_cast<const ARef&>(derived());
  }

  template <int N>[[maybe_unused]] constexpr ARef name(const char (&value)[N]) const&& {
    derived().check_invariants("name");
    derived().atom_ptr()->name = AtomName(value);
    return static_cast<const ARef&>(derived());
  }

  [[maybe_unused]] const ARef& name(const std::string& value) const& {
    derived().check_invariants("name");
    derived().atom_ptr()->name = AtomName(value);
    return static_cast<const ARef&>(derived());
  }

  [[maybe_unused]] ARef name(const std::string& value) const&& {
    derived().check_invariants("name");
    derived().atom_ptr()->name = AtomName(value);
    return static_cast<const ARef&>(derived());
  }

  [[maybe_unused]] constexpr const ARef& name(const AtomName& value) const& {
    derived().check_invariants("name");
    derived().atom_ptr()->name = value;
    return static_cast<const ARef&>(derived());
  }

  [[maybe_unused]] constexpr ARef name(const AtomName& value) const&& {
    derived().check_invariants("name");
    derived().atom_ptr()->name = value;
    return static_cast<const ARef&>(derived());
  }

  [[nodiscard]] constexpr ResidueRef residue() const {
    derived().check_invariants("residue");
    return ResidueRef(*derived().atom_ptr()->residue);
  }

  [[nodiscard]] constexpr MoleculeRef molecule() const {
    derived().check_invariants("molecule");
    return MoleculeRef(*derived().atom_ptr()->residue->molecule);
  }

  [[nodiscard]] constexpr Frame& frame() const {
    derived().check_invariants("frame");
    return *derived().atom_ptr()->residue->molecule->frame;
  }
};

} // namespace xmol::proxy::references::api