#pragma once

#include "AtomRef.h"
#include "xmol/proxy/FrameObserver.h"

namespace xmol::proxy {

/// Smart Atom reference proxy
class ConstAtomSmartRef : public FrameObserver<ConstAtomSmartRef>, public api::ConstAtomAPI<ConstAtomSmartRef> {
public:
  explicit ConstAtomSmartRef(AtomRef atom);

  using api::ConstAtomAPI<ConstAtomSmartRef>::frame; // disambiguate from FrameObserver::frame

  operator ConstAtomRef() const&& {
    check_invariants("operator AtomRef()");
    return m_ref.const_();
  }
  operator const ConstAtomRef&() const& {
    check_invariants("operator const AtomRef&()");
    return m_ref.const_();
  }

private:
  AtomRef m_ref;
  friend Frame;
  friend AtomSmartRef;

  friend api::AtomAPI<ConstAtomSmartRef>;
  friend api::ConstAtomAPI<ConstAtomSmartRef>;

  [[nodiscard]] constexpr const XYZ* coord_ptr() const { return m_ref.coord_ptr(); }
  [[nodiscard]] constexpr const BaseAtom* atom_ptr() const { return m_ref.atom_ptr(); }

  void on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin);
  void on_base_atoms_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin);

  constexpr void check_invariants(const char* func_name) const {
    if (!is_bound_to_frame()) {
      throw DeadFrameAccessError(std::string("ConstAtomSmartRef::") + func_name);
    }
  }
};
} // namespace xmol::proxy