#include "xmol/predicates/predicates.h"

using namespace xmol::predicates;

MoleculePredicate MoleculePredicate::operator&&(const MoleculePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return MoleculePredicate([pred1,pred2](MoleculeRef c)->bool{ return pred1(c) && pred2(c); });
}

MoleculePredicate MoleculePredicate::operator||(const MoleculePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return MoleculePredicate([pred1,pred2](MoleculeRef c)->bool{ return pred1(c) || pred2(c); });
}

MoleculePredicate MoleculePredicate::operator^(const MoleculePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return MoleculePredicate([pred1,pred2](MoleculeRef c)->bool{ return pred1(c) ^ pred2(c); });
}

ResiduePredicate MoleculePredicate::operator&&(const ResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ResiduePredicate([pred1,pred2](ResidueRef r)->bool{ return pred1(r.molecule()) && pred2(r); });
}

ResiduePredicate MoleculePredicate::operator||(const ResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ResiduePredicate([pred1,pred2](ResidueRef r)->bool{ return pred1(r.molecule()) || pred2(r); });
}

ResiduePredicate MoleculePredicate::operator^(const ResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ResiduePredicate([pred1,pred2](ResidueRef r)->bool{ return pred1(r.molecule()) ^ pred2(r); });
}


AtomPredicate MoleculePredicate::operator&&(const AtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](AtomRef a)->bool{ return pred1(a.residue().molecule()) && pred2(a); });
}

AtomPredicate MoleculePredicate::operator||(const AtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](AtomRef a)->bool{ return pred1(a.residue().molecule()) || pred2(a); });
}

AtomPredicate MoleculePredicate::operator^(const AtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](AtomRef a)->bool{ return pred1(a.residue().molecule()) ^ pred2(a); });
}


// ResiduePredicate


ResiduePredicate ResiduePredicate::operator&&(const MoleculePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ResiduePredicate([pred1,pred2](ResidueRef r)->bool{ return pred1(r) && pred2(r.molecule()); });
}

ResiduePredicate ResiduePredicate::operator||(const MoleculePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ResiduePredicate([pred1,pred2](ResidueRef r)->bool{ return pred1(r) || pred2(r.molecule()); });
}

ResiduePredicate ResiduePredicate::operator^(const MoleculePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ResiduePredicate([pred1,pred2](ResidueRef r)->bool{ return pred1(r) ^ pred2(r.molecule()); });
}

ResiduePredicate ResiduePredicate::operator&&(const ResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ResiduePredicate([pred1,pred2](ResidueRef r)->bool{ return pred1(r) && pred2(r); });
}

ResiduePredicate ResiduePredicate::operator||(const ResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ResiduePredicate([pred1,pred2](ResidueRef r)->bool{ return pred1(r) || pred2(r); });
}

ResiduePredicate ResiduePredicate::operator^(const ResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ResiduePredicate([pred1,pred2](ResidueRef r)->bool{ return pred1(r) ^ pred2(r); });
}


AtomPredicate ResiduePredicate::operator&&(const AtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](AtomRef a)->bool{ return pred1(a.residue()) && pred2(a); });
}

AtomPredicate ResiduePredicate::operator||(const AtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](AtomRef a)->bool{ return pred1(a.residue()) || pred2(a); });
}

AtomPredicate ResiduePredicate::operator^(const AtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](AtomRef a)->bool{ return pred1(a.residue()) ^ pred2(a); });
}


// AtomPredicate


AtomPredicate AtomPredicate::operator&&(const MoleculePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](AtomRef a)->bool{ return pred1(a) && pred2(a.residue().molecule()); });
}

AtomPredicate AtomPredicate::operator||(const MoleculePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](AtomRef a)->bool{ return pred1(a) || pred2(a.residue().molecule()); });
}

AtomPredicate AtomPredicate::operator^(const MoleculePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](AtomRef a)->bool{ return pred1(a) ^ pred2(a.residue().molecule()); });
}

AtomPredicate AtomPredicate::operator&&(const ResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](AtomRef a)->bool{ return pred1(a) && pred2(a.residue()); });
}

AtomPredicate AtomPredicate::operator||(const ResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](AtomRef a)->bool{ return pred1(a) || pred2(a.residue()); });
}

AtomPredicate AtomPredicate::operator^(const ResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](AtomRef a)->bool{ return pred1(a) ^ pred2(a.residue()); });
}


AtomPredicate AtomPredicate::operator&&(const AtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](AtomRef a)->bool{ return pred1(a) && pred2(a); });
}

AtomPredicate AtomPredicate::operator||(const AtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](AtomRef a)->bool{ return pred1(a) || pred2(a); });
}

AtomPredicate AtomPredicate::operator^(const AtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](AtomRef a)->bool{ return pred1(a) ^ pred2(a); });
}

