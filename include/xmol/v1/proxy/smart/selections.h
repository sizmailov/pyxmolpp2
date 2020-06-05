#pragma once
#include "../../fwd.h"
#include "../selections.h"
#include "FrameObserver.h"

namespace xmol::v1::proxy::smart {

class AtomSmartSelection : public AtomSelection, public FrameObserver<AtomSmartSelection> {
public:
  AtomSmartSelection(AtomSelection sel);

  /// Inplace union
  AtomSmartSelection& operator|=(const AtomSelection& rhs) {
    AtomSelection::operator|=(rhs);
    return *this;
  };

  /// Inplace difference
  AtomSmartSelection& operator-=(const AtomSelection& rhs) {
    AtomSelection::operator-=(rhs);
    return *this;
  };

  /// Inplace intersection
  AtomSmartSelection& operator&=(const AtomSelection& rhs) {
    AtomSelection::operator&=(rhs);
    return *this;
  };

private:
  void check_precondition(const char* func_name) const override {
    if (!is_bound_to_frame() && !m_data.empty()) {
      throw DeadFrameAccessError(std::string("AtomSmartSelection::") + func_name);
    }
  }
  friend Frame;
  struct AtomRefLessThanComparator;
  void on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin);
  void on_base_atoms_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin);
};

} // namespace xmol::v1::proxy::smart