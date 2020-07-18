#pragma once

#include "xmol/base.h"
#include "xmol/proxy/Selection.h"
#include "xmol/proxy/spans.h"
#include "api/MoleculeAPI.h"

namespace xmol::proxy {

class ConstMoleculeRef : public api::ConstMoleculeAPI<ConstMoleculeRef> {
private:
  BaseMolecule* m_molecule = nullptr;

  constexpr void check_invariants(const char*) const {};

  [[nodiscard]] constexpr const BaseMolecule* mol_ptr() const { return m_molecule; }

  constexpr explicit ConstMoleculeRef(BaseMolecule& Molecule) : m_molecule(&Molecule){};
  constexpr ConstMoleculeRef(BaseMolecule* ptr, BaseMolecule*) : m_molecule(ptr){};
  constexpr void advance() { ++m_molecule; }
  void rebase(BaseMolecule* from, BaseMolecule* to) { m_molecule = to + (m_molecule - from); }
  constexpr ConstMoleculeRef() = default; /// constructs object in invalid state (with nullptrs)

  friend MoleculeRef;

  template <typename> friend class proxy::api::ConstAtomAPI;
  template <typename, typename> friend class proxy::api::ConstResidueAPI;
  template <typename, typename> friend class proxy::api::ConstMoleculeAPI;
};

}