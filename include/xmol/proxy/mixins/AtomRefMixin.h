#pragma once

namespace xmol::proxy {

template <typename ARef> class AtomGettersMixin {
  using RRef = std::conditional_t<std::is_same_v<ARef,AtomRef>, ResidueRef, /*const*/ResidueRef>;
public:
  AtomGettersMixin() = default;

#define REF_GETTER(NAME, EXPR)                                                                                         \
  [[nodiscard]] auto& NAME() const {                                                                                   \
    ref().check_invariants(#NAME);                                                                                     \
    return EXPR;                                                                                                       \
  }

  REF_GETTER(r, *ref().coord_ptr());
  REF_GETTER(id, ref().atom_ptr()->id);
  REF_GETTER(mass, ref().atom_ptr()->mass);
  REF_GETTER(vdw_radius, ref().atom_ptr()->vdw_radius);
  REF_GETTER(name, ref().atom_ptr()->name);
  REF_GETTER(frame, ref().residue().frame());

#undef REF_GETTER

#define VALUE_GETTER(NAME, EXPR)                                                                                       \
  [[nodiscard]] auto NAME() const {                                                                                    \
    ref().check_invariants(#NAME);                                                                                     \
    return EXPR;                                                                                                       \
  }
  VALUE_GETTER(residue, RRef(*ref().atom_ptr()->residue));
  VALUE_GETTER(molecule, ref().residue().molecule());

#undef VALUE_GETTER

private:
  const ARef& ref() const { return static_cast<const ARef&>(*this); }
};

template <typename ARef> class AtomSettersMixin : public AtomGettersMixin<ARef> {
public:
  AtomSettersMixin() = default;

  using AtomGettersMixin<ARef>::name;
  using AtomGettersMixin<ARef>::mass;
  using AtomGettersMixin<ARef>::id;
  using AtomGettersMixin<ARef>::r;
  using AtomGettersMixin<ARef>::residue;
  using AtomGettersMixin<ARef>::molecule;
  using AtomGettersMixin<ARef>::frame;

#define PROXY_SETTER(NAME, TYPE, EXPR)                                                                                 \
  [[maybe_unused]] ARef& NAME(TYPE value)& {                                                                           \
    ref().check_invariants(#NAME);                                                                                     \
    EXPR;                                                                                                              \
    return ref();                                                                                                      \
  }                                                                                                                    \
                                                                                                                       \
  [[maybe_unused]] ARef&& NAME(TYPE value)&& {                                                                         \
    ref().check_invariants(#NAME);                                                                                     \
    EXPR;                                                                                                              \
    return std::move(ref());                                                                                           \
  }

  PROXY_SETTER(r, const XYZ&, *ref().coord_ptr() = value);
  PROXY_SETTER(id, AtomId, ref().atom_ptr()->id = value);
  PROXY_SETTER(mass, float, ref().atom_ptr()->mass = value);
  PROXY_SETTER(vdw_radius, float, ref().atom_ptr()->vdw_radius = value);
  PROXY_SETTER(name, AtomName, ref().atom_ptr()->name = value);
  PROXY_SETTER(name, const std::string&, ref().atom_ptr()->name = AtomName(value));
  PROXY_SETTER(name, const char*, ref().atom_ptr()->name = AtomName(value));

#undef PROXY_SETTER

private:
  ARef& ref() & { return static_cast<ARef&>(*this); }
  ARef&& ref() && { return static_cast<ARef&&>(*this); }
};

} // namespace xmol::proxy