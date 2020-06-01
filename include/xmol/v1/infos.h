#pragma once
#include "fwd.h"

namespace xmol {
namespace v1 {
namespace detail {

template <typename T> struct InfoRange {
  T* m_begin = nullptr;
  T* m_end = nullptr;
  constexpr size_t size() const { return m_end - m_begin; };
};

struct AtomInfo;
struct ResidueInfo;
struct MoleculeInfo;

struct AtomInfo {
  AtomName m_name;
  AtomId m_id;
  ResidueInfo* m_residue_info = nullptr;
};

using AtomInfoRange = InfoRange<AtomInfo>;

struct ResidueInfo {
  ResidueName m_name;
  ResidueId m_id;
  AtomInfoRange m_atoms_info;
  MoleculeInfo* m_molecule_info = nullptr;
};

using ResidueInfoRange = InfoRange<ResidueInfo>;

struct MoleculeInfo {
  ChainName m_name;
  ResidueInfoRange m_residues_info;
};
} // namespace detail
} // namespace v1
} // namespace xmol