#pragma once

#include "ConstResidueSmartRef.h"
#include "xmol/proxy/FrameObserver.h"

namespace xmol::proxy {

/// Smart Residue reference proxy
class ResidueSmartRef : public api::ResidueAPI<ResidueSmartRef> {
public:
  explicit ResidueSmartRef(const ResidueRef& res) : m_ref(res){}

  operator const ResidueRef&() const& {
    check_invariants("operator ResidueRef&");
    return m_ref;
  };

  operator ResidueRef() const && {
    check_invariants("operator ResidueRef&");
    return m_ref;
  };

private:
  ConstResidueSmartRef m_ref;

  void on_base_residues_move(BaseResidue* from_begin, BaseResidue* from_end, BaseResidue* to_begin);

  constexpr void check_invariants(const char* func_name) const {
    m_ref.check_invariants(func_name);
  }

  [[nodiscard]] constexpr BaseResidue* res_ptr() const { return m_ref.m_ref.res_ptr(); }

  friend api::ResidueAPI<ResidueSmartRef>;
  friend api::ConstResidueAPI<ResidueSmartRef>;

  friend Frame;
};
} // namespace xmol::proxy