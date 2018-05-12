#pragma once

#include <utility>
#include <type_traits>

namespace xmol {
namespace utils {

template <typename T> struct optional {
  using value_type = T;
  static_assert(std::is_default_constructible<T>::value,"");
  optional() : is_set(false) {}
  explicit optional(const T& t) : m_value(t), is_set(true) {}
  optional(T&& t) : m_value(std::move(t)), is_set(true) {}
  optional(optional<T>&&) = default;
  optional(const optional<T>&) = default;
  optional& operator=(optional<T>&&) = default;
  optional& operator=(const optional<T>&) = default;

  optional& operator=(T&& t) {
    m_value = std::move(t);
    is_set = true;
    return *this;
  };
  optional& operator=(const T& t) {
    m_value = t;
    is_set = true;
    return *this;
  };

  const T& value() const {
    if (is_set)
      return m_value;
    throw std::runtime_error("bad optional access");
  };
  T& value() {
    if (is_set)
      return m_value;
    throw std::runtime_error("bad optional access");
  };

  explicit operator bool() const { return is_set; };
  bool operator!() const { return !is_set; };
  bool operator<(const T& rhs) const { return value() < rhs; };
  bool operator>(const T& rhs) const { return value() > rhs; };
  bool operator<=(const T& rhs) const { return value() <= rhs; };
  bool operator>=(const T& rhs) const { return value() >= rhs; };

  const T* operator->() const{ return &m_value;}
  T* operator->() { return &m_value;}

  const T& operator*() const &{ return m_value;}
  T& operator*() & { return m_value;}

  const T& operator*() const&&{ return std::move(m_value);}
  T&& operator*() &&{ return std::move(m_value);}


private:
  T m_value;
  bool is_set;
};
}
}
