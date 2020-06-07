#pragma once
#include "../future/span.h"
#include <cstddef>
#include <iterator>
#include <vector>

namespace xmol::v1::proxy {

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

    bool operator!=(const Iterator& other) { return p != other.p; }
    bool operator==(const Iterator& other) { return p == other.p; }

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

protected:
  T* m_begin = nullptr;
  T* m_end = nullptr;

  template <typename Predicate>[[nodiscard]] std::vector<Proxy> internal_filter(Predicate&& p) {
    std::vector<Proxy> result;
    for (auto& x : *this) { // todo: change to "const auto&" when const references arrive
      if (p(x)) {
        result.push_back(x);
      }
    }
    return result;
  }
};

} // namespace xmol::v1::proxy