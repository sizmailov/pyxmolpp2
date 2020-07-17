#pragma once

#include "ConstMoleculeRef.h"
#include "ConstResidueRef.h"
#include "xmol/proxy/Selection.h"
#include "xmol/proxy/spans.h"

namespace xmol::proxy {

class ConstAtomRef {
public:
  constexpr ConstAtomRef(const ConstAtomRef& rhs) = default;
  constexpr ConstAtomRef(ConstAtomRef&& rhs) noexcept = default;
  constexpr ConstAtomRef& operator=(const ConstAtomRef& rhs) = default;
  constexpr ConstAtomRef& operator=(ConstAtomRef&& rhs) noexcept = default;

  [[nodiscard]] constexpr const XYZ& r() const {
    check_invariants("r");
    return *coord_ptr();
  }

  [[nodiscard]] constexpr AtomId id() const {
    check_invariants("id");
    return atom_ptr()->id;
  }

  [[nodiscard]] constexpr float mass() const {
    check_invariants("mass");
    return atom_ptr()->mass;
  }

  [[nodiscard]] constexpr float vdw_radius() const {
    check_invariants("vdw_radius");
    return atom_ptr()->vdw_radius;
  }

  [[nodiscard]] constexpr AtomName name() const {
    check_invariants("name");
    return atom_ptr()->name;
  }

  [[nodiscard]] AtomIndex index() const;

  [[nodiscard]] constexpr ConstResidueRef residue() const {
    check_invariants("residue");
    return ConstResidueRef(*atom_ptr()->residue);
  }

  [[nodiscard]] constexpr ConstMoleculeRef molecule() const {
    check_invariants("molecule");
    return ConstMoleculeRef(*atom_ptr()->residue->molecule);
  }

  [[nodiscard]] constexpr const Frame& frame() const {
    check_invariants("frame");
    return *atom_ptr()->residue->molecule->frame;
  }

  constexpr bool operator!=(const ConstAtomRef& rhs) const {
    return atom_ptr() != rhs.atom_ptr(); // comparing only one pair of pointers since they always must be in sync
  }

  /// Check if references point to same data
  constexpr bool operator==(const ConstAtomRef& rhs) const {
    return atom_ptr() == rhs.atom_ptr(); // comparing only one pair of pointers since they always must be in sync
  }

private:
  XYZ* m_coord = nullptr;
  BaseAtom* m_atom = nullptr;

  [[nodiscard]] constexpr const XYZ* coord_ptr() const { return m_coord; }
  [[nodiscard]] constexpr const BaseAtom* atom_ptr() const { return m_atom; }

  constexpr void check_invariants(const char*) const {};

  constexpr void advance() {
    ++m_atom;
    ++m_coord;
  }

  void rebase(BaseAtom* from, BaseAtom* to) { m_atom = to + (m_atom - from); }
  void rebase(XYZ* from, XYZ* to) { m_coord = to + (m_coord - from); }

  constexpr ConstAtomRef() = default; // constructs object in invalid state (with nullptrs)

  explicit ConstAtomRef(BaseAtom& atom);
  ConstAtomRef(BaseAtom* ptr, BaseAtom* end);

  friend AtomRef;
};

} // namespace xmol::proxy