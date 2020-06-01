#pragma once
#include "fwd.h"

namespace xmol {
namespace v1 {
namespace detail {

template <typename T> struct Span {
  constexpr Span(){}
  constexpr Span(T* b, T* e): m_begin(b), m_end(e){}
  constexpr Span(T* b, size_t n): m_begin(b), m_end(b+n){}
  T* m_begin = nullptr;
  T* m_end = nullptr;
  constexpr size_t size() const { return m_end - m_begin; };
  size_t empty() const { return m_end == m_begin; }
  T* begin() { return m_begin; };
  T* end() { return m_end; };

  void rebase(T* from, T* to){
    m_begin = to + (m_begin - from);
    m_end = to + (m_end - from);
  }
};

struct AtomInfo;
struct ResidueInfo;
struct MoleculeInfo;

struct AtomInfo {
  AtomName m_name;
  AtomId m_id;
  ResidueInfo* m_residue_info = nullptr;
};

using AtomInfoSpan = Span<AtomInfo>;

struct ResidueInfo {
  ResidueName m_name;
  ResidueId m_id;
  AtomInfoSpan m_atoms_info;
  MoleculeInfo* m_molecule_info = nullptr;
};

using ResidueInfoSpan = Span<ResidueInfo>;

struct MoleculeInfo {
  ChainName m_name;
  ResidueInfoSpan m_residues_info;
};

using MoleculeInfoSpan = Span<MoleculeInfo>;

} // namespace detail
} // namespace v1
} // namespace xmol