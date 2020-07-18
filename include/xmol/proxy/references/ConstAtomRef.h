#pragma once

#include "ConstMoleculeRef.h"
#include "ConstResidueRef.h"
#include "api/AtomAPI.h"
#include "xmol/proxy/Selection.h"
#include "xmol/proxy/spans.h"

namespace xmol::proxy {

class ConstAtomRef : public api::ConstAtomAPI<ConstAtomRef> {
public:
  constexpr ConstAtomRef(const ConstAtomRef& rhs) = default;
  constexpr ConstAtomRef(ConstAtomRef&& rhs) noexcept = default;
  constexpr ConstAtomRef& operator=(const ConstAtomRef& rhs) = default;
  constexpr ConstAtomRef& operator=(ConstAtomRef&& rhs) noexcept = default;

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