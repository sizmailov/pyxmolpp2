#pragma once


#include "Atom.h"
#include "xmol/geometry/AngleValue.h"

namespace xmol {
namespace polymer {

class TorsionAngle {
public:
  using AffectedAtomsSelector = std::function<AtomSelection(Atom& a, Atom& b, Atom& c, Atom& d)>;

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

  static TorsionAngle phi(Residue& r);
  static TorsionAngle psi(Residue& r);
  static TorsionAngle omega(Residue& r);

  static TorsionAngle chi1(Residue& r);
  static TorsionAngle chi2(Residue& r);
  static TorsionAngle chi3(Residue& r);
  static TorsionAngle chi4(Residue& r);
  static TorsionAngle chi5(Residue& r);

private:
  using residue_to_atoms = std::function<std::tuple<Atom&,Atom&,Atom&,Atom&>(Residue&)>;
  using KeyType = std::pair<ResidueName, TorsionAngleName>;
  using ValueType = std::pair<residue_to_atoms, TorsionAngle::AffectedAtomsSelector>;
  std::map<KeyType,ValueType> bindings;

  static TorsionAngleFactory& instance();
  TorsionAngleFactory();
  TorsionAngle get(Residue& residue, const TorsionAngleName& angleName);
};

}
}
