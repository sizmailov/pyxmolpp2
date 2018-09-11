#include "xmol/polymer/predicates.h"

using namespace xmol::polymer;



GenericChainPredicate GenericChainPredicate::operator&&(const GenericChainPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericChainPredicate([pred1,pred2](const Chain& c)->bool{ return pred1(c) && pred2(c); });
}

GenericChainPredicate GenericChainPredicate::operator||(const GenericChainPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericChainPredicate([pred1,pred2](const Chain& c)->bool{ return pred1(c) || pred2(c); });
}

GenericChainPredicate GenericChainPredicate::operator^(const GenericChainPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericChainPredicate([pred1,pred2](const Chain& c)->bool{ return pred1(c) ^ pred2(c); });
}

GenericResiduePredicate GenericChainPredicate::operator&&(const GenericResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericResiduePredicate([pred1,pred2](const Residue& r)->bool{ return pred1(r.chain()) && pred2(r); });
}

GenericResiduePredicate GenericChainPredicate::operator||(const GenericResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericResiduePredicate([pred1,pred2](const Residue& r)->bool{ return pred1(r.chain()) || pred2(r); });
}

GenericResiduePredicate GenericChainPredicate::operator^(const GenericResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericResiduePredicate([pred1,pred2](const Residue& r)->bool{ return pred1(r.chain()) ^ pred2(r); });
}


GenericAtomPredicate GenericChainPredicate::operator&&(const GenericAtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericAtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a.residue().chain()) && pred2(a); });
}

GenericAtomPredicate GenericChainPredicate::operator||(const GenericAtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericAtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a.residue().chain()) || pred2(a); });
}

GenericAtomPredicate GenericChainPredicate::operator^(const GenericAtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericAtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a.residue().chain()) ^ pred2(a); });
}


// GenericResiduePredicate


GenericResiduePredicate GenericResiduePredicate::operator&&(const GenericChainPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericResiduePredicate([pred1,pred2](const Residue& r)->bool{ return pred1(r) && pred2(r.chain()); });
}

GenericResiduePredicate GenericResiduePredicate::operator||(const GenericChainPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericResiduePredicate([pred1,pred2](const Residue& r)->bool{ return pred1(r) || pred2(r.chain()); });
}

GenericResiduePredicate GenericResiduePredicate::operator^(const GenericChainPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericResiduePredicate([pred1,pred2](const Residue& r)->bool{ return pred1(r) ^ pred2(r.chain()); });
}

GenericResiduePredicate GenericResiduePredicate::operator&&(const GenericResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericResiduePredicate([pred1,pred2](const Residue& r)->bool{ return pred1(r) && pred2(r); });
}

GenericResiduePredicate GenericResiduePredicate::operator||(const GenericResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericResiduePredicate([pred1,pred2](const Residue& r)->bool{ return pred1(r) || pred2(r); });
}

GenericResiduePredicate GenericResiduePredicate::operator^(const GenericResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericResiduePredicate([pred1,pred2](const Residue& r)->bool{ return pred1(r) ^ pred2(r); });
}


GenericAtomPredicate GenericResiduePredicate::operator&&(const GenericAtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericAtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a.residue()) && pred2(a); });
}

GenericAtomPredicate GenericResiduePredicate::operator||(const GenericAtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericAtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a.residue()) || pred2(a); });
}

GenericAtomPredicate GenericResiduePredicate::operator^(const GenericAtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericAtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a.residue()) ^ pred2(a); });
}


// GenericAtomPredicate


GenericAtomPredicate GenericAtomPredicate::operator&&(const GenericChainPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericAtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a) && pred2(a.residue().chain()); });
}

GenericAtomPredicate GenericAtomPredicate::operator||(const GenericChainPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericAtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a) || pred2(a.residue().chain()); });
}

GenericAtomPredicate GenericAtomPredicate::operator^(const GenericChainPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericAtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a) ^ pred2(a.residue().chain()); });
}

GenericAtomPredicate GenericAtomPredicate::operator&&(const GenericResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericAtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a) && pred2(a.residue()); });
}

GenericAtomPredicate GenericAtomPredicate::operator||(const GenericResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericAtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a) || pred2(a.residue()); });
}

GenericAtomPredicate GenericAtomPredicate::operator^(const GenericResiduePredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericAtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a) ^ pred2(a.residue()); });
}


GenericAtomPredicate GenericAtomPredicate::operator&&(const GenericAtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericAtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a) && pred2(a); });
}

GenericAtomPredicate GenericAtomPredicate::operator||(const GenericAtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericAtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a) || pred2(a); });
}

GenericAtomPredicate GenericAtomPredicate::operator^(const GenericAtomPredicate& rhs) const {
  auto pred1 = m_predicate;
  auto pred2 = rhs.m_predicate;
  return GenericAtomPredicate([pred1,pred2](const Atom& a)->bool{ return pred1(a) ^ pred2(a); });
}

