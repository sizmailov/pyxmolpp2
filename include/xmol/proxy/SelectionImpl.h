#pragma once
#include "Selection.h"

namespace xmol::proxy {

template <typename T> void Selection<T>::unite(const Selection<T>& rhs) {
  auto comparator = LessThanComparator{};
  assert(std::is_sorted(m_data.begin(), m_data.end(), comparator));
  assert(std::is_sorted(rhs.m_data.begin(), rhs.m_data.end(), comparator));
  size_t selection_size = size();
  m_data.insert(m_data.end(), rhs.m_data.begin(), rhs.m_data.end());
  std::inplace_merge(m_data.begin(), m_data.begin() + selection_size, m_data.end(), comparator);
  auto end = std::unique(m_data.begin(), m_data.end(), EqualComparator{});
  m_data.erase(end, m_data.end());
}

template <typename T> void Selection<T>::substract(const Selection<T>& rhs) {
  auto comparator = LessThanComparator{};
  assert(std::is_sorted(m_data.begin(), m_data.end(), comparator));
  assert(std::is_sorted(rhs.m_data.begin(), rhs.m_data.end(), comparator));
  auto it = m_data.begin();
  auto it1 = m_data.begin();
  auto it2 = rhs.m_data.begin();
  while (it1 != m_data.end() && it2 != rhs.m_data.end()) {
    if (comparator(*it2, *it1)) { // it2 < it1
      ++it2;
    } else if (comparator(*it1, *it2)) { // it1 < it2
      *it = *it1;
      ++it;
      ++it1;
    } else { // it1 == it2;
      ++it1;
      ++it2;
    }
  }
  while (it1 != m_data.end()) {
    *it = *it1;
    ++it;
    ++it1;
  }
  m_data.erase(it, m_data.end());
  assert(std::is_sorted(m_data.begin(), m_data.end(), comparator));
}

template <typename T> void Selection<T>::intersect(const Selection<T>& rhs) {
  auto comparator = LessThanComparator{};
  static_cast<void>(comparator);
  assert(std::is_sorted(m_data.begin(), m_data.end(), comparator));
  assert(std::is_sorted(rhs.m_data.begin(), rhs.m_data.end(), comparator));

  auto end = std::set_intersection(m_data.begin(), m_data.end(), rhs.m_data.begin(), rhs.m_data.end(), m_data.begin(),
                                   comparator);
  m_data.erase(end, m_data.end());
  assert(std::is_sorted(m_data.begin(), m_data.end(), comparator));
}

template <typename T> bool Selection<T>::contains(const T& proxy) const {
  auto comparator = LessThanComparator{};
  assert(std::is_sorted(m_data.begin(), m_data.end(), comparator));
  return std::binary_search(m_data.begin(), m_data.end(), proxy, comparator);
}
} // namespace xmol::proxy