#pragma once
#include <cstdlib>
#include "xmol/v1/future/span.h"

namespace xmol::v1::proxy {

template <typename Proxy, typename T> class ProxySpan {
public:
  constexpr ProxySpan() = default;
  constexpr explicit ProxySpan(const future::Span<T>& span);
  constexpr ProxySpan(T* b, T* e);
  constexpr ProxySpan(T* b, size_t n);

  [[nodiscard]] constexpr size_t size() const;;
  [[nodiscard]] size_t empty() const;

  Proxy begin();
  Proxy end();

protected:
  void rebase(T* from, T* to);

private:
  T* m_begin = nullptr;
  T* m_end = nullptr;
};

} // namespace xmol::v1