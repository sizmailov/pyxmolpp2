#pragma once
#include "xmol/utils/ShortAsciiString.h"
#include <stdexcept>

/// Reworked original xmol
namespace xmol {

using Index = int;
using AtomIndex = Index;
using ResidueIndex = Index;
using MoleculeIndex = Index;
using CoordIndex = Index;

struct BaseAtom;
struct BaseResidue;
struct BaseMolecule;

/// Proxy references to frame
namespace proxy {

class CoordRef;
class ConstAtomRef;
class ConstResidueRef;
class ConstMoleculeRef;
class AtomRef;
class ResidueRef;
class MoleculeRef;

class CoordSelection;
class AtomSelection;
class ResidueSelection;
class MoleculeSelection;

class CoordSpan;
class AtomSpan;
class ResidueSpan;
class MoleculeSpan;

/// Reference counting (smart) proxies

class AtomSmartRef;
class ResidueSmartRef;
class MoleculeSmartRef;

namespace api {
template <typename Derived> class ConstAtomAPI;
template <typename Derived, typename ARef = AtomRef> class AtomAPI;
template <typename Derived, typename RRef = ConstResidueRef> class ConstResidueAPI;
template <typename Derived, typename ARef = ResidueRef> class ResidueAPI;
} // namespace references::api

namespace smart {

class CoordSmartSelection;
class AtomSmartSelection;
class ResidueSmartSelection;
class MoleculeSmartSelection;

class CoordSmartSpan;
class AtomSmartSpan;
class ResidueSmartSpan;
class MoleculeSmartSpan;

} // namespace smart
} // namespace proxy

/// life holder
class Frame;

class DeadFrameAccessError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

} // namespace xmol