#include "xmol/ProteinTorsionAngleFactory.h"
#include "xmol/proxy/smart/selections.h"
#include "xmol/proxy/spans-impl.h"
#include <tuple>

using namespace xmol;
using namespace xmol::proxy;

TorsionAngleFactory& TorsionAngleFactory::instance() {
  static TorsionAngleFactory _instance;
  return _instance;
}

std::optional<TorsionAngle> TorsionAngleFactory::phi(proxy::ResidueRef& r) {
  return instance()._get(r, TorsionAngleName("phi"));
}
std::optional<TorsionAngle> TorsionAngleFactory::psi(proxy::ResidueRef& r) {
  return instance()._get(r, TorsionAngleName("psi"));
}
std::optional<TorsionAngle> TorsionAngleFactory::omega(proxy::ResidueRef& r) {
  return instance()._get(r, TorsionAngleName("omega"));
}
std::optional<TorsionAngle> TorsionAngleFactory::chi1(proxy::ResidueRef& r) {
  return instance()._get(r, TorsionAngleName("chi1"));
}
std::optional<TorsionAngle> TorsionAngleFactory::chi2(proxy::ResidueRef& r) {
  return instance()._get(r, TorsionAngleName("chi2"));
}
std::optional<TorsionAngle> TorsionAngleFactory::chi3(proxy::ResidueRef& r) {
  return instance()._get(r, TorsionAngleName("chi3"));
}
std::optional<TorsionAngle> TorsionAngleFactory::chi4(proxy::ResidueRef& r) {
  return instance()._get(r, TorsionAngleName("chi4"));
}
std::optional<TorsionAngle> TorsionAngleFactory::chi5(proxy::ResidueRef& r) {
  return instance()._get(r, TorsionAngleName("chi5"));
}

void TorsionAngleFactory::define_protein_backbone_angles(ResidueName residueName) {
  instance()._define_protein_backbone_angles(residueName);
}

void TorsionAngleFactory::_define_protein_backbone_angles(ResidueName residueName) {
  { // phi
    auto atom_refs_maker = [](ResidueRef& r) {
      auto opt_prev = r.prev();
      if (opt_prev) {
        ResidueRef prev = *opt_prev;
        auto [a, b, c, d] = std::make_tuple(prev["C"], r["N"], r["CA"], r["C"]);
        if (a && b && c && d) {
          return std::optional<four_atoms>(std::make_tuple(*a, *b, *c, *d));
        }
      }
      return std::optional<four_atoms>{};
    };
    TorsionAngle::AffectedAtomsSelector selector = [](AtomRef& /*prev_c*/, AtomRef& n, AtomRef& /*ca*/, AtomRef& c) {
      std::vector<AtomRef> result;
      for (auto&& a : n.residue().atoms()) {
        if (a.name() != AtomName("H") && a.name() != AtomName("N") && a.name() != AtomName("CA")) {
          result.push_back(a);
        }
      }
      {
        auto current = c.residue().next();
        while (current) {
          for (auto& a : current->atoms()) {
            result.push_back(a);
          }
          current = current->next();
        }
      }
      return AtomSelection(result);
    };
    bindings.emplace(std::make_pair(residueName, TorsionAngleName("phi")), std::make_pair(atom_refs_maker, selector));
  }
  { // psi
    residue_to_atoms atom_refs_maker = [](ResidueRef& r) {
      auto opt_next = r.next();
      if (opt_next) {
        ResidueRef next = *opt_next;
        auto [a, b, c, d] = std::make_tuple(r["N"], r["CA"], r["C"], next["N"]);
        if (a && b && c && d) {
          return std::optional<four_atoms>(std::make_tuple(*a, *b, *c, *d));
        }
      }
      return std::optional<four_atoms>{};
    };
    TorsionAngle::AffectedAtomsSelector selector = [](AtomRef& n, AtomRef& /*ca*/, AtomRef& c, AtomRef& /*next_n*/) {
      std::vector<AtomRef> result;
      for (auto& a : n.residue().atoms()) {
        if (a.name() == AtomName("O")) {
          result.push_back(a);
        }
      }
      auto current = c.residue().next();
      while (current) {
        for (auto& a : current->atoms()) {
          result.push_back(a);
        }
        current = current->next();
      }
      return AtomSelection(result);
    };
    bindings.emplace(std::make_pair(residueName, TorsionAngleName("psi")), std::make_pair(atom_refs_maker, selector));
  }
  { // omega
    residue_to_atoms atom_refs_maker = [](ResidueRef& r) {
      auto opt_prev = r.prev();
      if (opt_prev) {
        ResidueRef prev = *opt_prev;
        auto [a, b, c, d] = std::make_tuple(prev["CA"], prev["C"], r["N"], r["CA"]);
        if (a && b && c && d) {
          return std::optional<four_atoms>(std::make_tuple(*a, *b, *c, *d));
        }
      }
      return std::optional<four_atoms>{};
    };
    TorsionAngle::AffectedAtomsSelector selector = [](AtomRef& /*prev_ca*/, AtomRef& /*prev_c*/, AtomRef& n,
                                                      AtomRef& /*ca*/) {
      std::vector<AtomRef> result;

      for (auto& a : n.residue().atoms()) {
        if (a.name() != AtomName("N")) {
          result.push_back(a);
        }
      }

      auto current = n.residue().next();
      while (current) {
        for (auto& a : current->atoms()) {
          result.push_back(a);
        }
        current = current->next();
      }
      return AtomSelection(result);
    };
    bindings.emplace(std::make_pair(residueName, TorsionAngleName("omega")), std::make_pair(atom_refs_maker, selector));
  }
}

void TorsionAngleFactory::define_protein_side_chain_angle(xmol::ResidueName residueName,
                                                          TorsionAngleName torsionAngleName,
                                                          const std::array<AtomName, 4>& names,
                                                          const std::set<AtomName>& affected_atoms) {

  residue_to_atoms atom_refs_maker = [names](ResidueRef& r) {
    auto [a,b,c,d] = std::make_tuple(r[names[0]], r[names[1]], r[names[2]], r[names[3]]);
    if (a && b && c && d) {
      return std::optional<four_atoms>(std::make_tuple(*a, *b, *c, *d));
    }
    return std::optional<four_atoms>{};
  };
  TorsionAngle::AffectedAtomsSelector selector = [affected_atoms](AtomRef& a, AtomRef&, AtomRef&, AtomRef&) {
    return a.residue().atoms().filter(
        [&affected_atoms](const AtomRef& x) { return affected_atoms.count(x.name()) == 1; });
  };
  instance().bindings.emplace(std::make_pair(residueName, torsionAngleName), std::make_pair(atom_refs_maker, selector));
}

std::optional<TorsionAngle> TorsionAngleFactory::get(proxy::ResidueRef& residue, const TorsionAngleName& angleName) {
  return instance()._get(residue, angleName);
}

std::optional<TorsionAngle> TorsionAngleFactory::_get(proxy::ResidueRef& r, const TorsionAngleName& angle_name) {
  auto it = bindings.find(std::make_pair(r.name(), angle_name));
  if (it == bindings.end()) {
    return std::optional<TorsionAngle>{};
  }
  auto& funcs = it->second;
  auto atoms = funcs.first(r);
  if (!atoms) {
    return std::optional<TorsionAngle>{};
  }
  return std::optional<TorsionAngle>(
      TorsionAngle(std::get<0>(*atoms), std::get<1>(*atoms), std::get<2>(*atoms), std::get<3>(*atoms), funcs.second));
}