#pragma once

#include "mixins/AtomRefMixin.h"
#include "xmol/base.h"
#include "xmol/proxy/Selection.h"
#include "xmol/proxy/spans.h"

namespace xmol::proxy {

class ConstAtomRef : public AtomGettersMixin<ConstAtomRef> {
public:
  constexpr ConstAtomRef(const ConstAtomRef& rhs) = default;
  constexpr ConstAtomRef(ConstAtomRef&& rhs) noexcept = default;
  constexpr ConstAtomRef& operator=(const ConstAtomRef& rhs) = default;
  constexpr ConstAtomRef& operator=(ConstAtomRef&& rhs) noexcept = default;

private:

  XYZ* m_coord = nullptr;
  BaseAtom* m_atom = nullptr;

  constexpr XYZ* const coord_ptr() const { return m_coord; }
  constexpr XYZ* const& coord_ptr() { return m_coord; }
  constexpr BaseAtom* const atom_ptr() const { return m_atom; }
  constexpr BaseAtom* const& atom_ptr() { return m_atom; }

  constexpr void check_invariants(const char*) const {};

  constexpr void advance() {
    ++m_atom;
    ++m_coord;
  }

  constexpr ConstAtomRef() = default; // constructs object in invalid state (with nullptrs)

  explicit ConstAtomRef(BaseAtom& atom);
  ConstAtomRef(BaseAtom* ptr, BaseAtom* end);

  friend AtomRef;
};

} // namespace xmol::proxy