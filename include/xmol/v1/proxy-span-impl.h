#pragma once
#include "proxy-span.h"

template <typename Proxy, typename T>
constexpr xmol::v1::proxy::ProxySpan<Proxy, T>::ProxySpan(T* b, T* e) : m_begin(b), m_end(e) {}

template <typename Proxy, typename T>
constexpr xmol::v1::proxy::ProxySpan<Proxy, T>::ProxySpan(const xmol::v1::future::Span<T>& span)
    : m_begin(span.m_begin), m_end(span.m_end) {}

template <typename Proxy, typename T>
constexpr xmol::v1::proxy::ProxySpan<Proxy, T>::ProxySpan(T* b, size_t n) : m_begin(b), m_end(b + n) {}

template <typename Proxy, typename T> Proxy xmol::v1::proxy::ProxySpan<Proxy, T>::begin() { return m_begin; }

template <typename Proxy, typename T> Proxy xmol::v1::proxy::ProxySpan<Proxy, T>::end() { return m_end; }

template <typename Proxy, typename T> void xmol::v1::proxy::ProxySpan<Proxy, T>::rebase(T* from, T* to) {
  m_begin = to + (m_begin - from);
  m_end = to + (m_end - from);
}
template <typename Proxy, typename T> constexpr size_t xmol::v1::proxy::ProxySpan<Proxy, T>::size() const {
  return m_end - m_begin;
}
template <typename Proxy, typename T> size_t xmol::v1::proxy::ProxySpan<Proxy, T>::empty() const {
  return m_end == m_begin;
}
template <typename Proxy, typename T> Proxy xmol::v1::proxy::ProxySpan<Proxy, T>::operator[](size_t i) {
  assert(i < size());
  return Proxy(*(m_begin + i));
}
