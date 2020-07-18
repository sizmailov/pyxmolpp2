#pragma once

#include "ConstMoleculeRef.h"
#include "xmol/base.h"
#include "api/ResidueAPI.h"

namespace xmol::proxy {

class ConstResidueRef : public api::ConstResidueAPI<ConstResidueRef> {
public:
  constexpr ConstResidueRef(const ConstResidueRef& rhs) = default;
  constexpr ConstResidueRef(ConstResidueRef&& rhs) noexcept = default;
  constexpr ConstResidueRef& operator=(const ConstResidueRef& rhs) = default;
  constexpr ConstResidueRef& operator=(ConstResidueRef&& rhs) noexcept = default;

private:

  template<typename>
  friend class proxy::api::ConstAtomAPI;

  template<typename, typename>
  friend class api::ConstResidueAPI;

  BaseResidue* m_residue = nullptr;

  constexpr void check_invariants(const char*) const {};

  [[nodiscard]] constexpr const BaseResidue* res_ptr() const { return m_residue; }

  constexpr explicit ConstResidueRef(BaseResidue& residue) : m_residue(&residue){};
  constexpr ConstResidueRef(BaseResidue* ptr, BaseResidue*) : m_residue(ptr){};
  constexpr void advance() { ++m_residue; }
  void rebase(BaseResidue* from, BaseResidue* to) { m_residue = to + (m_residue - from); }
  constexpr ConstResidueRef() = default; /// constructs object in invalid state (with nullptrs)

  friend ResidueRef;
  friend ConstAtomRef;
};

} // namespace xmol::proxy