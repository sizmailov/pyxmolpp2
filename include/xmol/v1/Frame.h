#pragma once
#include "fwd.h"
#include "xmol/selection/Observable.h"
#include <vector>

namespace xmol {

namespace v1 {

namespace detail {

template <typename T> struct InfoRange {
  T* m_begin = nullptr;
  T* m_end = nullptr;
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

class AtomRef;
class ResidueRef;
class MoleculeRef;
class Frame;

template <typename Observer> class FrameObserver {
public:
  FrameObserver() = delete;
  FrameObserver(FrameObserver&& rhs) noexcept;
  FrameObserver(const FrameObserver& rhs);
  ~FrameObserver();
  FrameObserver& operator=(FrameObserver&& rhs) noexcept;
  FrameObserver& operator=(const FrameObserver& rhs);

  const Frame& frame() const { return *m_frame; }

  Frame& frame() { return *m_frame; }

protected:
  friend Frame;
  FrameObserver(Frame& frame) : m_frame(&frame) {}
  void on_frame_move(Frame& from, Frame& to);
  void on_frame_delete();
  Frame* m_frame;
};

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
  AtomRef(Frame& frame, detail::AtomInfo& residue_info, XYZ& coords)
      : FrameObserver<AtomRef>(frame), m_coords(&coords), m_atom_info(&residue_info) {}
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

class Frame : public selection::Observable<AtomRef>,
              public selection::Observable<ResidueRef>,
              public selection::Observable<MoleculeRef> {
public:
  Frame() = default;
  Frame(const Frame& other) = default;
  Frame(Frame&& other);
  Frame& operator=(const Frame& other) = default;
  Frame& operator=(Frame&& other);
  ~Frame();

  MoleculeRef add_molecule(const ChainName& name = ChainName());

private:
  ResidueRef add_residue(MoleculeRef& mol, const ResidueName& residueName, const ResidueId& residueId);
  AtomRef add_atom(ResidueRef& residue, const AtomName& atomName, const AtomId& atomId);

  friend AtomRef;
  friend ResidueRef;
  friend MoleculeRef;

  std::vector<detail::AtomInfo> m_atom_info;
  std::vector<detail::ResidueInfo> m_residue_info;
  std::vector<detail::MoleculeInfo> m_molecule_info;
  std::vector<XYZ> m_coordinates;
};

template <typename Observer>
FrameObserver<Observer>::FrameObserver(FrameObserver<Observer>&& rhs) noexcept : m_frame(rhs.m_frame) {
  static_cast<selection::Observable<Observer>*>(m_frame)->on_move(static_cast<Observer&>(rhs),
                                                                  static_cast<Observer&>(*this));
  rhs.m_frame = nullptr;
}

template <typename Observer>
FrameObserver<Observer>::FrameObserver(const FrameObserver<Observer>& rhs) : m_frame(rhs.m_frame) {
  static_cast<selection::Observable<Observer>*>(m_frame)->on_copy(static_cast<Observer&>(*this));
}

template <typename Observer> FrameObserver<Observer>::~FrameObserver() {
  if (m_frame) {
    static_cast<selection::Observable<Observer>*>(m_frame)->on_delete(static_cast<Observer&>(*this));
  }
}

template <typename Observer>
FrameObserver<Observer>& FrameObserver<Observer>::operator=(FrameObserver<Observer>&& rhs) noexcept {
  if (&rhs != this) {
    m_frame = rhs.m_frame;
    static_cast<selection::Observable<Observer>*>(m_frame)->on_move(static_cast<Observer&>(rhs),
                                                                    static_cast<Observer&>(*this));
    rhs.m_frame = nullptr;
  }
  return *this;
}

template <typename Observer> FrameObserver<Observer>& FrameObserver<Observer>::operator=(const FrameObserver& rhs) {
  if (&rhs != this) {
    m_frame = rhs.m_frame;
    static_cast<selection::Observable<Observer>*>(m_frame)->on_copy(static_cast<Observer&>(*this));
  }
  return *this;
}
template <typename Observer> void FrameObserver<Observer>::on_frame_move(Frame& from, Frame& to) {
  assert(m_frame == &from);
  m_frame = &to;
}
template <typename Observer> void FrameObserver<Observer>::on_frame_delete() { m_frame = nullptr; }
} // namespace v1

} // namespace xmol