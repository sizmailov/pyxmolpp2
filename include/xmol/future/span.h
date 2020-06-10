#pragma once
#include <cstdlib>

/// Backported C++20 features
namespace xmol::future {

/// Analogue of C++20 std::span
template <typename T> struct Span {
  constexpr Span() {}
  constexpr Span(T* b, T* e) : m_begin(b), m_end(e) {}
  constexpr Span(T* b, size_t n) : m_begin(b), m_end(b + n) {}
  T* m_begin = nullptr;
  T* m_end = nullptr;
  constexpr size_t size() const { return m_end - m_begin; };
  size_t empty() const { return m_end == m_begin; }
  T* begin() const { return m_begin; };
  T* end() const { return m_end; };
  T& operator[](size_t i) { return m_begin[i]; };
  const T& operator[](size_t i) const { return m_begin[i]; };

  void rebase(const T* from, T* to) {
    m_begin = to + (m_begin - from);
    m_end = to + (m_end - from);
  }
};
} // namespace xmol::future