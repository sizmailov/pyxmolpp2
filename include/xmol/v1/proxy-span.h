#pragma once
#include "xmol/v1/future/span.h"
#include <cstdlib>

namespace xmol::v1::proxy {

template <typename Proxy, typename T> class ProxySpan {
public:
  class Iterator {
  public:
    explicit Iterator(T* ptr);
    Proxy* operator->();
    Proxy& operator*();
    Iterator operator++();
    bool operator!=(const Iterator& other);

  private:
    T* ptr;
    Proxy p;
  };
  ProxySpan() = default;
  explicit ProxySpan(const future::Span<T>& span);
  ProxySpan(T* b, T* e);
  ProxySpan(T* b, size_t n);

  [[nodiscard]] size_t size() const;
  [[nodiscard]] size_t empty() const;

  [[nodiscard]] Proxy operator[](size_t i);

  [[nodiscard]] Iterator begin();
  [[nodiscard]] Iterator end();

protected:
  void rebase(T* from, T* to);

private:
  T* m_begin = nullptr;
  T* m_end = nullptr;
};

} // namespace xmol::v1::proxy