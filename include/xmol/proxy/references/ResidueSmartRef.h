#pragma once

#include "AtomRef.h"
#include "MoleculeRef.h"
#include "ResidueRef.h"
#include "xmol/proxy/FrameObserver.h"

namespace xmol::proxy {

/// Smart Residue reference proxy
class ResidueSmartRef : public FrameObserver<ResidueSmartRef> {
public:
  ResidueSmartRef(ResidueRef res);

  [[nodiscard]] constexpr ResidueId id() const {
    check_invariants("id");
    return m_ref.id();
  }

  [[maybe_unused]] constexpr const ResidueRef& id(const ResidueId& value) const& {
    check_invariants("id");
    return m_ref.id(value);
  }

  [[maybe_unused]] constexpr ResidueRef id(const ResidueId& value) const&& {
    check_invariants("id");
    return m_ref.id(value);
  }

  [[maybe_unused]] constexpr const ResidueRef& id(const ResidueIdSerial& value) const& {
    check_invariants("id");
    return m_ref.id(value);
  }

  [[maybe_unused]] constexpr ResidueRef id(const ResidueIdSerial& value) const&& {
    check_invariants("id");
    return m_ref.id(value);
  }

  [[nodiscard]] constexpr ResidueName name() const {
    check_invariants("name");
    return m_ref.name();
  }

  template <int N>[[maybe_unused]] constexpr const ResidueRef& name(const char (&value)[N]) const& {
    check_invariants("name");
    return m_ref.name(value);
  }

  template <int N>[[maybe_unused]] constexpr ResidueRef name(const char (&value)[N]) const&& {
    check_invariants("name");
    return m_ref.name(value);
  }

  [[maybe_unused]] const ResidueRef& name(const std::string& value) const& {
    check_invariants("name");
    return m_ref.name(value);
  }

  [[maybe_unused]] ResidueRef name(const std::string& value) const&& {
    check_invariants("name");
    return m_ref.name(value);
  }

  [[maybe_unused]] constexpr const ResidueRef& name(const ResidueName& value) const& {
    check_invariants("name");
    return m_ref.name(value);
  }

  [[maybe_unused]] constexpr ResidueRef name(const ResidueName& value) const&& {
    check_invariants("name");
    return m_ref.name(value);
  }

  [[nodiscard]] ResidueIndex index() const {
    check_invariants("index");
    return m_ref.index();
  }

  [[nodiscard]] constexpr bool empty() const {
    check_invariants("empty");
    return m_ref.empty();
  }

  [[nodiscard]] constexpr size_t size() const {
    check_invariants("size");
    return m_ref.size();
  }

  [[nodiscard]] constexpr MoleculeRef molecule() const {
    check_invariants("molecule");
    return m_ref.molecule();
  }

  [[nodiscard]] constexpr Frame& frame() const {
    check_invariants("frame");
    return m_ref.frame();
  }

  /// Atoms of the residue
  [[nodiscard]] AtomSpan atoms() const {
    check_invariants("atoms");
    return m_ref.atoms();
  }

  /// Atoms coordinates of the residue
  [[nodiscard]] CoordSpan coords() const {
    check_invariants("coords()");
    return m_ref.coords();
  }

  /// Next residue in the molecule
  [[nodiscard]] std::optional<ResidueRef> next() const {
    check_invariants("next");
    return m_ref.next();
  }

  /// Previous residue in the molecule
  [[nodiscard]] std::optional<ResidueRef> prev() const {
    check_invariants("prev");
    return m_ref.prev();
  }

  /// Get children atom by name
  [[nodiscard]] std::optional<AtomRef> operator[](const AtomName& name) const {
    check_invariants("operator[]");
    return m_ref[name];
  }

  /// Get children atom by name
  [[nodiscard]] std::optional<AtomRef> operator[](const char* name) const {
    check_invariants("operator[]");
    return m_ref[name];
  }

  /// Get children atom by name
  [[nodiscard]] std::optional<AtomRef> operator[](const std::string& name) const {
    check_invariants("operator[]");
    return m_ref[name];
  }

  /// @brief Adds atom to the end of the reside and return its reference
  ///
  /// Invalidates all kinds of non-smart atom references including proxy::ResidueRef, proxy::ResidueRefSpan and
  /// proxy::ResidueSelection
  ///
  /// Appropriate Frame::reserve_atoms() call prevents references invalidation
  [[nodiscard]] AtomRef add_atom() const {
    check_invariants("add_atom");
    return m_ref.add_atom();
  }

  operator ResidueRef&() {
    check_invariants("operator ResidueRef&");
    return m_ref;
  };

  operator ResidueRef() const {
    check_invariants("operator ResidueRef&");
    return m_ref;
  };

  /// Check if references point to same data
  [[nodiscard]] constexpr bool operator!=(const ResidueSmartRef& rhs) const {
    check_invariants("operator!=");
    return m_ref != rhs.m_ref;
  }
  [[nodiscard]] constexpr bool operator==(const ResidueSmartRef& rhs) const {
    check_invariants("operator==");
    return m_ref == rhs.m_ref;
  }

private:
  ResidueRef m_ref;

  void on_base_residues_move(BaseResidue* from_begin, BaseResidue* from_end, BaseResidue* to_begin);

  constexpr void check_invariants(const char* func_name) const {
    if (!is_bound_to_frame()) {
      throw DeadFrameAccessError(std::string("ResidueSmartRef::") + func_name);
    }
  }
  friend Frame;
};
} // namespace xmol::proxy