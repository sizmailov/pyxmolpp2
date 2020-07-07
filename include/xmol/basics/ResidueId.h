#pragma once

#include "xmol/utils/ShortAsciiString.h"

#include <cstdint>
#include <stdexcept>
#include <tuple>
#include <type_traits>

namespace xmol {

namespace detail {
struct InsertionCodeTag {};
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
  ResidueId& operator=(const residueSerial_t& id) {
    this->serial = id;
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

inline std::string to_string(const ResidueId& residueId) {
  return std::to_string(residueId.serial) + residueId.iCode.str();
}

inline bool operator<(const ResidueId& lhs, const residueSerial_t& rhs) { return lhs < ResidueId(rhs); }
inline bool operator>(const ResidueId& lhs, const residueSerial_t& rhs) { return lhs > ResidueId(rhs); }
inline bool operator==(const ResidueId& lhs, const residueSerial_t& rhs) { return lhs == ResidueId(rhs); }
inline bool operator!=(const ResidueId& lhs, const residueSerial_t& rhs) { return lhs != ResidueId(rhs); }
inline bool operator<=(const ResidueId& lhs, const residueSerial_t& rhs) { return lhs <= ResidueId(rhs); }
inline bool operator>=(const ResidueId& lhs, const residueSerial_t& rhs) { return lhs >= ResidueId(rhs); }

inline bool operator<(const residueSerial_t& lhs, const ResidueId& rhs) { return ResidueId(lhs) < rhs; }
inline bool operator>(const residueSerial_t& lhs, const ResidueId& rhs) { return ResidueId(lhs) > rhs; }
inline bool operator==(const residueSerial_t& lhs, const ResidueId& rhs) { return ResidueId(lhs) == rhs; }
inline bool operator!=(const residueSerial_t& lhs, const ResidueId& rhs) { return ResidueId(lhs) != rhs; }
inline bool operator<=(const residueSerial_t& lhs, const ResidueId& rhs) { return ResidueId(lhs) <= rhs; }
inline bool operator>=(const residueSerial_t& lhs, const ResidueId& rhs) { return ResidueId(lhs) >= rhs; }

} // namespace xmol