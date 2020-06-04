#pragma once
#include "spans.h"
#include "selections.h"

namespace xmol::v1::proxy {

template <typename Predicate> AtomSelection AtomRefSpan::filter(Predicate&& p) {
  return AtomSelection(internal_filter(std::forward<Predicate>(p)));
}

template <typename Predicate> ResidueSelection ResidueRefSpan::filter(Predicate&& p) {
  return ResidueSelection(internal_filter(std::forward<Predicate>(p)));
}

template <typename Predicate> MoleculeSelection MoleculeRefSpan::filter(Predicate&& p) {
  return MoleculeSelection(internal_filter(std::forward<Predicate>(p)));
}

} // namespace xmol::v1::proxy