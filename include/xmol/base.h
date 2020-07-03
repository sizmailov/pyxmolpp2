#pragma once
#include "basics/ResidueId.h"
#include "future/span.h"
#include "fwd.h"
#include "geom/XYZ.h"

namespace xmol {

using XYZ = xmol::geom::XYZ;
using CoordEigenVector = XYZ::Vector_t;
using CoordEigenMatrix = Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor>;
using CoordEigenMatrixf = Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor>;
using CoordEigenMatrixMap = Eigen::Map<CoordEigenMatrix>;
using CoordEigenMatrixMapf = Eigen::Map<CoordEigenMatrixf>;

namespace detail {
struct AtomNameTag {};
struct ResidueNameTag {};
struct ChainNameTag {};
} // namespace detail

using AtomId = int32_t;
using AtomName = xmol::utils::ShortAsciiString<4, false, detail::AtomNameTag>;
using ResidueName = xmol::utils::ShortAsciiString<3, false, detail::ResidueNameTag>;
using MoleculeName = xmol::utils::ShortAsciiString<1, false, detail::ChainNameTag>;

/// Storage of atomic data except coords
struct BaseAtom {
  AtomName name;                  /// Atom name
  AtomId id;                      /// Atom id
  BaseResidue* residue = nullptr; /// Parent residue
  float mass = 1.0;               /// Atomic mass in Daltons
  float vdw_radius = 1.0;         /// Van der Waals radius
};

/// Storage of residue data
struct BaseResidue {
  ResidueName name;                 /// Residue name
  ResidueId id;                     /// Residue id
  future::Span<BaseAtom> atoms;     /// Children atoms
  BaseMolecule* molecule = nullptr; /// Parent molecule
};

/// Storage of molecule data
struct BaseMolecule {
  Frame* frame;                       /// Parent frame
  MoleculeName name;                  /// Molecule name
  future::Span<BaseResidue> residues; /// Children residues
};

} // namespace xmol