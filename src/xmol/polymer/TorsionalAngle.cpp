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
  if (this->rotatedAtomsGetter) {
    auto atoms = this->rotatedAtomsGetter(a, b, c, d);
    if (atoms.count(a)) {
      throw PolymerException("TorsionAngle: `a` atom MUST NOT be in rotated atoms");
    }
    if (!atoms.count(d)) {
      throw PolymerException("TorsionAngle: `d` atom MUST be in rotated atoms");
    }
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

xmol::utils::optional<TorsionAngle> TorsionAngleFactory::phi(xmol::polymer::Residue& r) {
  return instance()._get(r, TorsionAngleName("phi"));
}
xmol::utils::optional<TorsionAngle> TorsionAngleFactory::psi(xmol::polymer::Residue& r) {
  return instance()._get(r, TorsionAngleName("psi"));
}
xmol::utils::optional<TorsionAngle> TorsionAngleFactory::omega(xmol::polymer::Residue& r) {
  return instance()._get(r, TorsionAngleName("omega"));
}
xmol::utils::optional<TorsionAngle> TorsionAngleFactory::chi1(xmol::polymer::Residue& r) {
  return instance()._get(r, TorsionAngleName("chi1"));
}
xmol::utils::optional<TorsionAngle> TorsionAngleFactory::chi2(xmol::polymer::Residue& r) {
  return instance()._get(r, TorsionAngleName("chi2"));
}
xmol::utils::optional<TorsionAngle> TorsionAngleFactory::chi3(xmol::polymer::Residue& r) {
  return instance()._get(r, TorsionAngleName("chi3"));
}
xmol::utils::optional<TorsionAngle> TorsionAngleFactory::chi4(xmol::polymer::Residue& r) {
  return instance()._get(r, TorsionAngleName("chi4"));
}
xmol::utils::optional<TorsionAngle> TorsionAngleFactory::chi5(xmol::polymer::Residue& r) {
  return instance()._get(r, TorsionAngleName("chi5"));
}

void TorsionAngleFactory::define_protein_backbone_angles(xmol::polymer::ResidueName residueName) {
  instance()._define_protein_backbone_angles(residueName);
}

void TorsionAngleFactory::_define_protein_backbone_angles(xmol::polymer::ResidueName residueName) {
  { // phi
    residue_to_atoms atom_refs_maker = [](Residue& r) -> std::tuple<Atom*, Atom*, Atom*, Atom*> {
      Residue* prev = r.prev();
      if (!prev) {
        return std::tuple<Atom*, Atom*, Atom*, Atom*>(nullptr, nullptr, nullptr, nullptr);
      }
      return std::make_tuple(&prev->operator[](AtomName("C")), &r[AtomName("N")], &r[AtomName("CA")],
                             &r[AtomName("C")]);
    };
    TorsionAngle::AffectedAtomsSelector selector = [](Atom& /*prev_c*/, Atom& n, Atom& /*ca*/, Atom& c) {
      std::vector<Atom*> result;
      n.residue().asAtoms().filter([](const Atom&a){

        return a.name()!=AtomName("NH") &&
            a.name()!=AtomName("N") &&
            a.name()!=AtomName("CA")
            ;
      }).for_each([&result](Atom&a){result.push_back(&a);});
      {
        Residue* current = c.residue().next();
        while (current != nullptr) {
          for (auto&a : *current){
            result.push_back(&a);
          }
          current = current->next();
        }
      }
      return AtomSelection(result.begin(),result.end(),xmol::selection::NoSortTag{});
    };
    bindings.emplace(std::make_pair(residueName, TorsionAngleName("phi")), std::make_pair(atom_refs_maker, selector));
  }
  { // psi
    residue_to_atoms atom_refs_maker = [](Residue& r) -> std::tuple<Atom*, Atom*, Atom*, Atom*> {
      Residue* next = r.next();
      if (!next) {
        return std::tuple<Atom*, Atom*, Atom*, Atom*>(nullptr, nullptr, nullptr, nullptr);
      }
      return std::make_tuple(&r[AtomName("N")], &r[AtomName("CA")], &r[AtomName("C")],
                             &next->operator[](AtomName("N")));
    };
    TorsionAngle::AffectedAtomsSelector selector = [](Atom& n, Atom& /*ca*/, Atom& c, Atom& /*next_n*/) {
      std::vector<Atom*> result;

      for (auto& a : n.residue()) {
        if (a.name() == AtomName("O")) {
          result.push_back(&a);
        }
      }
      Residue* current = c.residue().next();
      while (current != nullptr) {
        for (auto& a : *current) {
          result.push_back(&a);
        }
        current = current->next();
      }
      return AtomSelection(result.begin(),result.end(),selection::NoSortTag{});
    };
    bindings.emplace(std::make_pair(residueName, TorsionAngleName("psi")), std::make_pair(atom_refs_maker, selector));
  }
  { // omega
    residue_to_atoms atom_refs_maker = [](Residue& r) -> std::tuple<Atom*, Atom*, Atom*, Atom*> {
      Residue* prev = r.prev();
      if (!prev) {
        return std::tuple<Atom*, Atom*, Atom*, Atom*>(nullptr, nullptr, nullptr, nullptr);
      }
      return std::make_tuple(&prev->operator[](AtomName("CA")), &prev->operator[](AtomName("C")), &r[AtomName("N")],
                             &r[AtomName("CA")]);
    };
    TorsionAngle::AffectedAtomsSelector selector = [](Atom& /*prev_ca*/, Atom& /*prev_c*/, Atom& n, Atom& /*ca*/) {
      std::vector<Atom*> result;

      for (auto& a : n.residue()) {
        if (a.name() != AtomName("N")) {
          result.push_back(&a);
        }
      }

      Residue* current = n.residue().next();
      while (current != nullptr) {
        for (auto& a : *current) {
          result.push_back(&a);
        }
        current = current->next();
      }
      return AtomSelection(result.begin(),result.end(),selection::NoSortTag{});
    };
    bindings.emplace(std::make_pair(residueName, TorsionAngleName("omega")), std::make_pair(atom_refs_maker, selector));
  }
}

void TorsionAngleFactory::define_protein_side_chain_angle(xmol::polymer::ResidueName residueName,
                                                          xmol::polymer::TorsionAngleName torsionAngleName,
                                                          const std::array<xmol::polymer::AtomName, 4>& names,
                                                          const std::set<xmol::polymer::AtomName>& affected_atoms) {

  residue_to_atoms atom_refs_maker = [names](Residue& r) {
    return std::make_tuple(&r[names[0]], &r[names[1]], &r[names[2]], &r[names[3]]);
  };
  TorsionAngle::AffectedAtomsSelector selector = [affected_atoms](Atom& a, Atom&, Atom&, Atom&) {
    return a.residue().asAtoms().filter(
        [&affected_atoms](const Atom& x) { return affected_atoms.count(x.name()) == 1; });
  };
  instance().bindings.emplace(std::make_pair(residueName, torsionAngleName), std::make_pair(atom_refs_maker, selector));
}

xmol::utils::optional<TorsionAngle> TorsionAngleFactory::get(xmol::polymer::Residue& residue,
                                                             const xmol::polymer::TorsionAngleName& angleName) {
  return instance()._get(residue, angleName);
}

xmol::utils::optional<TorsionAngle> TorsionAngleFactory::_get(Residue& r, const TorsionAngleName& angle_name) {
  auto it = bindings.find(std::make_pair(r.name(), angle_name));
  if (it == bindings.end()) {
    return xmol::utils::optional<TorsionAngle>{};
  }
  auto& funcs = it->second;
  auto atoms = funcs.first(r);
  if (!std::get<0>(atoms) || !std::get<1>(atoms) || !std::get<2>(atoms) || !std::get<3>(atoms)) {
    return xmol::utils::optional<TorsionAngle>{};
  }
  return xmol::utils::optional<TorsionAngle>(
      TorsionAngle(*std::get<0>(atoms), *std::get<1>(atoms), *std::get<2>(atoms), *std::get<3>(atoms), funcs.second));
}
