#include "xmol/proxy/references/MoleculeRef.h"
#include "xmol/Frame.h"
#include "xmol/proxy/references/MoleculeSmartRef.h"

using namespace xmol::proxy;

template <typename Derived, typename MRef> ResidueRef api::MoleculeAPI<Derived, MRef>::add_residue() const {
  derived().check_invariants("add_residue");
  return ResidueRef(frame().add_residue(*derived().mol_ptr()));
}

template <typename Derived, typename MRef>
std::optional<ResidueRef> api::MoleculeAPI<Derived, MRef>::operator[](const xmol::ResidueId& id) const {
  derived().check_invariants("operator[]");
  for (auto& r : residues()) {
    if (r.id() == id) {
      return r;
    }
  }
  return {};
}

template <typename Derived, typename MRef>
std::optional<ResidueRef> api::MoleculeAPI<Derived, MRef>::operator[](ResidueIdSerial id) const { return (*this)[ResidueId(id)]; }


template <typename Derived, typename MRef>
std::optional<ConstResidueRef> api::ConstMoleculeAPI<Derived, MRef>::operator[](const xmol::ResidueId& id) const {
  throw std::runtime_error("not implemented");
  derived().check_invariants("operator[]");
  /*
  for (auto& r : residues()) {
    if (r.id() == id) {
      return r;
    }
  }
  return {};
  */
}

template <typename Derived, typename MRef>
std::optional<ConstResidueRef> api::ConstMoleculeAPI<Derived, MRef>::operator[](ResidueIdSerial id) const { return (*this)[ResidueId(id)]; }

template <typename Derived, typename MRef>
xmol::MoleculeIndex api::ConstMoleculeAPI<Derived, MRef>::index() const {
  derived().check_invariants("index");
  return frame().index_of(*derived().mol_ptr());
}

template class api::ConstMoleculeAPI<MoleculeRef>;
template class api::ConstMoleculeAPI<MoleculeSmartRef>;
template class api::MoleculeAPI<MoleculeRef>;
template class api::MoleculeAPI<MoleculeSmartRef>;
