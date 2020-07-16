#pragma once

#include "mixins/MoleculeRefMixin.h"
#include "xmol/base.h"
#include "xmol/proxy/Selection.h"
#include "xmol/proxy/spans.h"

namespace xmol::proxy {

class ConstMoleculeRef : public MoleculeGettersMixin<ConstMoleculeRef> {
public:
  constexpr ConstMoleculeRef(const ConstMoleculeRef& rhs) = default;
  constexpr ConstMoleculeRef(ConstMoleculeRef&& rhs) noexcept = default;
  constexpr ConstMoleculeRef& operator=(const ConstMoleculeRef& rhs) = default;
  constexpr ConstMoleculeRef& operator=(ConstMoleculeRef&& rhs) noexcept = default;

private:
  BaseMolecule* m_molecule = nullptr;

  constexpr void check_invariants(const char*) const {};

  constexpr BaseMolecule* const mol_ptr() const { return m_molecule; }
  constexpr BaseMolecule* const& mol_ptr() { return m_molecule; }

  constexpr explicit ConstMoleculeRef(BaseMolecule& Molecule) : m_molecule(&Molecule){};
  constexpr ConstMoleculeRef(BaseMolecule* ptr, BaseMolecule*) : m_molecule(ptr){};
  constexpr void advance() { ++m_molecule; }
  constexpr ConstMoleculeRef() = default; /// constructs object in invalid state (with nullptrs)

  friend MoleculeRef;
};

}