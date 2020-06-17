#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <type_traits>

#include <cassert>

namespace xmol::utils {

template <int MAX_LENGTH, bool ALLOW_CONSTRUCT_TRUNCATION = false, typename = void> class ShortAsciiString {

public:
  static_assert(MAX_LENGTH <= 8);
  using uint_type = typename std::conditional<
      MAX_LENGTH <= 4,
      typename std::conditional<MAX_LENGTH <= 2, typename std::conditional<MAX_LENGTH <= 1, uint8_t, uint16_t>::type,
                                uint32_t>::type,
      uint64_t>::type;
  static const int max_length = MAX_LENGTH;

  inline ShortAsciiString() : m_value(0){};

  explicit ShortAsciiString(const std::string& aName)
      : m_value(to_uint(aName.c_str())) {}

//  inline explicit ShortAsciiString(const char* aName)
//      : m_value(to_uint(aName)) {}

  template<int N>
  constexpr inline explicit ShortAsciiString(const char (&aName)[N]) noexcept
      : m_value(0) {
    static_assert(N<=MAX_LENGTH+1 || ALLOW_CONSTRUCT_TRUNCATION);
    const char* ptr = aName;
    for (int i = 0; *ptr != '\0' && i < MAX_LENGTH; ++i, ++ptr) {
      m_value = (uint_type(*ptr) << (8 * i)) + m_value;
    }
  }

  [[nodiscard]] constexpr char operator[](const uint8_t& i) const noexcept {
    assert(i<MAX_LENGTH);
    return m_value >> (8*i) & 255;
  }

  inline explicit ShortAsciiString(const char* aName, int exact_length)
      : m_value(to_uint(aName, exact_length)) {}

  inline ShortAsciiString(const ShortAsciiString& other)
      : m_value(other.m_value){};
  inline ShortAsciiString(ShortAsciiString&& other) noexcept
      : m_value(other.m_value) {
    other.m_value = 0;
  };

  inline ShortAsciiString& operator=(const ShortAsciiString& other) {
    m_value = other.m_value;
    return *this;
  }
  inline ShortAsciiString& operator=(ShortAsciiString&& other) noexcept {
    m_value = other.m_value;
    other.m_value = 0;
    return *this;
  }

  ShortAsciiString& operator=(const std::string& aName) {
    m_value = to_uint(aName.c_str());
    return *this;
  }
  inline ShortAsciiString& operator=(const char* aName) {
    m_value = to_uint(aName);
    return *this;
  }

  inline bool operator==(const ShortAsciiString& other) const {
    return m_value == other.m_value;
  }
  inline bool operator!=(const ShortAsciiString& other) const {
    return m_value != other.m_value;
  }
  inline bool operator==(const char* other) const {
    return m_value == ShortAsciiString(other).m_value;
  }
  inline bool operator!=(const char* other) const {
    return m_value != ShortAsciiString(other).m_value;
  }
  inline bool operator<(const ShortAsciiString& other) const {
    return m_value < other.m_value;
  }
  inline bool operator>(const ShortAsciiString& other) const {
    return m_value > other.m_value;
  }

  std::string str() const {

    uint_type tmp = m_value;

    std::string result;
    result.reserve(MAX_LENGTH);

    while (tmp > 0) {
      result.push_back(char(tmp % 256));
      tmp /= 256;
    }
    return result;
  }

  explicit operator std::string() const { return this->str(); }
  constexpr inline uint_type value() const { return m_value; }

private:
  inline uint_type static to_uint(const char* aName) {
    uint_type value = 0;
    const char* origin = aName;
    for (int i = 0; *aName != '\0' && i < MAX_LENGTH; ++i, ++aName) {
      value = (uint_type(*aName) << (8 * i)) + value;
    }
    return (ALLOW_CONSTRUCT_TRUNCATION || *aName == '\0')
               ? value
               : throw std::runtime_error("ShortName::too_long: len(`" + std::string(origin, MAX_LENGTH) + "`...) > " +
                                          std::to_string(MAX_LENGTH));
  }
  inline uint_type static to_uint(const char* aName, int length) {
    assert(length <= MAX_LENGTH);
    uint_type value = 0;
    const char* end = aName + length;
    for (int i = 0; aName != end; ++aName, ++i) {
      value = (uint_type(*aName) << (8 * i)) + value;
    }
    return value;
  }
  uint_type m_value;
};
} // namespace xmol::utils

namespace std {
template <int N, bool ALLOW_TRUNC, typename T> struct hash<xmol::utils::ShortAsciiString<N, ALLOW_TRUNC, T>> {
  using argument_type = xmol::utils::ShortAsciiString<N, ALLOW_TRUNC, T>;
  using result_type = size_t;

  result_type operator()(argument_type const& s) const { return s.value(); }
};
} // namespace std
