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
class Atom;
class Residue;
class Molecule;
} // namespace proxy

/// ref counting proxies
class AtomRef;
class ResidueRef;
class MoleculeRef;

/// life holder
class Frame;

} // namespace v1
} // namespace xmol