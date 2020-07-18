#pragma once

#include "MoleculeRef.h"
#include "xmol/proxy/FrameObserver.h"

namespace xmol::proxy {

/// Smart Molecule reference proxy
class ConstMoleculeSmartRef : public FrameObserver<ConstMoleculeSmartRef>, public api::ConstMoleculeAPI<ConstMoleculeSmartRef> {
public:
  explicit ConstMoleculeSmartRef(const MoleculeRef& atom);

  using api::ConstMoleculeAPI<ConstMoleculeSmartRef>::frame; // disambiguate from FrameObserver::frame

  operator const ConstMoleculeRef&() const {
    check_invariants("operator const MoleculeRef&()");
    return m_ref.const_();
  }

  operator const MoleculeRef&() const {
    check_invariants("operator const MoleculeRef&()");
    return m_ref;
  }

private:
  MoleculeRef m_ref;
  friend Frame;
  friend MoleculeSmartRef;

  friend api::MoleculeAPI<ConstMoleculeSmartRef>;
  friend api::ConstMoleculeAPI<ConstMoleculeSmartRef>;

  [[nodiscard]] constexpr const BaseMolecule* mol_ptr() const { return m_ref.mol_ptr(); }

  void on_base_molecules_move(BaseMolecule* from_begin, BaseMolecule* from_end, BaseMolecule* to_begin);

  constexpr void check_invariants(const char* func_name) const {
    if (!is_bound_to_frame()) {
      throw DeadFrameAccessError(std::string("ConstMoleculeSmartRef::") + func_name);
    }
  }
};
} // namespace xmol::proxy