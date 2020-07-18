#pragma once

#include "ResidueRef.h"
#include "xmol/proxy/FrameObserver.h"

namespace xmol::proxy {

/// Smart Residue reference proxy
class ConstResidueSmartRef : public FrameObserver<ConstResidueSmartRef>, public api::ConstResidueAPI<ConstResidueSmartRef> {
public:
  explicit ConstResidueSmartRef(const ResidueRef& atom);

  using api::ConstResidueAPI<ConstResidueSmartRef>::frame; // disambiguate from FrameObserver::frame

  operator const ConstResidueRef&() const {
    check_invariants("operator const ResidueRef&()");
    return m_ref.const_();
  }

  operator const ResidueRef&() const {
    check_invariants("operator const ResidueRef&()");
    return m_ref;
  }

private:
  ResidueRef m_ref;
  friend Frame;
  friend ResidueSmartRef;

  friend api::ResidueAPI<ConstResidueSmartRef>;
  friend api::ConstResidueAPI<ConstResidueSmartRef>;

  [[nodiscard]] constexpr const BaseResidue* res_ptr() const { return m_ref.res_ptr(); }

  void on_base_residues_move(BaseResidue* from_begin, BaseResidue* from_end, BaseResidue* to_begin);

  constexpr void check_invariants(const char* func_name) const {
    if (!is_bound_to_frame()) {
      throw DeadFrameAccessError(std::string("ConstResidueSmartRef::") + func_name);
    }
  }
};
} // namespace xmol::proxy