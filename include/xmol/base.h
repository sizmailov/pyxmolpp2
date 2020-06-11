#pragma once
#include "future/span.h"
#include "fwd.h"
#include "geom/XYZ.h"
#include <tuple>

namespace xmol {

using XYZ = xmol::geom::XYZ;
using CoordEigenVector = XYZ::Vector_t;
using CoordEigenMatrix = Eigen::Matrix<double, 3, Eigen::Dynamic>;
using CoordEigenMatrixMap = Eigen::Map<Eigen::Matrix<double, 3, Eigen::Dynamic>>;

namespace detail {
struct AtomNameTag {};
struct InsertionCodeTag {};
struct ResidueNameTag {};
struct ChainNameTag {};
} // namespace detail
using ResidueInsertionCode = utils::ShortAsciiString<1, false, detail::InsertionCodeTag>;

using residueSerial_t = int32_t;
struct ResidueId {
  ResidueId() : ResidueId(0, ResidueInsertionCode("")) {}
  explicit ResidueId(residueSerial_t serial) : serial(serial) {}
  ResidueId(residueSerial_t serial, ResidueInsertionCode iCode) : serial(serial), iCode(std::move(iCode)) {}

  ResidueId(const ResidueId& other) = default;
  ResidueId(ResidueId&& other) = default;

  ResidueId& operator=(const ResidueId& other) = default;
  ResidueId& operator=(ResidueId&& other) = default;
  ResidueId& operator=(const residueSerial_t& serial) {
    this->serial = serial;
    iCode = ResidueInsertionCode{};
    return *this;
  };

  inline bool operator<(const ResidueId& other) const {
    return std::tie(serial, iCode) < std::tie(other.serial, other.iCode);
  }
  inline bool operator>(const ResidueId& other) const {
    return std::tie(serial, iCode) > std::tie(other.serial, other.iCode);
  }
  inline bool operator==(const ResidueId& other) const {
    return std::tie(serial, iCode) == std::tie(other.serial, other.iCode);
  }
  inline bool operator!=(const ResidueId& other) const {
    return std::tie(serial, iCode) != std::tie(other.serial, other.iCode);
  }
  inline bool operator<=(const ResidueId& other) const {
    return std::tie(serial, iCode) <= std::tie(other.serial, other.iCode);
  }
  inline bool operator>=(const ResidueId& other) const {
    return std::tie(serial, iCode) >= std::tie(other.serial, other.iCode);
  }

  residueSerial_t serial;
  ResidueInsertionCode iCode;
};

inline bool operator< (const ResidueId& lhs, const residueSerial_t& rhs) { return lhs <  ResidueId(rhs); }
inline bool operator> (const ResidueId& lhs, const residueSerial_t& rhs) { return lhs >  ResidueId(rhs); }
inline bool operator==(const ResidueId& lhs, const residueSerial_t& rhs) { return lhs == ResidueId(rhs); }
inline bool operator!=(const ResidueId& lhs, const residueSerial_t& rhs) { return lhs != ResidueId(rhs); }
inline bool operator<=(const ResidueId& lhs, const residueSerial_t& rhs) { return lhs <= ResidueId(rhs); }
inline bool operator>=(const ResidueId& lhs, const residueSerial_t& rhs) { return lhs >= ResidueId(rhs); }

inline bool operator< (const residueSerial_t& lhs, const ResidueId& rhs) { return ResidueId(lhs) <  rhs; }
inline bool operator> (const residueSerial_t& lhs, const ResidueId& rhs) { return ResidueId(lhs) >  rhs; }
inline bool operator==(const residueSerial_t& lhs, const ResidueId& rhs) { return ResidueId(lhs) == rhs; }
inline bool operator!=(const residueSerial_t& lhs, const ResidueId& rhs) { return ResidueId(lhs) != rhs; }
inline bool operator<=(const residueSerial_t& lhs, const ResidueId& rhs) { return ResidueId(lhs) <= rhs; }
inline bool operator>=(const residueSerial_t& lhs, const ResidueId& rhs) { return ResidueId(lhs) >= rhs; }

namespace detail {} // namespace detail

using AtomId = int32_t;
using MoleculeSerial = size_t;
using AtomName = xmol::utils::ShortAsciiString<4, false, detail::AtomNameTag>;
using ResidueName = xmol::utils::ShortAsciiString<3, false, detail::ResidueNameTag>;
using MoleculeName = xmol::utils::ShortAsciiString<1, false, detail::ChainNameTag>;
using ResidueInsertionCode = xmol::ResidueInsertionCode;

/// Storage of atomic data except coords
struct BaseAtom {
  AtomName name;                  /// Atom name
  AtomId id;                      /// Atom id
  BaseResidue* residue = nullptr; /// Parent residue
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