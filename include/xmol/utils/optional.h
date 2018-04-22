#pragma once

namespace xmol {
namespace utils {

template <typename T> struct optional {
  optional() : is_set(false) {}
  optional(T t) : m_value(std::move(t)), is_set(true) {}
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

  operator const T&() const { return this->value(); };
  operator T&() { return this->value(); };

  operator bool() { return is_set; };
  bool operator!() { return !is_set; };

private:
  T m_value;
  bool is_set;
};
}
}