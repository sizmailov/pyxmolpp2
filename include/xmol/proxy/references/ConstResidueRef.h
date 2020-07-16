#pragma once

#include "mixins/ResidueRefMixin.h"
#include "xmol/base.h"
#include "xmol/proxy/Selection.h"
#include "xmol/proxy/spans.h"

namespace xmol::proxy {

class ConstResidueRef : public ResidueGettersMixin<ConstResidueRef> {
public:
  constexpr ConstResidueRef(const ConstResidueRef& rhs) = default;
  constexpr ConstResidueRef(ConstResidueRef&& rhs) noexcept = default;
  constexpr ConstResidueRef& operator=(const ConstResidueRef& rhs) = default;
  constexpr ConstResidueRef& operator=(ConstResidueRef&& rhs) noexcept = default;

private:
  BaseResidue* m_residue = nullptr;

  constexpr void check_invariants(const char*) const {};

  constexpr BaseResidue* const res_ptr() const { return m_residue; }
  constexpr BaseResidue* const& res_ptr() { return m_residue; }

  constexpr explicit ConstResidueRef(BaseResidue& residue) : m_residue(&residue){};
  constexpr ConstResidueRef(BaseResidue* ptr, BaseResidue*) : m_residue(ptr){};
  constexpr void advance() { ++m_residue; }
  constexpr ConstResidueRef() = default; /// constructs object in invalid state (with nullptrs)

  friend ResidueRef;
};

} // namespace xmol::proxy