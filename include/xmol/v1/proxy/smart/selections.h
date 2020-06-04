#pragma once
#include "../../fwd.h"
#include "../selections.h"
#include "FrameObserver.h"

namespace xmol::v1::proxy::smart {

class AtomSmartSelection : public AtomSelection, public FrameObserver<AtomSmartSelection> {

private:
  friend Frame;
  void on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin);
  void on_base_atoms_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin);
};

} // namespace xmol::v1::proxy::smart