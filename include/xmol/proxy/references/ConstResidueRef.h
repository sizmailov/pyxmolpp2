#pragma once

#include "mixins/ResidueRefMixin.h"
#include "xmol/base.h"
#include "xmol/proxy/Selection.h"
#include "xmol/proxy/spans.h"

namespace xmol::proxy {

class ResidueConstRef : public ResidueGettersMixin<ResidueConstRef> {
public:
  constexpr ResidueConstRef(const ResidueConstRef& rhs) = default;
  constexpr ResidueConstRef(ResidueConstRef&& rhs) noexcept = default;
  constexpr ResidueConstRef& operator=(const ResidueConstRef& rhs) = default;
  constexpr ResidueConstRef& operator=(ResidueConstRef&& rhs) noexcept = default;

private:
  BaseResidue* m_residue = nullptr;

  constexpr void check_invariants(const char*) const {};

  constexpr BaseResidue* const res_ptr() const { return m_residue; }
  constexpr BaseResidue* const& res_ptr() { return m_residue; }

  constexpr explicit ResidueConstRef(BaseResidue& residue) : m_residue(&residue){};
  constexpr ResidueConstRef(BaseResidue* ptr, BaseResidue*) : m_residue(ptr){};
  constexpr void advance() { ++m_residue; }
  constexpr ResidueConstRef() = default; /// constructs object in invalid state (with nullptrs)

  friend ResidueRef;
};

} // namespace xmol::proxy