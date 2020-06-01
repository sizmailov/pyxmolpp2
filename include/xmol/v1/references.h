#pragma once
#include "FrameObserver.h"

namespace xmol {
namespace v1 {

class AtomRef : public FrameObserver<AtomRef> {
public:
  AtomName name() const {
    assert(m_frame);
    assert(m_atom_info);
    return m_atom_info->m_name;
  }
  void name(const AtomName& name) {
    assert(m_frame);
    assert(m_atom_info);
    m_atom_info->m_name = name;
  }

private:
  AtomRef(Frame& frame, detail::AtomInfo& atom_info, XYZ& coords)
      : FrameObserver<AtomRef>(frame), m_coords(&coords), m_atom_info(&atom_info) {}
  friend Frame;
  XYZ* m_coords;
  detail::AtomInfo* m_atom_info;

  void on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin);
  void on_atom_info_move(detail::AtomInfo* from_begin, detail::AtomInfo* from_end, detail::AtomInfo* to_begin);
};

class ResidueRef : public FrameObserver<ResidueRef> {
public:
  AtomRef add_atom(const AtomName& atomName, const AtomId& atomId);

  ResidueName name() const {
    assert(m_frame);
    assert(m_residue_info);
    return m_residue_info->m_name;
  }
  void name(const ResidueName& name) {
    assert(m_frame);
    assert(m_residue_info);
    m_residue_info->m_name = name;
  }
  bool empty() const {
    assert(m_frame);
    assert(m_residue_info);
    return m_residue_info->m_atoms_info.m_begin == m_residue_info->m_atoms_info.m_end;
  }
  size_t size() const {
    assert(m_frame);
    assert(m_residue_info);
    return m_residue_info->m_atoms_info.m_end - m_residue_info->m_atoms_info.m_begin;
  }

private:
  ResidueRef(Frame& frame, detail::ResidueInfo& residue_info)
      : FrameObserver<ResidueRef>(frame), m_residue_info(&residue_info) {}
  friend Frame;
  detail::ResidueInfo* m_residue_info;
  void on_residue_info_move(detail::ResidueInfo* from_begin, detail::ResidueInfo* from_end,
                            detail::ResidueInfo* to_begin);
};

class MoleculeRef : public FrameObserver<MoleculeRef> {
public:
  MoleculeRef(Frame& frame, detail::MoleculeInfo& molecule_info)
      : FrameObserver<MoleculeRef>(frame), m_molecule_info(&molecule_info) {}

  [[nodiscard]] ChainName name() const;

  void name(const ChainName& name);

  [[nodiscard]] bool empty() const;

  [[nodiscard]] size_t size() const;

  ResidueRef add_residue(const ResidueName& residueName, const ResidueId& residueId);

private:
  friend Frame;
  detail::MoleculeInfo* m_molecule_info;
  void on_molecule_info_move(detail::MoleculeInfo* from_begin, detail::MoleculeInfo* from_end,
                             detail::MoleculeInfo* to_begin);
};
} // namespace v1
} // namespace xmol