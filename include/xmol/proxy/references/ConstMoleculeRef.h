#pragma once

#include "xmol/base.h"
#include "xmol/proxy/Selection.h"
#include "xmol/proxy/spans.h"

namespace xmol::proxy {

class ConstMoleculeRef {
public:
  constexpr ConstMoleculeRef(const ConstMoleculeRef& rhs) = default;
  constexpr ConstMoleculeRef(ConstMoleculeRef&& rhs) noexcept = default;
  constexpr ConstMoleculeRef& operator=(const ConstMoleculeRef& rhs) = default;
  constexpr ConstMoleculeRef& operator=(ConstMoleculeRef&& rhs) noexcept = default;

  [[nodiscard]] constexpr MoleculeName name() const {
    check_invariants("name");
    return mol_ptr()->name;
  }

  [[nodiscard]] constexpr bool empty() const {
    check_invariants("empty");
    return mol_ptr()->residues.m_begin == mol_ptr()->residues.m_end;
  }

  [[nodiscard]] constexpr size_t size() const {
    check_invariants("size");
    return mol_ptr()->residues.m_end - mol_ptr()->residues.m_begin;
  }

  [[nodiscard]] constexpr const Frame& frame() const {
    check_invariants("frame");
    return *mol_ptr()->frame;
  }

  /// Index of the molecule in frame
  [[nodiscard]] MoleculeIndex index() const noexcept;

// TODO: Enable
//  /// Residues of the molecule
//  ConstResidueSpan residues() { return ConstResidueSpan{mol_ptr()->residues}; }
//  /// Atoms of the molecule
//  ConstAtomSpan atoms() {
//    if (empty())
//      return {};
//    assert(mol_ptr()->residues.m_begin);
//    assert(mol_ptr()->residues.m_end);
//    return ConstAtomSpan(mol_ptr()->residues.m_begin->atoms.m_begin,
//                    (mol_ptr()->residues.m_begin + size() - 1)->atoms.m_end);
//  }
//  /// Atom coordinates of the molecule
//  ConstCoordSpan coords() { return atoms().coords(); }
  /// Check if references point to same data
  constexpr bool operator!=(const ConstMoleculeRef& rhs) const { return m_molecule != rhs.m_molecule; }
  constexpr bool operator==(const ConstMoleculeRef& rhs) const { return m_molecule == rhs.m_molecule; }
private:
  BaseMolecule* m_molecule = nullptr;

  constexpr void check_invariants(const char*) const {};

  [[nodiscard]] constexpr const BaseMolecule* mol_ptr() const { return m_molecule; }

  constexpr explicit ConstMoleculeRef(BaseMolecule& Molecule) : m_molecule(&Molecule){};
  constexpr ConstMoleculeRef(BaseMolecule* ptr, BaseMolecule*) : m_molecule(ptr){};
  constexpr void advance() { ++m_molecule; }
  void rebase(BaseMolecule* from, BaseMolecule* to) { m_molecule = to + (m_molecule - from); }
  constexpr ConstMoleculeRef() = default; /// constructs object in invalid state (with nullptrs)

  friend ConstAtomRef;
  friend MoleculeRef;
  friend ConstResidueRef;

  template<typename>
  friend class proxy::api::ConstAtomAPI;
};

}