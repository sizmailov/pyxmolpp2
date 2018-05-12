#pragma once

#include "Atom.h"
#include "xmol/geometry/AngleValue.h"
#include <xmol/utils/optional.h>

namespace xmol {
namespace polymer {

class TorsionAngle {
public:
  using AffectedAtomsSelector = std::function<AtomSelection(Atom& a, Atom& b, Atom& c, Atom& d)>;

  TorsionAngle() = default;
  TorsionAngle(Atom& a, Atom& b, Atom& c, Atom& d);
  TorsionAngle(Atom& a, Atom& b, Atom& c, Atom& d, AffectedAtomsSelector rotatedAtomsGetter);

  geometry::AngleValue value() const;

  void set(const geometry::AngleValue& value, geometry::AngleValue noop_tolerance = geometry::Degrees(0.01));

private:
  AtomRef a, b, c, d;
  AffectedAtomsSelector rotatedAtomsGetter;
};

namespace detail{
  struct TorsionAngleNameTag{};
}

using TorsionAngleName = utils::ShortAsciiString<4,true,detail::TorsionAngleNameTag>;

class TorsionAngleFactory{
public:
  static utils::optional<TorsionAngle> phi(Residue& r);
  static utils::optional<TorsionAngle> psi(Residue& r);
  static utils::optional<TorsionAngle> omega(Residue& r);

  static utils::optional<TorsionAngle> chi1(Residue& r);
  static utils::optional<TorsionAngle> chi2(Residue& r);
  static utils::optional<TorsionAngle> chi3(Residue& r);
  static utils::optional<TorsionAngle> chi4(Residue& r);
  static utils::optional<TorsionAngle> chi5(Residue& r);

  static void define_protein_backbone_angles(ResidueName);
  static void define_protein_side_chain_angle(ResidueName, TorsionAngleName, const std::array<AtomName, 4>& names,
                                              const std::set<AtomName>& affected_atoms);
  static xmol::utils::optional<TorsionAngle> get(Residue& residue, const TorsionAngleName& angleName);

private:
  using residue_to_atoms = std::function<std::tuple<Atom*,Atom*,Atom*,Atom*>(Residue&)>;
  using KeyType = std::pair<ResidueName, TorsionAngleName>;
  using ValueType = std::pair<residue_to_atoms, TorsionAngle::AffectedAtomsSelector>;
  std::map<KeyType,ValueType> bindings;

  static TorsionAngleFactory& instance();
  TorsionAngleFactory();
  xmol::utils::optional<TorsionAngle> _get(Residue& residue, const TorsionAngleName& angleName);
  void _define_protein_backbone_angles(ResidueName);
};

}
}
