#pragma once
#include "../base.h"
#include "ProxySpan.h"

namespace xmol::v1::proxy {

class AtomRefSpan : public ProxySpan<AtomRef, BaseAtom> {
public:
  using ProxySpan::ProxySpan;
  ResidueRefSpan residues();
  MoleculeRefSpan molecules();

  [[nodiscard]] bool contains(const AtomRef& ref) const;

  template <typename Predicate> AtomSelection filter(Predicate&& p);

private:
  friend smart::AtomSmartSpan;
};

class ResidueRefSpan : public ProxySpan<ResidueRef, BaseResidue> {
public:
  using ProxySpan::ProxySpan;
  AtomRefSpan atoms();
  MoleculeRefSpan molecules();

  [[nodiscard]] bool contains(const ResidueRef& ref) const;

  template <typename Predicate> ResidueSelection filter(Predicate&& p);

private:
  friend smart::ResidueSmartSpan;
};

class MoleculeRefSpan : public ProxySpan<MoleculeRef, BaseMolecule> {
public:
  using ProxySpan::ProxySpan;
  AtomRefSpan atoms();
  ResidueRefSpan residues();

  [[nodiscard]] bool contains(const MoleculeRef& ref) const;

  template <typename Predicate> MoleculeSelection filter(Predicate&& p);

private:
  friend smart::MoleculeSmartSpan;
};

} // namespace xmol::v1::proxy