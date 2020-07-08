#pragma once

#include <type_traits>
#include "xmol/fwd.h"

namespace xmol::proxy {

template <typename RRef> class ResidueGettersMixin {
  using MRef = std::conditional_t<std::is_same_v<RRef, ResidueRef>, MoleculeRef, /*const*/ResidueRef>;
public:
  constexpr ResidueGettersMixin() = default;

#define REF_GETTER(NAME, EXPR)                                                                                         \
  [[nodiscard]] constexpr auto& NAME() const {                                                                         \
    ref().check_invariants(#NAME);                                                                                     \
    return EXPR;                                                                                                       \
  }

  REF_GETTER(id, ref().res_ptr()->id);
  REF_GETTER(name, ref().res_ptr()->name);
  REF_GETTER(frame, ref().molecule().frame());

#undef REF_GETTER

#define VALUE_GETTER(NAME, EXPR)                                                                                       \
  [[nodiscard]] constexpr auto NAME() const {                                                                          \
    ref().check_invariants(#NAME);                                                                                     \
    return EXPR;                                                                                                       \
  }
  VALUE_GETTER(molecule, MRef(*ref().res_ptr()->molecule));
  VALUE_GETTER(empty, ref().res_ptr()->atoms.m_begin == ref().res_ptr()->atoms.m_end);
  VALUE_GETTER(size, ref().res_ptr()->atoms.m_end - ref().res_ptr()->atoms.m_begin);

#undef VALUE_GETTER

  /// Next residue in the molecule
  std::optional<RRef> next() {
    if (ref().res_ptr() + 1 < ref().res_ptr()->molecule->residues.begin() + molecule().size()) {
      return ResidueRef(*(ref().res_ptr() + 1));
    }
    return {};
  }

  /// Previous residue in the molecule
  std::optional<RRef> prev() {
    if (ref().res_ptr() >= ref().res_ptr()->molecule->residues.begin() + 1 ) {
      return ResidueRef(*(ref().res_ptr() - 1));
    }
    return {};
  }

private:
  const RRef& ref() const { return static_cast<const RRef&>(*this); }
};

template <typename RRef> class ResidueSettersMixin : public ResidueGettersMixin<RRef> {
public:
  constexpr ResidueSettersMixin() = default;

  using ResidueGettersMixin<RRef>::name;
  using ResidueGettersMixin<RRef>::id;
  using ResidueGettersMixin<RRef>::molecule;
  using ResidueGettersMixin<RRef>::frame;

#define PROXY_SETTER(NAME, TYPE, EXPR)                                                                                 \
  [[maybe_unused]] constexpr RRef& NAME(TYPE value)& {                                                                 \
    ref().check_invariants(#NAME);                                                                                     \
    EXPR;                                                                                                              \
    return ref();                                                                                                      \
  }                                                                                                                    \
                                                                                                                       \
  [[maybe_unused]] constexpr RRef&& NAME(TYPE value)&& {                                                               \
    ref().check_invariants(#NAME);                                                                                     \
    EXPR;                                                                                                              \
    return std::move(ref());                                                                                           \
  }

  PROXY_SETTER(id, ResidueId, ref().res_ptr()->id = value);
  PROXY_SETTER(id, residueSerial_t , ref().res_ptr()->id = value);
  PROXY_SETTER(name, ResidueName, ref().res_ptr()->name = value);
  PROXY_SETTER(name, const std::string&, ref().res_ptr()->name = ResidueName(value));
  PROXY_SETTER(name, const char*, ref().res_ptr()->name = ResidueName(value));

#undef PROXY_SETTER

private:
  constexpr RRef& ref() & { return static_cast<RRef&>(*this); }
  constexpr RRef&& ref() && { return static_cast<RRef&&>(*this); }
};

} // namespace xmol::proxy