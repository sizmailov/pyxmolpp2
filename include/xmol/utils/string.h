#pragma once
#include <algorithm>
#include <cctype>
#include <functional>
#include <locale>

namespace xmol {
namespace utils {
namespace string {

// trim from start
static inline std::string& ltrim(std::string& s) {
  s.erase(s.begin(),
          std::find_if(s.begin(), s.end(),
                       std::not1(std::ptr_fun<int, int>(std::isspace))));
  return s;
}

// trim from end
static inline std::string& rtrim(std::string& s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       std::not1(std::ptr_fun<int, int>(std::isspace)))
              .base(),
          s.end());
  return s;
}

// trim from both ends
static inline std::string& trim(std::string& s) { return ltrim(rtrim(s)); }

static inline std::string ltrim(std::string&& s) {
  s.erase(s.begin(),
          std::find_if(s.begin(), s.end(),
                       std::not1(std::ptr_fun<int, int>(std::isspace))));
  return std::move(s);
}

// trim from end
static inline std::string rtrim(std::string&& s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       std::not1(std::ptr_fun<int, int>(std::isspace)))
              .base(),
          s.end());
  return std::move(s);
}

// trim from both ends
static inline std::string trim(std::string&& s) {
  return std::move(ltrim(rtrim(s)));
}

template <typename I>
std::string int2hex(I w, size_t hex_len = sizeof(I) << 1) {
  static const char* digits = "0123456789ABCDEF";
  std::string rc(hex_len, '0');
  for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
    rc[i] = digits[(w >> j) & 0x0f];
  return rc;
}
} // ::xmol::utils::string
} // ::xmol::utils
} // ::xmol
