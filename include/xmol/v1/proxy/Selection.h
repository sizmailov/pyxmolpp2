#pragma once
#include <algorithm>
#include <cassert>
#include <vector>

namespace xmol::v1 {

template <typename T> class Selection {
public:
  struct Comparator {
    /// Intentionally left unimplemented. Must be specialized individually
    bool operator()(const T& p1, const T& p2);
  };

  Selection() = default;
  template <typename Iterator> Selection(Iterator first, Iterator last) : m_data(first, last) {
    if (!std::is_sorted(m_data.begin(), m_data.end(), Comparator{})) {
      std::sort(m_data.begin(), m_data.end(), Comparator{});
    }
    auto it = std::unique(m_data.begin(), m_data.end(), Comparator{});
    m_data.erase(it, m_data.end());
  };

  template <typename Container> explicit Selection(const Container& c) : Selection(std::begin(c), std::end(c)) {}
  explicit Selection(std::vector<T>&& data) : m_data(std::move(data)) {
    if (!std::is_sorted(m_data.begin(), m_data.end(), Comparator{})) {
      std::sort(m_data.begin(), m_data.end(), Comparator{});
    }
    auto it = std::unique(m_data.begin(), m_data.end(), Comparator{});
    m_data.erase(it, m_data.end());
  }

  [[nodiscard]] auto begin() { return m_data.begin(); }
  [[nodiscard]] auto end() { return m_data.end(); }
  [[nodiscard]] size_t size() { return m_data.size(); }
  [[nodiscard]] size_t empty() { return m_data.empty(); }

  template <typename Predicate>[[nodiscard]] Selection filter(Predicate&& p) {
    Selection<T> result;
    for (auto& x : *this) { // todo: change to "const auto&" when const references arrive
      if (p(x)) {
        result.m_data.push_back(x);
      }
    }
    return result;
  }

  Selection& operator|=(const Selection& rhs);
  Selection& operator-=(const Selection& rhs);
  Selection& operator&=(const Selection& rhs);

  bool contains(const T& proxy) const;

protected:
  std::vector<T> m_data;
};

} // namespace xmol::v1