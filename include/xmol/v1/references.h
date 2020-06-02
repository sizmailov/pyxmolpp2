#pragma once
#include "fwd.h"
#include "FrameObserver.h"

namespace xmol {
namespace v1 {

class AtomRef : public FrameObserver<AtomRef> {
public:
  AtomName name() const;
  void name(const AtomName& name);

private:
  explicit AtomRef(BaseAtom& atom);
  AtomRef(Frame& frame, BaseAtom& atom_info, XYZ& coords)
      : FrameObserver<AtomRef>(frame), m_coords(&coords), m_atom_info(&atom_info) {}
  friend Frame;
  XYZ* m_coords;
  BaseAtom* m_atom_info;

  void on_coordinates_move(XYZ* from_begin, XYZ* from_end, XYZ* to_begin);
  void on_atom_info_move(BaseAtom* from_begin, BaseAtom* from_end, BaseAtom* to_begin);
};

class ResidueRef : public FrameObserver<ResidueRef> {
public:
  AtomRef add_atom(const AtomName& atomName, const AtomId& atomId);

  ResidueName name() const;
  void name(const ResidueName& name);
  bool empty() const;
  size_t size() const;
private:
  explicit ResidueRef(BaseResidue& res);;
  ResidueRef(Frame& frame, BaseResidue& residue_info)
      : FrameObserver<ResidueRef>(frame), m_residue_info(&residue_info) {}
  friend Frame;
  BaseResidue* m_residue_info;
  void on_residue_info_move(BaseResidue* from_begin, BaseResidue* from_end,
                            BaseResidue* to_begin);
};

class MoleculeRef : public FrameObserver<MoleculeRef> {
public:
  [[nodiscard]] MoleculeName name() const;
  void name(const MoleculeName& name);

  [[nodiscard]] bool empty() const;

  [[nodiscard]] size_t size() const;

  ResidueRef add_residue(const ResidueName& residueName, const ResidueId& residueId);

private:
  explicit MoleculeRef(BaseMolecule& mol);
  MoleculeRef(Frame& frame, BaseMolecule& molecule_info)
      : FrameObserver<MoleculeRef>(frame), m_molecule_info(&molecule_info) {}
  friend Frame;
  BaseMolecule* m_molecule_info;
  void on_molecule_info_move(BaseMolecule* from_begin, BaseMolecule* from_end,
                             BaseMolecule* to_begin);
};
} // namespace v1
} // namespace xmol