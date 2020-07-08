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
  constexpr ResidueId() : ResidueId(0, ResidueInsertionCode("")) {}
  constexpr explicit ResidueId(residueSerial_t serial) : serial(serial) {}
  constexpr ResidueId(residueSerial_t serial, ResidueInsertionCode iCode) : serial(serial), iCode(std::move(iCode)) {}

  constexpr ResidueId(const ResidueId& other) = default;
  constexpr ResidueId(ResidueId&& other) = default;

  constexpr ResidueId& operator=(const ResidueId& other) = default;
  constexpr ResidueId& operator=(ResidueId&& other) = default;
  constexpr ResidueId& operator=(const residueSerial_t& id) {
    this->serial = id;
    iCode = ResidueInsertionCode{};
    return *this;
  };

  constexpr bool operator<(const ResidueId& other) const {
    return std::tie(serial, iCode) < std::tie(other.serial, other.iCode);
  }
  constexpr bool operator>(const ResidueId& other) const {
    return std::tie(serial, iCode) > std::tie(other.serial, other.iCode);
  }
  constexpr bool operator==(const ResidueId& other) const {
    return std::tie(serial, iCode) == std::tie(other.serial, other.iCode);
  }
  constexpr bool operator!=(const ResidueId& other) const {
    return std::tie(serial, iCode) != std::tie(other.serial, other.iCode);
  }
  constexpr bool operator<=(const ResidueId& other) const {
    return std::tie(serial, iCode) <= std::tie(other.serial, other.iCode);
  }
  constexpr bool operator>=(const ResidueId& other) const {
    return std::tie(serial, iCode) >= std::tie(other.serial, other.iCode);
  }

  residueSerial_t serial;
  ResidueInsertionCode iCode;
};

inline std::string to_string(const ResidueId& residueId) {
  return std::to_string(residueId.serial) + residueId.iCode.str();
}

constexpr bool operator<(const ResidueId& lhs, const residueSerial_t& rhs) { return lhs < ResidueId(rhs); }
constexpr bool operator>(const ResidueId& lhs, const residueSerial_t& rhs) { return lhs > ResidueId(rhs); }
constexpr bool operator==(const ResidueId& lhs, const residueSerial_t& rhs) { return lhs == ResidueId(rhs); }
constexpr bool operator!=(const ResidueId& lhs, const residueSerial_t& rhs) { return lhs != ResidueId(rhs); }
constexpr bool operator<=(const ResidueId& lhs, const residueSerial_t& rhs) { return lhs <= ResidueId(rhs); }
constexpr bool operator>=(const ResidueId& lhs, const residueSerial_t& rhs) { return lhs >= ResidueId(rhs); }

constexpr bool operator<(const residueSerial_t& lhs, const ResidueId& rhs) { return ResidueId(lhs) < rhs; }
constexpr bool operator>(const residueSerial_t& lhs, const ResidueId& rhs) { return ResidueId(lhs) > rhs; }
constexpr bool operator==(const residueSerial_t& lhs, const ResidueId& rhs) { return ResidueId(lhs) == rhs; }
constexpr bool operator!=(const residueSerial_t& lhs, const ResidueId& rhs) { return ResidueId(lhs) != rhs; }
constexpr bool operator<=(const residueSerial_t& lhs, const ResidueId& rhs) { return ResidueId(lhs) <= rhs; }
constexpr bool operator>=(const residueSerial_t& lhs, const ResidueId& rhs) { return ResidueId(lhs) >= rhs; }

} // namespace xmol