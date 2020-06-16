#pragma once
#include "TorsionAngle.h"

namespace xmol {

namespace detail {
struct TorsionAngleNameTag {};
} // namespace detail

using TorsionAngleName = utils::ShortAsciiString<4, true, detail::TorsionAngleNameTag>;

class TorsionAngleFactory {
public:
  static std::optional<TorsionAngle> phi(proxy::ResidueRef& r);
  static std::optional<TorsionAngle> psi(proxy::ResidueRef& r);
  static std::optional<TorsionAngle> omega(proxy::ResidueRef& r);

  static std::optional<TorsionAngle> chi1(proxy::ResidueRef& r);
  static std::optional<TorsionAngle> chi2(proxy::ResidueRef& r);
  static std::optional<TorsionAngle> chi3(proxy::ResidueRef& r);
  static std::optional<TorsionAngle> chi4(proxy::ResidueRef& r);
  static std::optional<TorsionAngle> chi5(proxy::ResidueRef& r);

  static void define_protein_backbone_angles(ResidueName);
  static void define_protein_side_chain_angle(ResidueName, TorsionAngleName, const std::array<AtomName, 4>& names,
                                              const std::set<AtomName>& affected_atoms);
  static std::optional<TorsionAngle> get(proxy::ResidueRef& residue, const TorsionAngleName& angleName);

  using four_atoms = std::tuple<proxy::AtomRef, proxy::AtomRef, proxy::AtomRef, proxy::AtomRef>;
  using residue_to_atoms =  std::function<std::optional<four_atoms>(proxy::ResidueRef&)>;

private:
  using ARef = proxy::AtomRef;
  using KeyType = std::pair<ResidueName, TorsionAngleName>;
  using ValueType = std::pair<residue_to_atoms, TorsionAngle::AffectedAtomsSelector>;
  std::map<KeyType, ValueType> bindings;

  static TorsionAngleFactory& instance();
  TorsionAngleFactory();
  std::optional<TorsionAngle> _get(proxy::ResidueRef& residue, const TorsionAngleName& angleName);
  void _define_protein_backbone_angles(ResidueName);
};

} // namespace xmol