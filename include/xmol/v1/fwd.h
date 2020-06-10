#pragma once
#include "xmol/v1/utils/ShortAsciiString.h"
#include <stdexcept>

/// Reworked original xmol
namespace xmol::v1 {

struct BaseAtom;
struct BaseResidue;
struct BaseMolecule;

/// Proxy references to frame
namespace proxy {

class CoordRef;
class AtomRef;
class ResidueRef;
class MoleculeRef;

class CoordSelection;
class AtomSelection;
class ResidueSelection;
class MoleculeSelection;

class CoordSpan;
class AtomRefSpan;
class ResidueRefSpan;
class MoleculeRefSpan;

/// Reference counting (smart) proxies
namespace smart {

class AtomSmartRef;
class ResidueSmartRef;
class MoleculeSmartRef;

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

} // namespace xmol::v1