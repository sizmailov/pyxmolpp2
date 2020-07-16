#pragma once

#include <type_traits>
#include "xmol/base.h"

namespace xmol::proxy {

template <typename MRef> class MoleculeGettersMixin {
//  using MRef = std::conditional_t<std::is_same_v<RRef, ResidueRef>, MoleculeRef, /*const*/ResidueRef>;
public:
  constexpr MoleculeGettersMixin() = default;

#define REF_GETTER(NAME, EXPR)                                                                                         \
  [[nodiscard]] constexpr auto& NAME() const {                                                                         \
    ref().check_invariants(#NAME);                                                                                     \
    return EXPR;                                                                                                       \
  }

  REF_GETTER(name, ref().mol_ptr()->name);
  REF_GETTER(frame, *ref().mol_ptr()->frame);

#undef REF_GETTER

#define VALUE_GETTER(NAME, EXPR)                                                                                       \
  [[nodiscard]] constexpr auto NAME() const {                                                                          \
    ref().check_invariants(#NAME);                                                                                     \
    return EXPR;                                                                                                       \
  }

  VALUE_GETTER(empty, ref().mol_ptr()->residues.m_begin == ref().mol_ptr()->residues.m_end);
  VALUE_GETTER(size, ref().mol_ptr()->residues.m_end - ref().mol_ptr()->residues.m_begin);

#undef VALUE_GETTER

private:
  const MRef& ref() const { return static_cast<const MRef&>(*this); }
};

template <typename MRef> class MoleculeSettersMixin : public MoleculeGettersMixin<MRef> {
public:
  constexpr MoleculeSettersMixin() = default;

  using MoleculeGettersMixin<MRef>::name;
  using MoleculeGettersMixin<MRef>::frame;

#define PROXY_SETTER(NAME, TYPE, EXPR)                                                                                 \
  [[maybe_unused]] constexpr MRef& NAME(TYPE value)& {                                                                 \
    ref().check_invariants(#NAME);                                                                                     \
    EXPR;                                                                                                              \
    return ref();                                                                                                      \
  }                                                                                                                    \
                                                                                                                       \
  [[maybe_unused]] constexpr MRef&& NAME(TYPE value)&& {                                                               \
    ref().check_invariants(#NAME);                                                                                     \
    EXPR;                                                                                                              \
    return std::move(ref());                                                                                           \
  }

  PROXY_SETTER(name, const MoleculeName&, ref().mol_ptr()->name = value);
  PROXY_SETTER(name, const std::string&, ref().mol_ptr()->name = MoleculeName(value));
  PROXY_SETTER(name, const char*, ref().mol_ptr()->name = MoleculeName(value));

#undef PROXY_SETTER

private:
  constexpr MRef& ref() & { return static_cast<MRef&>(*this); }
  constexpr MRef&& ref() && { return static_cast<MRef&&>(*this); }
};

} // namespace xmol::proxy