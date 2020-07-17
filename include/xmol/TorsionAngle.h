#pragma once

#include "geom/AngleValue.h"
#include "xmol/proxy/references/AtomSmartRef.h"
#include <functional>
#include <optional>
#include <set>
#include <map>

namespace xmol {

class TorsionAngle {
public:
  using ARef = proxy::AtomRef;
  using ASRef = proxy::AtomSmartRef;
  using AffectedAtomsSelector = std::function<proxy::AtomSelection(const ARef&, const ARef&,const  ARef&,const  ARef&)>;

  TorsionAngle(ARef a, ARef b, ARef c, ARef d);
  TorsionAngle(ARef a, ARef b, ARef c, ARef d, AffectedAtomsSelector rotatedAtomsGetter);

  geom::AngleValue value() const;

  void set(const geom::AngleValue& value, geom::AngleValue noop_tolerance = geom::Degrees(0.01));

private:
  ASRef a, b, c, d;
  AffectedAtomsSelector m_affected_atoms;
};


} // namespace xmol