#pragma once
#include "spans.h"
#include "selections.h"

namespace xmol::proxy {

template <typename Predicate> CoordSelection CoordSpan::filter(Predicate&& p) const {
    return CoordSelection(*m_frame, internal_filter(std::forward<Predicate>(p)));
}

template <typename Predicate> AtomSelection AtomSpan::filter(Predicate&& p) const {
  return AtomSelection(internal_filter(std::forward<Predicate>(p)));
}

template <typename Predicate> ResidueSelection ResidueSpan::filter(Predicate&& p) const {
  return ResidueSelection(internal_filter(std::forward<Predicate>(p)));
}

template <typename Predicate> MoleculeSelection MoleculeSpan::filter(Predicate&& p) const {
  return MoleculeSelection(internal_filter(std::forward<Predicate>(p)));
}

} // namespace xmol::proxy