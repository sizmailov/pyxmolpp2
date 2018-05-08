#include "xmol/geometry/Transformation3d.h"
#include "xmol/geometry/basic.h"
#include "xmol/polymer/TorsionAngle.h"
#include "xmol/polymer/exceptions.h"
#include <utility>

using namespace xmol::polymer;
using namespace xmol::geometry;

TorsionAngle::TorsionAngle(xmol::polymer::Atom& a, xmol::polymer::Atom& b, xmol::polymer::Atom& c,
                           xmol::polymer::Atom& d)
    : a(a), b(b), c(c), d(d) {}

TorsionAngle::TorsionAngle(xmol::polymer::Atom& a, xmol::polymer::Atom& b, xmol::polymer::Atom& c,
                           xmol::polymer::Atom& d, TorsionAngle::AffectedAtomsSelector rotatedAtomsGetter)
    : a(a), b(b), c(c), d(d), rotatedAtomsGetter(std::move(rotatedAtomsGetter)) {
  auto atoms = this->rotatedAtomsGetter(a, b, c, d);
  if (atoms.count(a)) {
    throw PolymerException("TorsionAngle: `a` atom MUST NOT be in rotated atoms");
  }
  if (!atoms.count(d)) {
    throw PolymerException("TorsionAngle: `d` atom MUST be in rotated atoms");
  }
}

AngleValue TorsionAngle::value() const {
  return dihedral_angle(static_cast<Atom&>(a).r(), static_cast<Atom&>(b).r(), static_cast<Atom&>(c).r(),
                        static_cast<Atom&>(d).r());
}

void TorsionAngle::set(const xmol::geometry::AngleValue& value, xmol::geometry::AngleValue noop_tolerance) {
  if (!rotatedAtomsGetter) {
    throw PolymerException("TorsionAngle: affected AtomSelection are not set");
  }
  auto current = this->value();
  auto delta = (value - current).to_standard_range();
  if (fabs(delta) > noop_tolerance) {
    auto& a = static_cast<Atom&>(this->a);
    auto& b = static_cast<Atom&>(this->b);
    auto& c = static_cast<Atom&>(this->c);
    auto& d = static_cast<Atom&>(this->d);

    auto rot = geometry::Rotation3d(c.r() - b.r(), delta);
    auto transl = geometry::Translation3d(c.r() - rot.transform(c.r()));
    auto transf = geometry::Transformation3d(rot, transl);

    auto atoms = rotatedAtomsGetter(a, b, c, d);
    if (atoms.count(a)) {
      throw PolymerException("TorsionAngle: `a` atom MUST NOT be in affected AtomSelection");
    }
    if (!atoms.count(d)) {
      throw PolymerException("TorsionAngle: `d` atom MUST be in affected AtomSelection");
    }
    atoms.for_each([&transf](Atom& a) { a.set_r(transf.transform(a.r())); });
  }
}

TorsionAngleFactory& TorsionAngleFactory::instance() {
  static TorsionAngleFactory _instance;
  return _instance;
}

TorsionAngle TorsionAngleFactory::phi(xmol::polymer::Residue& r) {
  return instance().get(r, TorsionAngleName("phi"));
}
TorsionAngle TorsionAngleFactory::psi(xmol::polymer::Residue& r) {
  return instance().get(r, TorsionAngleName("psi"));
}
TorsionAngle TorsionAngleFactory::omega(xmol::polymer::Residue& r) {
  return instance().get(r, TorsionAngleName("omega"));
}
TorsionAngle TorsionAngleFactory::chi1(xmol::polymer::Residue& r) {
  return instance().get(r, TorsionAngleName("chi1"));
}
TorsionAngle TorsionAngleFactory::chi2(xmol::polymer::Residue& r) {
  return instance().get(r, TorsionAngleName("chi2"));
}
TorsionAngle TorsionAngleFactory::chi3(xmol::polymer::Residue& r) {
  return instance().get(r, TorsionAngleName("chi3"));
}
TorsionAngle TorsionAngleFactory::chi4(xmol::polymer::Residue& r) {
  return instance().get(r, TorsionAngleName("chi4"));
}
TorsionAngle TorsionAngleFactory::chi5(xmol::polymer::Residue& r) {
  return instance().get(r, TorsionAngleName("chi5"));
}
TorsionAngle TorsionAngleFactory::get(Residue& r, const TorsionAngleName& angle_name) {
  auto it = bindings.find(std::make_pair(r.name(), angle_name));
  if (it == bindings.end()) {
    throw PolymerException("Torsion angle `" + angle_name.str() + "` is not defined for residue `" + r.name().str() + "`");
  }
  auto& funcs = it->second;
  auto atoms = funcs.first(r);
  return TorsionAngle(std::get<0>(atoms), std::get<1>(atoms), std::get<2>(atoms), std::get<3>(atoms), funcs.second);
}
