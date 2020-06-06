#pragma once
#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

namespace xmol::v1::proxy {

template <typename T> class Selection {
public:
  struct LessThanComparator {
    /// Intentionally left unimplemented. Must be specialized individually
    bool operator()(const T& p1, const T& p2);
  };
  struct EqualComparator {
    bool operator()(const T& p1, const T& p2) { return !LessThanComparator{}(p1, p2) && !LessThanComparator{}(p2, p1); }
  };

  Selection() = default;
  template <typename Iterator>
  Selection(Iterator first, Iterator last, bool sorted_and_unique = false) : m_data(first, last) {
    if (!sorted_and_unique) {
      if (!std::is_sorted(m_data.begin(), m_data.end(), LessThanComparator{})) {
        std::sort(m_data.begin(), m_data.end(), LessThanComparator{});
      }
      auto it = std::unique(m_data.begin(), m_data.end(), EqualComparator{});
      m_data.erase(it, m_data.end());
    }
  };

  template <typename Container>
  explicit Selection(Container&& c, bool sorted_and_unique = false)
      : Selection(std::begin(c), std::end(c), sorted_and_unique) {}

  explicit Selection(std::vector<T>&& data, bool sorted_and_unique = false) : m_data(std::move(data)) {
    if (!sorted_and_unique) {
      if (!std::is_sorted(m_data.begin(), m_data.end(), LessThanComparator{})) {
        std::sort(m_data.begin(), m_data.end(), LessThanComparator{});
      }
      auto it = std::unique(m_data.begin(), m_data.end(), EqualComparator{});
      m_data.erase(it, m_data.end());
    }
  }

  [[nodiscard]] auto begin() {
    return m_data.begin();
  }
  [[nodiscard]] auto end() {
    return m_data.end();
  }
  [[nodiscard]] size_t size() const {
    return m_data.size();
  }
  [[nodiscard]] size_t empty() const {
    return m_data.empty();
  }

  T& operator[](int i) {
    assert(i >= 0);
    assert(i < size());
    return m_data[i];
  }

  void unite(const Selection& rhs);
  void substract(const Selection& rhs);
  void intersect(const Selection& rhs);

  bool contains(const T& proxy) const;

protected:

  template <typename Predicate>[[nodiscard]] std::vector<T> internal_filter(Predicate&& p) {
    std::vector<T> result;
    for (auto& x : *this) { // todo: change to "const auto&" when const references arrive
      if (p(x)) {
        result.push_back(x);
      }
    }
    return result;
  }
  std::vector<T> m_data;
};

} // namespace xmol::v1::proxy