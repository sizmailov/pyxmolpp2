#include "xmol/v1/ProteinTorsionAngleFactory.h"

using namespace xmol::v1;

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
  //  { // phi
  //    residue_to_atoms atom_refs_maker = [](Residue& r) -> std::tuple<Atom*, Atom*, Atom*, Atom*> {
  //      Residue* prev = r.prev();
  //      if (!prev) {
  //        return std::tuple<Atom*, Atom*, Atom*, Atom*>(nullptr, nullptr, nullptr, nullptr);
  //      }
  //      return std::make_tuple(&prev->operator[](AtomName("C")), &r[AtomName("N")], &r[AtomName("CA")],
  //                             &r[AtomName("C")]);
  //    };
  //    TorsionAngle::AffectedAtomsSelector selector = [](Atom& /*prev_c*/, Atom& n, Atom& /*ca*/, Atom& c) {
  //      std::vector<Atom*> result;
  //      n.residue()
  //          .asAtoms()
  //          .filter([](const Atom& a) {
  //            return a.name() != AtomName("NH") && a.name() != AtomName("N") && a.name() != AtomName("CA");
  //          })
  //          .for_each([&result](Atom& a) { result.push_back(&a); });
  //      {
  //        Residue* current = c.residue().next();
  //        while (current != nullptr) {
  //          for (auto& a : *current) {
  //            result.push_back(&a);
  //          }
  //          current = current->next();
  //        }
  //      }
  //      return AtomSelection(result.begin(), result.end(), xmol::selection::NoSortTag{});
  //    };
  //    bindings.emplace(std::make_pair(residueName, TorsionAngleName("phi")), std::make_pair(atom_refs_maker,
  //    selector));
  //  }
  //  { // psi
  //    residue_to_atoms atom_refs_maker = [](Residue& r) -> std::tuple<Atom*, Atom*, Atom*, Atom*> {
  //      Residue* next = r.next();
  //      if (!next) {
  //        return std::tuple<Atom*, Atom*, Atom*, Atom*>(nullptr, nullptr, nullptr, nullptr);
  //      }
  //      return std::make_tuple(&r[AtomName("N")], &r[AtomName("CA")], &r[AtomName("C")],
  //                             &next->operator[](AtomName("N")));
  //    };
  //    TorsionAngle::AffectedAtomsSelector selector = [](Atom& n, Atom& /*ca*/, Atom& c, Atom& /*next_n*/) {
  //      std::vector<Atom*> result;
  //
  //      for (auto& a : n.residue()) {
  //        if (a.name() == AtomName("O")) {
  //          result.push_back(&a);
  //        }
  //      }
  //      Residue* current = c.residue().next();
  //      while (current != nullptr) {
  //        for (auto& a : *current) {
  //          result.push_back(&a);
  //        }
  //        current = current->next();
  //      }
  //      return AtomSelection(result.begin(), result.end(), selection::NoSortTag{});
  //    };
  //    bindings.emplace(std::make_pair(residueName, TorsionAngleName("psi")), std::make_pair(atom_refs_maker,
  //    selector));
  //  }
  //  { // omega
  //    residue_to_atoms atom_refs_maker = [](Residue& r) -> std::tuple<Atom*, Atom*, Atom*, Atom*> {
  //      Residue* prev = r.prev();
  //      if (!prev) {
  //        return std::tuple<Atom*, Atom*, Atom*, Atom*>(nullptr, nullptr, nullptr, nullptr);
  //      }
  //      return std::make_tuple(&prev->operator[](AtomName("CA")), &prev->operator[](AtomName("C")), &r[AtomName("N")],
  //                             &r[AtomName("CA")]);
  //    };
  //    TorsionAngle::AffectedAtomsSelector selector = [](Atom& /*prev_ca*/, Atom& /*prev_c*/, Atom& n, Atom& /*ca*/) {
  //      std::vector<Atom*> result;
  //
  //      for (auto& a : n.residue()) {
  //        if (a.name() != AtomName("N")) {
  //          result.push_back(&a);
  //        }
  //      }
  //
  //      Residue* current = n.residue().next();
  //      while (current != nullptr) {
  //        for (auto& a : *current) {
  //          result.push_back(&a);
  //        }
  //        current = current->next();
  //      }
  //      return AtomSelection(result.begin(), result.end(), selection::NoSortTag{});
  //    };
  //    bindings.emplace(std::make_pair(residueName, TorsionAngleName("omega")), std::make_pair(atom_refs_maker,
  //    selector));
  //  }
}

void TorsionAngleFactory::define_protein_side_chain_angle(xmol::polymer::ResidueName residueName,
                                                          TorsionAngleName torsionAngleName,
                                                          const std::array<AtomName, 4>& names,
                                                          const std::set<AtomName>& affected_atoms) {
  //
  //  residue_to_atoms atom_refs_maker = [names](Residue& r) {
  //    return std::make_tuple(&r[names[0]], &r[names[1]], &r[names[2]], &r[names[3]]);
  //  };
  //  TorsionAngle::AffectedAtomsSelector selector = [affected_atoms](Atom& a, Atom&, Atom&, Atom&) {
  //    return a.residue().asAtoms().filter(
  //        [&affected_atoms](const Atom& x) { return affected_atoms.count(x.name()) == 1; });
  //  };
  //  instance().bindings.emplace(std::make_pair(residueName, torsionAngleName), std::make_pair(atom_refs_maker,
  //  selector));
}

std::optional<TorsionAngle> TorsionAngleFactory::get(proxy::ResidueRef& residue, const TorsionAngleName& angleName) {
  //  return instance()._get(residue, angleName);
  return {};
}

std::optional<TorsionAngle> TorsionAngleFactory::_get(proxy::ResidueRef& r, const TorsionAngleName& angle_name) {
  return {};
//  auto it = bindings.find(std::make_pair(r.name(), angle_name));
//  if (it == bindings.end()) {
//    return std::optional<TorsionAngle>{};
//  }
//  auto& funcs = it->second;
//  auto atoms = funcs.first(r);
//  if (!std::get<0>(atoms) || !std::get<1>(atoms) || !std::get<2>(atoms) || !std::get<3>(atoms)) {
//    return std::optional<TorsionAngle>{};
//  }
//  return std::optional<TorsionAngle>(
//      TorsionAngle(*std::get<0>(atoms), *std::get<1>(atoms), *std::get<2>(atoms), *std::get<3>(atoms), funcs.second));
}