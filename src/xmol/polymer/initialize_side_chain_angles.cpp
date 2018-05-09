#include "xmol/polymer/TorsionAngle.h"
#include <utility>

using namespace xmol::polymer;
using namespace xmol::geometry;

namespace {
std::pair<ResidueName, TorsionAngleName> make_key(const char* rname, const char* angle_name) {
  return std::make_pair(ResidueName(rname), TorsionAngleName(angle_name));
};
std::function<std::tuple<Atom*, Atom*, Atom*, Atom*>(Residue&)> make_atoms_refs(const char* a, const char* b,
                                                                                const char* c, const char* d) {
  return [a, b, c, d](Residue& r) { return std::make_tuple(&r[AtomName(a)], &r[AtomName(b)], &r[AtomName(c)], &r[AtomName(d)]); };
};
template <int N>
TorsionAngle::AffectedAtomsSelector make_atom_selector(std::vector<std::string> names) {
  std::set<AtomName> names_set;
  for (auto& name : names) {
    names_set.insert(AtomName(name));
  }
  return [names_set](Atom& a, Atom& b, Atom& c, Atom& d) -> AtomSelection {
    return d.residue().asAtoms().filter([&](Atom& x) { return names_set.count(x.name()) != 0; });
  };
};
}

TorsionAngleFactory::TorsionAngleFactory() {
  bindings = std::map<KeyType, ValueType>{
      {make_key("ARG", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), nullptr}},
      {make_key("ASN", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), nullptr}},
      {make_key("ASP", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), nullptr}},
      {make_key("ASH", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), nullptr}},
      {make_key("CYS", "chi1"), {make_atoms_refs("N", "CA", "CB", "SG"), nullptr}},
      {make_key("CYM", "chi1"), {make_atoms_refs("N", "CA", "CB", "SG"), nullptr}},
      {make_key("CYX", "chi1"), {make_atoms_refs("N", "CA", "CB", "SG"), nullptr}},
      {make_key("GLN", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), nullptr}},
      {make_key("GLU", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), nullptr}},
      {make_key("GLH", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), nullptr}},
      {make_key("HIS", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), nullptr}},
      {make_key("HID", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), nullptr}},
      {make_key("HIP", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), nullptr}},
      {make_key("HIE", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), nullptr}},
      {make_key("ILE", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG1"), nullptr}},
      {make_key("LEU", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), nullptr}},
      {make_key("LYS", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), nullptr}},
      {make_key("MET", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), nullptr}},
      {make_key("PHE", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), nullptr}},
      {make_key("PRO", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), nullptr}},
      {make_key("SER", "chi1"), {make_atoms_refs("N", "CA", "CB", "OG"), nullptr}},
      {make_key("THR", "chi1"), {make_atoms_refs("N", "CA", "CB", "OG1"), nullptr}},
      {make_key("TRP", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), nullptr}},
      {make_key("TYR", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), nullptr}},
      {make_key("VAL", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG1"), nullptr}},

      {make_key("ARG", "chi2"), {make_atoms_refs("CA","CB","CG","CD"), nullptr}},
      {make_key("ASN", "chi2"), {make_atoms_refs("CA","CB","CG","OD1"), nullptr}},
      {make_key("ASP", "chi2"), {make_atoms_refs("CA","CB","CG","OD1"), nullptr}},
      {make_key("ASH", "chi2"), {make_atoms_refs("CA","CB","CG","OD1"), nullptr}},
      {make_key("GLN", "chi2"), {make_atoms_refs("CA","CB","CG","CD"), nullptr}},
      {make_key("GLU", "chi2"), {make_atoms_refs("CA","CB","CG","CD"), nullptr}},
      {make_key("GLH", "chi2"), {make_atoms_refs("CA","CB","CG","CD"), nullptr}},
      {make_key("HIS", "chi2"), {make_atoms_refs("CA","CB","CG","ND1"), nullptr}},
      {make_key("HID", "chi2"), {make_atoms_refs("CA","CB","CG","ND1"), nullptr}},
      {make_key("HIP", "chi2"), {make_atoms_refs("CA","CB","CG","ND1"), nullptr}},
      {make_key("HIE", "chi2"), {make_atoms_refs("CA","CB","CG","ND1"), nullptr}},
      {make_key("ILE", "chi2"), {make_atoms_refs("CA","CB","CG1","CD"), nullptr}},
      {make_key("LEU", "chi2"), {make_atoms_refs("CA","CB","CG","CD1"), nullptr}},
      {make_key("LYS", "chi2"), {make_atoms_refs("CA","CB","CG","CD"), nullptr}},
      {make_key("MET", "chi2"), {make_atoms_refs("CA","CB","CG","SD"), nullptr}},
      {make_key("PHE", "chi2"), {make_atoms_refs("CA","CB","CG","CD1"), nullptr}},
      {make_key("PRO", "chi2"), {make_atoms_refs("CA","CB","CG","CD"), nullptr}},
      {make_key("TRP", "chi2"), {make_atoms_refs("CA","CB","CG","CD1"), nullptr}},
      {make_key("TYR", "chi2"), {make_atoms_refs("CA","CB","CG","CD1"), nullptr}},

      {make_key("ARG", "chi3"), {make_atoms_refs("CB","CG","CD","NE"), nullptr}},
      {make_key("GLN", "chi3"), {make_atoms_refs("CB","CG","CD","OE1"), nullptr}},
      {make_key("GLU", "chi3"), {make_atoms_refs("CB","CG","CD","OE1"), nullptr}},
      {make_key("GLH", "chi3"), {make_atoms_refs("CB","CG","CD","OE1"), nullptr}},
      {make_key("LYS", "chi3"), {make_atoms_refs("CB","CG","CD","CE"), nullptr}},
      {make_key("MET", "chi3"), {make_atoms_refs("CB","CG","SD","CE"), nullptr}},

      {make_key("ARG", "chi4"), {make_atoms_refs("CG","CD","NE","CZ"), nullptr}},
      {make_key("LYS", "chi4"), {make_atoms_refs("CG","CD","CE","NZ"), nullptr}},

      {make_key("MET", "chi5"), {make_atoms_refs("CD","NE","CZ","NH1"), nullptr}},
  };
  _define_protein_backbone_angles(ResidueName("ALA"));
  _define_protein_backbone_angles(ResidueName("ARG"));
  _define_protein_backbone_angles(ResidueName("ASN"));
  _define_protein_backbone_angles(ResidueName("ASP"));
  _define_protein_backbone_angles(ResidueName("ASH"));
  _define_protein_backbone_angles(ResidueName("CYS"));
  _define_protein_backbone_angles(ResidueName("CYM"));
  _define_protein_backbone_angles(ResidueName("CYX"));
  _define_protein_backbone_angles(ResidueName("GLN"));
  _define_protein_backbone_angles(ResidueName("GLU"));
  _define_protein_backbone_angles(ResidueName("GLH"));
  _define_protein_backbone_angles(ResidueName("HIS"));
  _define_protein_backbone_angles(ResidueName("HID"));
  _define_protein_backbone_angles(ResidueName("HIP"));
  _define_protein_backbone_angles(ResidueName("HIE"));
  _define_protein_backbone_angles(ResidueName("ILE"));
  _define_protein_backbone_angles(ResidueName("LEU"));
  _define_protein_backbone_angles(ResidueName("LYS"));
  _define_protein_backbone_angles(ResidueName("MET"));
  _define_protein_backbone_angles(ResidueName("PHE"));
  _define_protein_backbone_angles(ResidueName("PRO"));
  _define_protein_backbone_angles(ResidueName("SER"));
  _define_protein_backbone_angles(ResidueName("THR"));
  _define_protein_backbone_angles(ResidueName("TRP"));
  _define_protein_backbone_angles(ResidueName("TYR"));
  _define_protein_backbone_angles(ResidueName("VAL"));
  _define_protein_backbone_angles(ResidueName("GLY"));
}
