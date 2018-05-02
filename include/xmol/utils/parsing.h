#pragma once

#include <cstring>
#include <gsl/gsl_assert>
#include <string>
#include <tuple>
#include <utility>

namespace xmol {
namespace utils {
namespace parsing {

constexpr static int powers_of_10[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

template <int LEN> inline std::pair<bool, int> parse_uint_fixed_length(const std::string& line, int pos) noexcept {
  bool success = GSL_LIKELY(line.size() >= pos + LEN);
  if (GSL_UNLIKELY(!success)) {
    return {false, 0};
  }
  int number = 0;
#pragma unroll
  for (int i = 0; i < LEN; i++) {
    unsigned digit = static_cast<unsigned>(line[pos + i]) - '0';
    success &= digit <= 9;
    number += digit * powers_of_10[LEN - i - 1];
  }
  return std::make_pair(success, number);
};

inline std::pair<bool, int> parse_uint(const std::string& line, int pos, int LEN) noexcept {
  switch (LEN) {
  case (1):
    return parse_uint_fixed_length<1>(line, pos);
  case (2):
    return parse_uint_fixed_length<2>(line, pos);
  case (3):
    return parse_uint_fixed_length<3>(line, pos);
  case (4):
    return parse_uint_fixed_length<4>(line, pos);
  case (5):
    return parse_uint_fixed_length<5>(line, pos);
  case (6):
    return parse_uint_fixed_length<6>(line, pos);
  case (7):
    return parse_uint_fixed_length<7>(line, pos);
  case (8):
    return parse_uint_fixed_length<8>(line, pos);
  case (9):
    return parse_uint_fixed_length<9>(line, pos);
  default:
    return {false, 0};
  }
};

enum class SpaceStrip { NONE, LEFT, RIGHT, LEFT_AND_RIGHT };

template <SpaceStrip stripping = SpaceStrip::NONE>
std::pair<bool, int> parse_int(const std::string& line, int pos, int LEN) noexcept {
  // short-circuit for invalid
  if (GSL_UNLIKELY(line.size() < pos + LEN)) {
    return {false, 0};
  }

  if (stripping == SpaceStrip::RIGHT || stripping == SpaceStrip::LEFT_AND_RIGHT) {
    while (line[pos + LEN - 1] == ' ' && LEN > 0) {
      --LEN;
    }
  }

  if (stripping == SpaceStrip::LEFT || stripping == SpaceStrip::LEFT_AND_RIGHT) {
    while (line[pos] == ' ' && LEN > 0) {
      ++pos;
      --LEN;
    }
  }

  if (line[pos] == '-') {
    bool success;
    int value;
    std::tie(success, value) = parse_uint(line, pos + 1, LEN - 1);
    return {success, -value};
  } else {
    return parse_uint(line, pos, LEN);
  }
};

template <int WIDTH, int PRECISION, SpaceStrip STRIP> struct parse_fixed_precision_fn {
  static_assert(PRECISION >= 0, "");
  static_assert(WIDTH > 0, "");
  static_assert(PRECISION == 0 || WIDTH >= PRECISION + 2, "");

  inline std::pair<bool, double> operator()(const std::string& line, int pos) const noexcept {

    if (GSL_UNLIKELY(line.size() < pos + WIDTH || (PRECISION > 0 && line[pos + WIDTH - PRECISION - 1] != '.'))) {
      return {false, 0};
    }

    int whole = WIDTH - PRECISION;
    if (PRECISION > 0) {
      --whole;
    }
    int precision = PRECISION;

    if (STRIP == SpaceStrip::LEFT_AND_RIGHT || STRIP == SpaceStrip::LEFT) {
      while (line[pos] == ' ' && whole > 0) {
        --whole;
        pos++;
      }
    }

    if (STRIP == SpaceStrip::LEFT_AND_RIGHT || STRIP == SpaceStrip::RIGHT) {
      while (line[pos + whole + precision] == ' ' && precision > 0) {
        --precision;
      }
    }

    int sign = 1;

    if (line[pos] == '-') {
      sign = -1;
      ++pos;
      --whole;
    }
    bool success;
    int whole_part;
    std::tie(success, whole_part) = parse_uint(line, pos, whole);

    if (GSL_UNLIKELY(!success)) {
      return {false, 0};
    }
    if (PRECISION == 0) {
      return {true, sign * whole_part};
    }
    bool success2;
    int fraction_part;
    std::tie(success2, fraction_part) = parse_uint(line, pos + whole + 1, precision);
    if (GSL_UNLIKELY(!success2)) {
      return {false, 0};
    }

    return {true, sign * (whole_part + double(fraction_part) / powers_of_10[precision])};
  };
};

inline namespace functional_objects {
struct parse_fixed_precision_fn___ {
  template <int WIDTH, int PRECISION, SpaceStrip STRIP = SpaceStrip::LEFT_AND_RIGHT>
  inline std::pair<bool, double> parse(const std::string& line, int pos) const {
    return parse_fixed_precision_fn<WIDTH,PRECISION,STRIP>{}(line,pos);
  }
};
constexpr parse_fixed_precision_fn___ parse_fixed_precision{};
}

std::pair<bool, double> parse_fixed_precision_rt(const std::string& line, int pos, int width) noexcept;
}
}
}