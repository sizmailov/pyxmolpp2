#pragma once
#include <cstdlib>

namespace xmol::v1::future {

template <typename T> struct Span {
  constexpr Span() {}
  constexpr Span(T* b, T* e) : m_begin(b), m_end(e) {}
  constexpr Span(T* b, size_t n) : m_begin(b), m_end(b + n) {}
  T* m_begin = nullptr;
  T* m_end = nullptr;
  constexpr size_t size() const { return m_end - m_begin; };
  size_t empty() const { return m_end == m_begin; }
  T* begin() { return m_begin; };
  T* end() { return m_end; };

  void rebase(T* from, T* to) {
    m_begin = to + (m_begin - from);
    m_end = to + (m_end - from);
  }
};
} // namespace xmol::v1::future