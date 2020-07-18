#include "xmol/proxy/references/ResidueRef.h"
#include "xmol/Frame.h"
#include "xmol/proxy/references/ResidueSmartRef.h"

using namespace xmol::proxy;

template <typename Derived, typename RRef> xmol::ResidueIndex api::ConstResidueAPI<Derived, RRef>::index() const {
  derived().check_invariants("index");
  return frame().index_of(*derived().res_ptr());
}

template <typename Derived, typename RRef> AtomRef api::ResidueAPI<Derived, RRef>::add_atom() const {
  derived().check_invariants("add_atom");
  return proxy::AtomRef(frame().add_atom(*derived().res_ptr()));
}

template <typename Derived, typename RRef>
std::optional<AtomRef> api::ResidueAPI<Derived, RRef>::operator[](const xmol::AtomName& name) const {
  derived().check_invariants("operator[]");
  for (auto& a : atoms()) {
    if (a.name() == name) {
      return a;
    }
  }
  return {};
}

template <typename Derived, typename RRef>
std::optional<AtomRef> api::ResidueAPI<Derived, RRef>::operator[](const char* name) const {
  return operator[](AtomName(name));
}

template <typename Derived, typename RRef>
std::optional<AtomRef> api::ResidueAPI<Derived, RRef>::operator[](const std::string& name) const {
  return operator[](AtomName(name));
}

template <typename Derived, typename RRef>
std::optional<ConstAtomRef> api::ConstResidueAPI<Derived, RRef>::operator[](const xmol::AtomName& name) const {
  throw std::runtime_error("not implemented");
  derived().check_invariants("operator[]");
  /*
  for (auto& a : atoms()) {
    if (a.name() == name) {
      return a;
    }
  }
  return {};
  */
}

template <typename Derived, typename RRef>
std::optional<ConstAtomRef> api::ConstResidueAPI<Derived, RRef>::operator[](const char* name) const {
  return operator[](AtomName(name));
}

template <typename Derived, typename RRef>
std::optional<ConstAtomRef> api::ConstResidueAPI<Derived, RRef>::operator[](const std::string& name) const {
  return operator[](AtomName(name));
}

template class api::ConstResidueAPI<ResidueRef>;
template class api::ConstResidueAPI<ResidueSmartRef>;
template class api::ResidueAPI<ResidueRef>;
template class api::ResidueAPI<ResidueSmartRef>;
