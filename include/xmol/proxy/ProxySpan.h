#pragma once
#include "xmol/future/span.h"
#include <cassert>
#include <cstddef>
#include <iterator>
#include <optional>
#include <vector>

namespace xmol::proxy {

/// Indicates span split
class SpanSplitError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

template <typename Proxy, typename T> class ProxySpan {
public:
  class Iterator {
  public:
    using difference_type = ptrdiff_t;
    using value_type = Proxy;
    using reference = Proxy&;
    using pointer = Proxy*;
    using iterator_category = std::input_iterator_tag;

    Iterator() = default;
    Iterator(const Iterator&) = default;
    Iterator& operator=(const Iterator&) = default;
    Iterator(T* ptr, T* end) : p(ptr, end){};
    Proxy* operator->() { return &p; };
    Proxy& operator*() { return p; };
    Iterator& operator++() {
      p.advance();
      return *this;
    }
    Iterator operator++(int) {
      auto copy = *this;
      p.advance();
      return copy;
    }; // postfix increment
    //    Iterator& operator--() { p.advance(-1); return *this; }
    //    Iterator operator--(int) { auto copy = *this; p.advance(-1); return copy; }; //postfix decrement

    bool operator!=(const Iterator& other) const { return p != other.p; }
    bool operator==(const Iterator& other) const { return p == other.p; }

  private:
    Proxy p;
  };
  ProxySpan() = default;
  explicit ProxySpan(const future::Span<T>& span) : m_begin(span.m_begin), m_end(span.m_end){};
  ProxySpan(T* b, T* e) : m_begin(b), m_end(e){};
  ProxySpan(T* b, size_t n) : m_begin(b), m_end(b + n){};

  [[nodiscard]] size_t size() const { return m_end - m_begin; };
  [[nodiscard]] size_t empty() const { return m_end == m_begin; };

  [[nodiscard]] Proxy operator[](size_t i) {
    assert(i < size());
    return Proxy(*(m_begin + i));
  }

  [[nodiscard]] Iterator begin() { return Iterator(m_begin, m_end); }
  [[nodiscard]] Iterator end() { return Iterator(m_end, m_end); }

protected:
  inline void rebase(T* from, T* to) {
    m_begin = to + (m_begin - from);
    m_end = to + (m_end - from);
  }

  T* m_begin = nullptr;
  T* m_end = nullptr;

  /// Precondition: rhs and this must point to same allocation
  void intersect(const ProxySpan& rhs) {
    assert(rhs.m_begin < rhs.m_end);
    assert(m_begin < m_end);
    m_begin = std::max(rhs.m_begin, m_begin);
    m_end = std::min(rhs.m_end, m_end);
  }

  template <typename Predicate>[[nodiscard]] std::vector<Proxy> internal_filter(Predicate&& p) {
    std::vector<Proxy> result;
    for (auto& x : *this) {
      if (p(x)) {
        result.push_back(x);
      }
    }
    return result;
  }

  [[nodiscard]] future::Span<T> slice_impl(std::optional<size_t> start, std::optional<size_t> stop) {
    if (!stop || stop > size()) {
      stop = size();
    }
    if (!start) {
      start = 0;
    }
    return future::Span(m_begin + *start, m_begin + *stop);
  }

  [[nodiscard]] std::vector<Proxy> slice_impl(std::optional<size_t> start, std::optional<size_t> stop,
                                              std::optional<size_t> step) {
    if (!stop || stop > size()) {
      stop = size();
    }
    if (!start) {
      start = 0;
    }
    if (!step) {
      step = 1;
    }
    std::vector<Proxy> result;
    for (auto it = m_begin + *start; it < m_begin + *stop; it += *step) {
      result.emplace_back(Proxy(*it));
    }
    return result;
  }
};

} // namespace xmol::proxy