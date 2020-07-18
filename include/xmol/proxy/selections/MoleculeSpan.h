#pragma once

#include "ResidueSpan.h"

namespace xmol::proxy {

class MoleculeSpan : public ProxySpan<MoleculeRef, BaseMolecule> {
public:
  using ProxySpan::ProxySpan;

  [[nodiscard]] CoordSpan coords();
  [[nodiscard]] AtomSpan atoms();
  [[nodiscard]] ResidueSpan residues();

  [[nodiscard]] bool contains(const MoleculeRef& ref) const;

  template <typename Predicate>[[nodiscard]] MoleculeSelection filter(Predicate&& p) const;
  [[nodiscard]] std::vector<MoleculeIndex> index() const;

  [[nodiscard]] MoleculeSelection slice(std::optional<size_t> start, std::optional<size_t> stop,
                                        std::optional<size_t> step) const;
  [[nodiscard]] MoleculeSpan slice(std::optional<size_t> start, std::optional<size_t> stop) const;

  /// Make smart span from this
  [[nodiscard]] smart::MoleculeSmartSpan smart() const;

  /// Intersect inplace
  MoleculeSpan& operator&=(const MoleculeSpan& rhs);

protected:
  [[nodiscard]] Frame* frame_ptr() { return empty() ? nullptr : m_begin->frame; }
  const Frame* frame_ptr() const { return empty() ? nullptr : m_begin->frame; }

private:
  friend smart::MoleculeSmartSpan;
};

AtomSelection operator|(const AtomSpan& lhs, const AtomSpan& rhs);
AtomSelection operator-(const AtomSpan& lhs, const AtomSpan& rhs);
AtomSpan operator&(const AtomSpan& lhs, const AtomSpan& rhs);

ResidueSelection operator|(const ResidueSpan& lhs, const ResidueSpan& rhs);
ResidueSelection operator-(const ResidueSpan& lhs, const ResidueSpan& rhs);
ResidueSpan operator&(const ResidueSpan& lhs, const ResidueSpan& rhs);

MoleculeSelection operator|(const MoleculeSpan& lhs, const MoleculeSpan& rhs);
MoleculeSelection operator-(const MoleculeSpan& lhs, const MoleculeSpan& rhs);
MoleculeSpan operator&(const MoleculeSpan& lhs, const MoleculeSpan& rhs);

} // namespace xmol::proxy