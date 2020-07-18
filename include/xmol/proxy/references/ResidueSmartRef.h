#pragma once

#include "AtomRef.h"
#include "MoleculeRef.h"
#include "ResidueRef.h"
#include "xmol/proxy/FrameObserver.h"

namespace xmol::proxy {

/// Smart Residue reference proxy
class ResidueSmartRef : public FrameObserver<ResidueSmartRef>, public api::ResidueAPI<ResidueSmartRef>{
public:
  explicit ResidueSmartRef(ResidueRef res);

  using api::ResidueAPI<ResidueSmartRef>::frame; // disambiguate with FrameObserver::frame;

  operator const ResidueRef&() const &{
    check_invariants("operator ResidueRef&");
    return m_ref;
  };

  operator ResidueRef() const && {
    check_invariants("operator ResidueRef&");
    return m_ref;
  };

private:
  ResidueRef m_ref;

  void on_base_residues_move(BaseResidue* from_begin, BaseResidue* from_end, BaseResidue* to_begin);

  constexpr void check_invariants(const char* func_name) const {
    if (!is_bound_to_frame()) {
      throw DeadFrameAccessError(std::string("ResidueSmartRef::") + func_name);
    }
  }

  [[nodiscard]] constexpr BaseResidue* res_ptr() const { return m_ref.res_ptr(); }

  friend api::ResidueAPI<ResidueSmartRef>;
  friend api::ConstResidueAPI<ResidueSmartRef>;

  friend Frame;
};
} // namespace xmol::proxy