#pragma once
#include "Selection.h"
#include "proxy.h"

namespace xmol::v1::proxy {

class AtomSelection;
class ResidueSelection;
class MoleculeSelection;

class AtomSelection : public Selection<AtomRef> {
public:
  ResidueRefSpan residues() {
    if (empty()) {
      return {};
    }
    return ResidueRefSpan(m_data.front().m_atom->residue, m_data.back().m_atom->residue);
  }
  MoleculeRefSpan molecules() {
    if (empty()) {
      return {};
    }
    return MoleculeRefSpan(m_data.front().m_atom->residue->molecule, m_data.back().m_atom->residue->molecule);
  }

private:
};

} // namespace xmol::v1::proxy