#include "xmol/polymer/predicates.h"

using namespace xmol::polymer;



ChainPredicate ChainPredicate::operator&&(const ChainPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ChainPredicate([pred1,pred2](const Chain& c)->bool{ return pred1(c) && pred2(c); });
}

ChainPredicate ChainPredicate::operator||(const ChainPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ChainPredicate([pred1,pred2](const Chain& c)->bool{ return pred1(c) || pred2(c); });
}

ChainPredicate ChainPredicate::operator^(const ChainPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ChainPredicate([pred1,pred2](const Chain& c)->bool{ return pred1(c) ^ pred2(c); });
}

ResiduePredicate ChainPredicate::operator&&(const ResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ResiduePredicate([pred1,pred2](const Residue& r)->bool{ return pred1(r.chain()) && pred2(r); });
}

ResiduePredicate ChainPredicate::operator||(const ResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ResiduePredicate([pred1,pred2](const Residue& r)->bool{ return pred1(r.chain()) || pred2(r); });
}

ResiduePredicate ChainPredicate::operator^(const ResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ResiduePredicate([pred1,pred2](const Residue& r)->bool{ return pred1(r.chain()) ^ pred2(r); });
}


AtomPredicate ChainPredicate::operator&&(const AtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a.residue().chain()) && pred2(a); });
}

AtomPredicate ChainPredicate::operator||(const AtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a.residue().chain()) || pred2(a); });
}

AtomPredicate ChainPredicate::operator^(const AtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a.residue().chain()) ^ pred2(a); });
}


// ResiduePredicate


ResiduePredicate ResiduePredicate::operator&&(const ChainPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ResiduePredicate([pred1,pred2](const Residue& r)->bool{ return pred1(r) && pred2(r.chain()); });
}

ResiduePredicate ResiduePredicate::operator||(const ChainPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ResiduePredicate([pred1,pred2](const Residue& r)->bool{ return pred1(r) || pred2(r.chain()); });
}

ResiduePredicate ResiduePredicate::operator^(const ChainPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ResiduePredicate([pred1,pred2](const Residue& r)->bool{ return pred1(r) ^ pred2(r.chain()); });
}

ResiduePredicate ResiduePredicate::operator&&(const ResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ResiduePredicate([pred1,pred2](const Residue& r)->bool{ return pred1(r) && pred2(r); });
}

ResiduePredicate ResiduePredicate::operator||(const ResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ResiduePredicate([pred1,pred2](const Residue& r)->bool{ return pred1(r) || pred2(r); });
}

ResiduePredicate ResiduePredicate::operator^(const ResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return ResiduePredicate([pred1,pred2](const Residue& r)->bool{ return pred1(r) ^ pred2(r); });
}


AtomPredicate ResiduePredicate::operator&&(const AtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a.residue()) && pred2(a); });
}

AtomPredicate ResiduePredicate::operator||(const AtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a.residue()) || pred2(a); });
}

AtomPredicate ResiduePredicate::operator^(const AtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a.residue()) ^ pred2(a); });
}


// AtomPredicate


AtomPredicate AtomPredicate::operator&&(const ChainPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a) && pred2(a.residue().chain()); });
}

AtomPredicate AtomPredicate::operator||(const ChainPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a) || pred2(a.residue().chain()); });
}

AtomPredicate AtomPredicate::operator^(const ChainPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a) ^ pred2(a.residue().chain()); });
}

AtomPredicate AtomPredicate::operator&&(const ResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a) && pred2(a.residue()); });
}

AtomPredicate AtomPredicate::operator||(const ResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a) || pred2(a.residue()); });
}

AtomPredicate AtomPredicate::operator^(const ResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a) ^ pred2(a.residue()); });
}


AtomPredicate AtomPredicate::operator&&(const AtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a) && pred2(a); });
}

AtomPredicate AtomPredicate::operator||(const AtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a) || pred2(a); });
}

AtomPredicate AtomPredicate::operator^(const AtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return AtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a) ^ pred2(a); });
}

