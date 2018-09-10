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

TorsionAngle::AffectedAtomsSelector make_atom_selector(const std::vector<std::string>& names) {
  std::set<AtomName> names_set;
  for (auto& name : names) {
    names_set.insert(AtomName(name));
  }
  return [names_set](Atom& /*a*/, Atom& /*b*/, Atom& /*c*/, Atom& d) -> AtomSelection {
    return d.residue().asAtoms().filter([&](const Atom& x) { return names_set.count(x.name()) != 0; });
  };
};
}

TorsionAngleFactory::TorsionAngleFactory() {
  bindings = std::map<KeyType, ValueType>{
      {make_key("ARG", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), make_atom_selector({"HB2", "HB3", "CG", "HG2", "HG3", "CD", "HD2", "HD3", "NE", "HE", "CZ", "NH1", "HH11", "HH12", "NH2", "HH21", "HH22"})}},
      {make_key("ASN", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), make_atom_selector({"HB2", "HB3", "CG", "OD1", "ND2", "HD21", "HD22"})}},
      {make_key("ASP", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), make_atom_selector({"HB2", "HB3", "CG", "OD1", "OD2"})}},
      {make_key("ASH", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), make_atom_selector({"HB2", "HB3", "CG", "OD1","HD1", "OD2"})}},
      {make_key("CYS", "chi1"), {make_atoms_refs("N", "CA", "CB", "SG"), make_atom_selector({"HB2", "HB3", "SG", "HG"})}},
      {make_key("CYM", "chi1"), {make_atoms_refs("N", "CA", "CB", "SG"), make_atom_selector({"HB2", "HB3", "SG"})}},
      {make_key("CYX", "chi1"), {make_atoms_refs("N", "CA", "CB", "SG"), make_atom_selector({"HB2", "HB3", "SG"})}},
      {make_key("GLN", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), make_atom_selector({"HB2", "HB3", "CG", "HG2", "HG3", "CD", "OE1", "NE2", "HE21", "HE22"})}},
      {make_key("GLU", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), make_atom_selector({"HB2", "HB3", "CG", "HG2", "HG3", "CD", "OE1", "OE2"})}},
      {make_key("GLH", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), make_atom_selector({"HB2", "HB3", "CG", "HG2", "HG3", "CD", "OE1", "OE2","HE1"})}},
      {make_key("HIS", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), make_atom_selector({"HB2", "HB3", "CG", "ND1", "HD1", "CE1", "HE1", "NE2", "HE2", "CD2", "HD2"})}},
      {make_key("HID", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), make_atom_selector({"HB2", "HB3", "CG", "ND1", "HD1", "CE1", "HE1", "NE2", "HE2", "CD2"})}},
      {make_key("HIP", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), make_atom_selector({"HB2", "HB3", "CG", "ND1", "HD1", "CE1", "HE1", "NE2", "HE2", "CD2", "HD2"})}},
      {make_key("HIE", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), make_atom_selector({"HB2", "HB3", "CG", "ND1", "CE1", "HE1", "NE2", "HE2", "CD2", "HD2"})}},
      {make_key("ILE", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG1"), make_atom_selector({"HB", "CG2", "HG21", "HG22", "HG23", "CG1", "HG12", "HG13", "CD1", "HD11", "HD12", "HD13"})}},
      {make_key("LEU", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), make_atom_selector({"HB2", "HB3", "CG", "HG", "CD1", "HD11", "HD12", "HD13", "CD2", "HD21", "HD22", "HD23"})}},
      {make_key("LYS", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), make_atom_selector({"HB2", "HB3", "CG", "HG2", "HG3", "CD", "HD2", "HD3", "CE", "HE2", "HE3", "NZ", "HZ1", "HZ2", "HZ3"})}},
      {make_key("MET", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), make_atom_selector({"HB2", "HB3", "CG", "HG2", "HG3", "SD", "CE", "HE1", "HE2", "HE3"})}},
      {make_key("PHE", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), make_atom_selector({"HB2", "HB3", "CG", "CD1", "HD1", "CE1", "HE1", "CZ", "HZ", "CE2", "HE2", "CD2", "HD2"})}},
      {make_key("PRO", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), nullptr}},
      {make_key("SER", "chi1"), {make_atoms_refs("N", "CA", "CB", "OG"), make_atom_selector({"HB2", "HB3", "OG", "HG"})}},
      {make_key("THR", "chi1"), {make_atoms_refs("N", "CA", "CB", "OG1"), make_atom_selector({"HB", "CG2", "HG21", "HG22", "HG23", "OG1", "HG1"})}},
      {make_key("TRP", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), make_atom_selector({"HB2", "HB3", "CG", "CD1", "HD1", "NE1", "HE1", "CE2", "CZ2", "HZ2", "CH2", "HH2", "CZ3", "HZ3", "CE3", "HE3", "CD2"})}},
      {make_key("TYR", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG"), make_atom_selector({"HB2", "HB3", "CG", "CD1", "HD1", "CE1", "HE1", "CZ", "OH", "HH", "CE2", "HE2", "CD2", "HD2"})}},
      {make_key("VAL", "chi1"), {make_atoms_refs("N", "CA", "CB", "CG1"), make_atom_selector({"HB", "CG1", "HG11", "HG12", "HG13", "CG2", "HG21", "HG22", "HG23"})}},

      {make_key("ARG", "chi2"), {make_atoms_refs("CA","CB","CG","CD"), make_atom_selector({"HG2", "HG3", "CD", "HD2", "HD3", "NE", "HE", "CZ", "NH1", "HH11", "HH12", "NH2", "HH21", "HH22"})}},
      {make_key("ASN", "chi2"), {make_atoms_refs("CA","CB","CG","OD1"), make_atom_selector({"OD1", "ND2", "HD21", "HD22"})}},
      {make_key("ASP", "chi2"), {make_atoms_refs("CA","CB","CG","OD1"), make_atom_selector({"OD1", "OD2"})}},
      {make_key("ASH", "chi2"), {make_atoms_refs("CA","CB","CG","OD1"), make_atom_selector({"OD1","HD1", "OD2"})}},
      {make_key("GLN", "chi2"), {make_atoms_refs("CA","CB","CG","CD"), make_atom_selector({"HG2", "HG3", "CD", "OE1", "NE2", "HE21", "HE22"})}},
      {make_key("GLU", "chi2"), {make_atoms_refs("CA","CB","CG","CD"), make_atom_selector({"HG2", "HG3", "CD", "OE1", "OE2"})}},
      {make_key("GLH", "chi2"), {make_atoms_refs("CA","CB","CG","CD"), make_atom_selector({"HG2", "HG3", "CD", "OE1", "OE2","HE1"})}},
      {make_key("HIS", "chi2"), {make_atoms_refs("CA","CB","CG","ND1"), make_atom_selector({"ND1", "HD1", "CE1", "HE1", "NE2", "HE2", "CD2", "HD2"})}},
      {make_key("HID", "chi2"), {make_atoms_refs("CA","CB","CG","ND1"), make_atom_selector({"ND1", "HD1", "CE1", "HE1", "NE2", "HE2", "CD2"})}},
      {make_key("HIP", "chi2"), {make_atoms_refs("CA","CB","CG","ND1"), make_atom_selector({"ND1", "HD1", "CE1", "HE1", "NE2", "HE2", "CD2", "HD2"})}},
      {make_key("HIE", "chi2"), {make_atoms_refs("CA","CB","CG","ND1"), make_atom_selector({"ND1", "CE1", "HE1", "NE2", "HE2", "CD2", "HD2"})}},
      {make_key("ILE", "chi2"), {make_atoms_refs("CA","CB","CG1","CD"), make_atom_selector({"HG22", "HG23", "CG1", "HG12", "HG13", "CD1", "HD11", "HD12", "HD13"})}},
      {make_key("LEU", "chi2"), {make_atoms_refs("CA","CB","CG","CD1"), make_atom_selector({"HG", "CD1", "HD11", "HD12", "HD13", "CD2", "HD21", "HD22", "HD23"})}},
      {make_key("LYS", "chi2"), {make_atoms_refs("CA","CB","CG","CD"), make_atom_selector({"HG2", "HG3", "CD", "HD2", "HD3", "CE", "HE2", "HE3", "NZ", "HZ1", "HZ2", "HZ3"})}},
      {make_key("MET", "chi2"), {make_atoms_refs("CA","CB","CG","SD"), make_atom_selector({"HG2", "HG3", "SD", "CE", "HE1", "HE2", "HE3"})}},
      {make_key("PHE", "chi2"), {make_atoms_refs("CA","CB","CG","CD1"), make_atom_selector({"CD1", "HD1", "CE1", "HE1", "CZ", "HZ", "CE2", "HE2", "CD2", "HD2"})}},
      {make_key("PRO", "chi2"), {make_atoms_refs("CA","CB","CG","CD"), nullptr}},
      {make_key("TRP", "chi2"), {make_atoms_refs("CA","CB","CG","CD1"), make_atom_selector({"CD1", "HD1", "NE1", "HE1", "CE2", "CZ2", "HZ2", "CH2", "HH2", "CZ3", "HZ3", "CE3", "HE3", "CD2"})}},
      {make_key("TYR", "chi2"), {make_atoms_refs("CA","CB","CG","CD1"), make_atom_selector({"CD1", "HD1", "CE1", "HE1", "CZ", "OH", "HH", "CE2", "HE2", "CD2", "HD2"})}},

      {make_key("ARG", "chi3"), {make_atoms_refs("CB","CG","CD","NE"), make_atom_selector({ "HD2", "HD3", "NE", "HE", "CZ", "NH1", "HH11", "HH12", "NH2", "HH21", "HH22"})}},
      {make_key("GLN", "chi3"), {make_atoms_refs("CB","CG","CD","OE1"), make_atom_selector({ "OE1", "NE2", "HE21", "HE22"})}},
      {make_key("GLU", "chi3"), {make_atoms_refs("CB","CG","CD","OE1"), make_atom_selector({ "OE1", "OE2"})}},
      {make_key("GLH", "chi3"), {make_atoms_refs("CB","CG","CD","OE1"), make_atom_selector({ "CD", "OE1", "OE2","HE1"})}},
      {make_key("LYS", "chi3"), {make_atoms_refs("CB","CG","CD","CE"), make_atom_selector({ "HD2", "HD3", "CE", "HE2", "HE3", "NZ", "HZ1", "HZ2", "HZ3"})}},
      {make_key("MET", "chi3"), {make_atoms_refs("CB","CG","SD","CE"), make_atom_selector({"CE", "HE1", "HE2", "HE3"})}},

      {make_key("ARG", "chi4"), {make_atoms_refs("CG","CD","NE","CZ"), make_atom_selector({"HE", "CZ", "NH1", "HH11", "HH12", "NH2", "HH21", "HH22"})}},
      {make_key("LYS", "chi4"), {make_atoms_refs("CG","CD","CE","NZ"), make_atom_selector({"HE2", "HE3", "NZ", "HZ1", "HZ2", "HZ3"})}},

      {make_key("ARG", "chi5"), {make_atoms_refs("CD","NE","CZ","NH1"), make_atom_selector({"NH1", "HH11", "HH12", "NH2", "HH21", "HH22"})}},
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
