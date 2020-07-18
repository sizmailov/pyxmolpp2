#pragma

#include "AtomSpan.h"

namespace xmol::proxy {

class ResidueSpan : public ProxySpan<ResidueRef, BaseResidue> {
public:
  using ProxySpan::ProxySpan;

  [[nodiscard]] CoordSpan coords() const;
  [[nodiscard]] AtomSpan atoms() const;
  [[nodiscard]] MoleculeSpan molecules() const;

  [[nodiscard]] bool contains(const ResidueRef& ref) const;

  template <typename Predicate> ResidueSelection filter(Predicate&& p) const;
  [[nodiscard]] std::vector<ResidueIndex> index() const;

  [[nodiscard]] ResidueSelection slice(std::optional<size_t> start, std::optional<size_t> stop, std::optional<size_t> step) const;
  [[nodiscard]] ResidueSpan slice(std::optional<size_t> start, std::optional<size_t> stop) const;

  /// Make smart span from this
  [[nodiscard]] smart::ResidueSmartSpan smart() const;

  /// Intersect inplace
  ResidueSpan& operator&=(const ResidueSpan& rhs);

protected:
  [[nodiscard]] Frame* frame_ptr() { return empty() ? nullptr : m_begin->molecule->frame; }
  const Frame* frame_ptr() const { return empty() ? nullptr : m_begin->molecule->frame; }

private:
  friend smart::ResidueSmartSpan;
};

} // namespace xmol::proxy