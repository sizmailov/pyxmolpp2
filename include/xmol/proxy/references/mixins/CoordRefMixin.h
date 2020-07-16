#pragma once

#include "xmol/base.h"
#include <type_traits>

namespace xmol::proxy {

template <typename CRef> class CoordGettersMixin {
public:
  constexpr CoordGettersMixin() = default;

#define REF_GETTER(NAME, EXPR)                                                                                         \
  [[nodiscard]] constexpr auto& NAME() const {                                                                         \
    ref().check_invariants(#NAME);                                                                                     \
    return EXPR;                                                                                                       \
  }
#define VALUE_GETTER(NAME, EXPR)                                                                                       \
  [[nodiscard]] constexpr auto NAME() const {                                                                          \
    ref().check_invariants(#NAME);                                                                                     \
    return EXPR;                                                                                                       \
  }

  VALUE_GETTER(x, ref().coord_ptr()->x());
  VALUE_GETTER(y, ref().coord_ptr()->y());
  VALUE_GETTER(z, ref().coord_ptr()->z());

  VALUE_GETTER(len2, ref().coord_ptr()->len2());
  VALUE_GETTER(len, ref().coord_ptr()->len());
  VALUE_GETTER(get, *ref().coord_ptr());

  REF_GETTER(_eigen, ref().coord_ptr()->_eigen());

#undef REF_GETTER
#undef VALUE_GETTER

  [[nodiscard]] XYZ cross(const XYZ& b) const { return XYZ(ref().coord_ptr()->cross(b)); }
  [[nodiscard]] XYZ dot(const XYZ& b) const { return XYZ(ref().coord_ptr()->dot(b)); }
  [[nodiscard]] double distance(const XYZ& b) const { return ref().coord_ptr()->distance(b); }
  [[nodiscard]] double distance2(const XYZ& b) const { return ref().coord_ptr()->distance2(b); }

private:
  const CRef& ref() const { return static_cast<const CRef&>(*this); }
};

template <typename CRef> class CoordSettersMixin : public CoordGettersMixin<CRef> {
public:
  constexpr CoordSettersMixin() = default;
  using CoordGettersMixin<CRef>::x;
  using CoordGettersMixin<CRef>::y;
  using CoordGettersMixin<CRef>::z;
  using CoordGettersMixin<CRef>::_eigen;

#define PROXY_SETTER(NAME, TYPE, EXPR)                                                                                 \
  [[maybe_unused]] constexpr CRef& NAME(TYPE value)& {                                                                 \
    ref().check_invariants(#NAME);                                                                                     \
    EXPR;                                                                                                              \
    return ref();                                                                                                      \
  }                                                                                                                    \
                                                                                                                       \
  [[maybe_unused]] constexpr CRef&& NAME(TYPE value)&& {                                                               \
    ref().check_invariants(#NAME);                                                                                     \
    EXPR;                                                                                                              \
    return std::move(ref());                                                                                           \
  }

  PROXY_SETTER(set, const XYZ&, *ref().coord_ptr() = value);
  PROXY_SETTER(x, double, *ref().coord_ptr().set_x(value););
  PROXY_SETTER(y, double, *ref().coord_ptr().set_y(value););
  PROXY_SETTER(z, double, *ref().coord_ptr().set_z(value););

  PROXY_SETTER(operator+=, const XYZ&, ref().coord_ptr()->operator+=(value);)
  PROXY_SETTER(operator-=, const XYZ&, ref().coord_ptr()->operator-=(value);)
  PROXY_SETTER(operator/=, double, ref().coord_ptr()->operator/=(value);)
  PROXY_SETTER(operator*=, double, ref().coord_ptr()->operator*=(value);)

#undef PROXY_SETTER

private:
  constexpr CRef& ref() & { return static_cast<CRef&>(*this); }
  constexpr CRef&& ref() && { return static_cast<CRef&&>(*this); }
};

} // namespace xmol::proxy