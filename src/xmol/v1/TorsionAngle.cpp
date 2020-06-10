#include "xmol/TorsionAngle.h"
#include "xmol/geom/affine/Transformation3d.h"
#include "xmol/proxy/smart/selections.h"

using namespace xmol;
using namespace xmol::geom;
using namespace xmol::geom::affine;

TorsionAngle::TorsionAngle(TorsionAngle::ARef a, TorsionAngle::ARef b, TorsionAngle::ARef c, TorsionAngle::ARef d)
    : a(a), b(b), c(c), d(d) {}

TorsionAngle::TorsionAngle(TorsionAngle::ARef a, TorsionAngle::ARef b, TorsionAngle::ARef c, TorsionAngle::ARef d,
                           TorsionAngle::AffectedAtomsSelector affected_atoms_selector)
    : a(a), b(b), c(c), d(d), m_affected_atoms(std::move(affected_atoms_selector)) {
  if (this->m_affected_atoms) {
    auto atoms = m_affected_atoms(a, b, c, d);
    if (atoms.contains(a)) {
      throw GeomError("TorsionAngle: `a` atom MUST NOT be in rotated atoms");
    }
    if (!atoms.contains(d)) {
      throw GeomError("TorsionAngle: `d` atom MUST be in rotated atoms");
    }
  }
}

AngleValue TorsionAngle::value() const { return dihedral_angle(a.r(), b.r(), c.r(), d.r()); }

void TorsionAngle::set(const xmol::geom::AngleValue& value, xmol::geom::AngleValue noop_tolerance) {
  if (!m_affected_atoms) {
    throw GeomError("TorsionAngle: affected AtomSelection are not set");
  }
  auto current = this->value();
  auto delta = (value - current).to_standard_range();
  if (fabs(delta) > noop_tolerance) {

    auto rot = Rotation3d(c.r() - b.r(), delta);
    auto transl = Translation3d(c.r() - rot.transform(c.r()));
    auto transf = Transformation3d(rot, transl);

    auto atoms = m_affected_atoms(a, b, c, d);
    if (atoms.contains(a)) {
      throw GeomError("TorsionAngle: `a` atom MUST NOT be in affected AtomSelection");
    }
    if (!atoms.contains(d)) {
      throw GeomError("TorsionAngle: `d` atom MUST be in affected AtomSelection");
    }
    for (auto& atom : atoms) {
      atom.r(transf.transform(atom.r()));
    }
  }
}
