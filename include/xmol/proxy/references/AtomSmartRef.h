#pragma once

#include "AtomRef.h"
#include "xmol/proxy/FrameObserver.h"

namespace xmol::proxy {

/// Smart Atom reference proxy
class AtomSmartRef : public FrameObserver<AtomSmartRef>, public api::AtomAPI<AtomSmartRef> {
public:
  explicit AtomSmartRef(AtomRef atom);

  using api::AtomAPI<AtomSmartRef>::frame; // disambiguate with FrameObserver::frame

  operator AtomRef() const&& {
    check_invariants("operator AtomRef()");
    return m_ref;
  }
  operator const AtomRef&() const& {
    check_invariants("operator const AtomRef&()");
    return m_ref;
  }

private:
  AtomRef m_ref;
  friend Frame;

  friend api::AtomAPI<AtomSmartRef>;
  friend api::ConstAtomAPI<AtomSmartRef>;

  [[nodiscard]] constexpr XYZ* coord_ptr() const { return m_ref.coord_ptr(); }
  [[nodiscard]] constexpr BaseAtom* atom_ptr() const { return m_ref.atom_ptr(); }

  void on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin);
  void on_base_atoms_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin);

  constexpr void check_invariants(const char* func_name) const {
    if (!is_bound_to_frame()) {
      throw DeadFrameAccessError(std::string("AtomSmartRef::") + func_name);
    }
  }
};
} // namespace xmol::proxy