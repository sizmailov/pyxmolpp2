#pragma once

#include "mixins/AtomRefMixin.h"
#include "xmol/base.h"
#include "xmol/proxy/Selection.h"
#include "xmol/proxy/spans.h"

namespace xmol::proxy {

class AtomConstRef : public AtomGettersMixin<AtomConstRef> {
public:
  constexpr AtomConstRef(const AtomConstRef& rhs) = default;
  constexpr AtomConstRef(AtomConstRef&& rhs) noexcept = default;
  constexpr AtomConstRef& operator=(const AtomConstRef& rhs) = default;
  constexpr AtomConstRef& operator=(AtomConstRef&& rhs) noexcept = default;

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

  constexpr AtomConstRef() = default; // constructs object in invalid state (with nullptrs)

  explicit AtomConstRef(BaseAtom& atom);
  AtomConstRef(BaseAtom* ptr, BaseAtom* end);

  friend AtomRef;
};

} // namespace xmol::proxy