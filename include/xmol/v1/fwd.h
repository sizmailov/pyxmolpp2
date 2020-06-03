#pragma once
#include "xmol/polymer/Atom_fwd.h"
#include "xmol/utils/ShortAsciiString.h"

namespace xmol {
namespace v1 {

namespace detail {
struct AtomNameTag {};
struct ResidueNameTag {};
struct ChainNameTag {};
} // namespace detail

using AtomId = int32_t;
using AtomName = xmol::utils::ShortAsciiString<4, false, detail::AtomNameTag>;
using ResidueName = xmol::utils::ShortAsciiString<3, false, detail::ResidueNameTag>;
using MoleculeName = xmol::utils::ShortAsciiString<1, false, detail::ChainNameTag>;
using ResidueId = xmol::polymer::ResidueId;
using XYZ = xmol::geometry::XYZ;

/// data holders
struct BaseAtom;
struct BaseResidue;
struct BaseMolecule;

/// lightweight proxies
namespace proxy {
class AtomRef;
class ResidueRef;
class MoleculeRef;

//class AtomSelection;
//class ResidueSelection;
//class MoleculeSelection;

class AtomRefSpan;
class ResidueRefSpan;
class MoleculeRefSpan;

namespace smart {
/// ref counting proxies
class AtomSmartRef;
class ResidueSmartRef;
class MoleculeSmartRef;

} // namespace smart
} // namespace proxy

/// life holder
class Frame;

} // namespace v1
} // namespace xmol