#pragma once

#include "ConstAtomSmartRef.h"
#include "xmol/proxy/FrameObserver.h"

namespace xmol::proxy {

/// Smart Atom reference proxy
class AtomSmartRef : public api::AtomAPI<AtomSmartRef> {
public:
  explicit AtomSmartRef(AtomRef atom) : m_ref(atom){};

  using api::AtomAPI<AtomSmartRef>::frame; // disambiguate from FrameObserver::frame

  operator const AtomRef&() const {
    check_invariants("operator const AtomRef&");
    return m_ref.m_ref;
  }

  operator const ConstAtomSmartRef() const {
    check_invariants("operator const ConstAtomSmartRef&");
    return m_ref;
  }

  operator const ConstAtomRef&() {
    check_invariants("operator const ConstAtomRef&");
    return m_ref.m_ref.const_();
  }

private:
  ConstAtomSmartRef m_ref;
  friend Frame;

  friend api::AtomAPI<AtomSmartRef>;
  friend api::ConstAtomAPI<AtomSmartRef>;

  constexpr void check_invariants(const char* func_name) const { m_ref.check_invariants(func_name); }
  [[nodiscard]] constexpr XYZ* coord_ptr() const { return m_ref.m_ref.coord_ptr(); }
  [[nodiscard]] constexpr BaseAtom* atom_ptr() const { return m_ref.m_ref.atom_ptr(); }
};
} // namespace xmol::proxy