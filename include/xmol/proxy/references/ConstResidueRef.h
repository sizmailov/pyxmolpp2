#pragma once

#include "ConstMoleculeRef.h"
#include "xmol/base.h"

namespace xmol::proxy {

class ConstResidueRef {
public:
  constexpr ConstResidueRef(const ConstResidueRef& rhs) = default;
  constexpr ConstResidueRef(ConstResidueRef&& rhs) noexcept = default;
  constexpr ConstResidueRef& operator=(const ConstResidueRef& rhs) = default;
  constexpr ConstResidueRef& operator=(ConstResidueRef&& rhs) noexcept = default;

  [[nodiscard]] constexpr ResidueId id() const {
    check_invariants("id");
    return res_ptr()->id;
  }

  [[nodiscard]] constexpr ResidueName name() const {
    check_invariants("name");
    return res_ptr()->name;
  }

  [[nodiscard]] ResidueIndex index() const;

  [[nodiscard]] constexpr bool empty() const {
    check_invariants("empty");
    return res_ptr()->atoms.m_begin == res_ptr()->atoms.m_end;
  }

  [[nodiscard]] constexpr size_t size() const {
    check_invariants("size");
    return res_ptr()->atoms.m_end - res_ptr()->atoms.m_begin;
  }

  [[nodiscard]] constexpr ConstMoleculeRef molecule() const {
    check_invariants("molecule");
    return ConstMoleculeRef(*res_ptr()->molecule);
  }

  [[nodiscard]] constexpr const Frame& frame() const {
    check_invariants("frame");
    return *res_ptr()->molecule->frame;
  }

  /// Next residue in the molecule
  [[nodiscard]] std::optional<ConstResidueRef> next() const {
    if (res_ptr() + 1 < res_ptr()->molecule->residues.begin() + molecule().size()) {
      return ConstResidueRef(*(m_residue + 1));
    }
    return std::nullopt;
  }

  /// Previous residue in the molecule
  [[nodiscard]] std::optional<ConstResidueRef> prev() const {
    if (res_ptr() >= res_ptr()->molecule->residues.begin() + 1) {
      return ConstResidueRef(*(m_residue - 1));
    }
    return std::nullopt;
  }

  /// Get children atom by name
  std::optional<ConstAtomRef> operator[](const AtomName& name) const;
  std::optional<ConstAtomRef> operator[](const char* name) const;
  std::optional<ConstAtomRef> operator[](const std::string& name) const;

  /// Check if references point to same data
  bool operator!=(const ConstResidueRef& rhs) const { return res_ptr() != rhs.res_ptr(); }
  bool operator==(const ConstResidueRef& rhs) const { return res_ptr() == rhs.res_ptr(); }

  // TODO: Enable
  //  /// Atoms of the residue
  //  ConstAtomSpan atoms() { return ConstAtomSpan{res_ptr()->atoms}; }
  //  /// Atom coordinates of the molecule
  //  ConstCoordSpan coords() { return atoms().coords(); }

private:

  template<typename>
  friend class proxy::api::ConstAtomAPI;

  BaseResidue* m_residue = nullptr;

  constexpr void check_invariants(const char*) const {};

  [[nodiscard]] constexpr const BaseResidue* res_ptr() const { return m_residue; }

  constexpr explicit ConstResidueRef(BaseResidue& residue) : m_residue(&residue){};
  constexpr ConstResidueRef(BaseResidue* ptr, BaseResidue*) : m_residue(ptr){};
  constexpr void advance() { ++m_residue; }
  void rebase(BaseResidue* from, BaseResidue* to) { m_residue = to + (m_residue - from); }
  constexpr ConstResidueRef() = default; /// constructs object in invalid state (with nullptrs)

  friend ResidueRef;
  friend ConstAtomRef;
};

} // namespace xmol::proxy