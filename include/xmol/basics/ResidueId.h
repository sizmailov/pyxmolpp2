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

using ResidueIdSerial = int32_t;
struct ResidueId {
  constexpr ResidueId() : ResidueId(0, ResidueInsertionCode("")) {}
  constexpr explicit ResidueId(ResidueIdSerial serial) : serial(serial) {}
  constexpr ResidueId(ResidueIdSerial serial, ResidueInsertionCode iCode) : serial(serial), iCode(std::move(iCode)) {}

  constexpr ResidueId(const ResidueId& other) = default;
  constexpr ResidueId(ResidueId&& other) = default;

  constexpr ResidueId& operator=(const ResidueId& other) = default;
  constexpr ResidueId& operator=(ResidueId&& other) = default;
  constexpr ResidueId& operator=(const ResidueIdSerial& id) {
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

  ResidueIdSerial serial;
  ResidueInsertionCode iCode;
};

inline std::string to_string(const ResidueId& residueId) {
  return std::to_string(residueId.serial) + residueId.iCode.str();
}

constexpr bool operator<(const ResidueId& lhs, const ResidueIdSerial& rhs) { return lhs < ResidueId(rhs); }
constexpr bool operator>(const ResidueId& lhs, const ResidueIdSerial& rhs) { return lhs > ResidueId(rhs); }
constexpr bool operator==(const ResidueId& lhs, const ResidueIdSerial& rhs) { return lhs == ResidueId(rhs); }
constexpr bool operator!=(const ResidueId& lhs, const ResidueIdSerial& rhs) { return lhs != ResidueId(rhs); }
constexpr bool operator<=(const ResidueId& lhs, const ResidueIdSerial& rhs) { return lhs <= ResidueId(rhs); }
constexpr bool operator>=(const ResidueId& lhs, const ResidueIdSerial& rhs) { return lhs >= ResidueId(rhs); }

constexpr bool operator<(const ResidueIdSerial& lhs, const ResidueId& rhs) { return ResidueId(lhs) < rhs; }
constexpr bool operator>(const ResidueIdSerial& lhs, const ResidueId& rhs) { return ResidueId(lhs) > rhs; }
constexpr bool operator==(const ResidueIdSerial& lhs, const ResidueId& rhs) { return ResidueId(lhs) == rhs; }
constexpr bool operator!=(const ResidueIdSerial& lhs, const ResidueId& rhs) { return ResidueId(lhs) != rhs; }
constexpr bool operator<=(const ResidueIdSerial& lhs, const ResidueId& rhs) { return ResidueId(lhs) <= rhs; }
constexpr bool operator>=(const ResidueIdSerial& lhs, const ResidueId& rhs) { return ResidueId(lhs) >= rhs; }

} // namespace xmol