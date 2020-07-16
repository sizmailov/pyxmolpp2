#pragma once

#include "mixins/MoleculeRefMixin.h"
#include "xmol/base.h"
#include "xmol/proxy/Selection.h"
#include "xmol/proxy/spans.h"

namespace xmol::proxy {

class MoleculeConstRef : public MoleculeGettersMixin<MoleculeConstRef> {
public:
  constexpr MoleculeConstRef(const MoleculeConstRef& rhs) = default;
  constexpr MoleculeConstRef(MoleculeConstRef&& rhs) noexcept = default;
  constexpr MoleculeConstRef& operator=(const MoleculeConstRef& rhs) = default;
  constexpr MoleculeConstRef& operator=(MoleculeConstRef&& rhs) noexcept = default;

private:
  BaseMolecule* m_molecule = nullptr;

  constexpr void check_invariants(const char*) const {};

  constexpr BaseMolecule* const mol_ptr() const { return m_molecule; }
  constexpr BaseMolecule* const& mol_ptr() { return m_molecule; }

  constexpr explicit MoleculeConstRef(BaseMolecule& Molecule) : m_molecule(&Molecule){};
  constexpr MoleculeConstRef(BaseMolecule* ptr, BaseMolecule*) : m_molecule(ptr){};
  constexpr void advance() { ++m_molecule; }
  constexpr MoleculeConstRef() = default; /// constructs object in invalid state (with nullptrs)

  friend MoleculeRef;
};

}