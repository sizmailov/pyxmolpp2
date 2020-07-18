#pragma once

#include "MoleculeRef.h"
#include "ResidueRef.h"
#include "xmol/proxy/FrameObserver.h"

namespace xmol::proxy {
/// Smart Molecule reference proxy
class MoleculeSmartRef : public FrameObserver<MoleculeSmartRef>, public api::MoleculeAPI<MoleculeSmartRef> {
public:
  explicit MoleculeSmartRef(MoleculeRef mol);

  using api::MoleculeAPI<MoleculeSmartRef>::frame; // disambiguate from FrameObserver::frame;

  operator const MoleculeRef&() const& {
    check_invariants("operator MoleculeRef&");
    return m_ref;
  };

  operator MoleculeRef() const&& {
    check_invariants("operator const MoleculeRef&()");
    return m_ref;
  };

private:
  MoleculeRef m_ref;
  void on_base_molecules_move(BaseMolecule* from_begin, BaseMolecule* from_end, BaseMolecule* to_begin);
  constexpr void check_invariants(const char* func_name) const {
    if (!is_bound_to_frame()) {
      throw DeadFrameAccessError(std::string("MoleculeSmartRef::") + func_name);
    }
  }
  [[nodiscard]] constexpr BaseMolecule* mol_ptr() const { return m_ref.mol_ptr(); }

  friend Frame;
  friend api::MoleculeAPI<MoleculeSmartRef>;
  friend api::ConstMoleculeAPI<MoleculeSmartRef>;
};
} // namespace xmol::proxy