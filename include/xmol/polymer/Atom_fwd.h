#pragma once

#include "xmol/utils/ShortAsciiString.h"
#include "xmol/selection/ContainerSelection_fwd.h"

#include <memory>

namespace xmol::polymer {

using residueIndex_t = int32_t;
using chainIndex_t = int32_t;

using atomId_t = int32_t;
using residueId_t = int32_t;
using frameIndex_t = int32_t;


struct XYZ {
  double x, y, z;
};

class Atom;
class Residue;
class Chain;
class Frame;

namespace detail {

struct AtomNameTag { };
struct ResidueNameTag { };
struct ChainNameTag { };

template<typename T>
using enabled_if_atom = std::enable_if_t<std::is_same_v<std::remove_const_t<T>, Atom>>;

template<typename T>
using enabled_if_residue = std::enable_if_t<std::is_same_v<std::remove_const_t<T>, Residue>>;

template<typename T>
using enabled_if_chain = std::enable_if_t<std::is_same_v<std::remove_const_t<T>, Chain>>;

template<typename T, typename U>
using add_constness_as = std::conditional_t<std::is_const_v<T>,const U, std::remove_const_t<U>>;

}

using AtomName = xmol::utils::ShortAsciiString<4, detail::AtomNameTag>;
using ResidueName = xmol::utils::ShortAsciiString<3, detail::ResidueNameTag>;
using ChainName = xmol::utils::ShortAsciiString<1, detail::ChainNameTag>;

}

namespace xmol::selection {

template<typename T>
class SelectionBaseExtension<T,xmol::polymer::detail::enabled_if_atom<T>>: public SelectionBase<T> {
public:
  SelectionBaseExtension() = default;
  template<typename U=T, typename SFINAE2=std::enable_if_t<std::is_const_v<U>>>
  SelectionBaseExtension(const SelectionBaseExtension<typename SelectionBaseExtension<U>::value_type>& rhs) : SelectionBase<T>(rhs){};
  SelectionBaseExtension(const SelectionBaseExtension& rhs) = default;
  SelectionBaseExtension(SelectionBaseExtension&& rhs) noexcept = default;

  SelectionBaseExtension& operator=(const SelectionBaseExtension& rhs) = default;
  SelectionBaseExtension& operator=(SelectionBaseExtension&& rhs) = default;
  Selection<xmol::polymer::detail::add_constness_as<T,xmol::polymer::Residue>> asResidues() const;
  Selection<xmol::polymer::detail::add_constness_as<T,xmol::polymer::Chain>> asChains() const;
};

template<typename T>
class SelectionBaseExtension<T,xmol::polymer::detail::enabled_if_residue<T>>: public SelectionBase<T> {
public:
  SelectionBaseExtension() = default;
  template<typename U=T, typename SFINAE2=std::enable_if_t<std::is_const_v<U>>>
  SelectionBaseExtension(const SelectionBaseExtension<typename SelectionBaseExtension<U>::value_type>& rhs) : SelectionBase<T>(rhs){};
  SelectionBaseExtension(const SelectionBaseExtension& rhs) = default;
  SelectionBaseExtension(SelectionBaseExtension&& rhs) noexcept = default;

  SelectionBaseExtension& operator=(const SelectionBaseExtension& rhs) = default;
  SelectionBaseExtension& operator=(SelectionBaseExtension&& rhs) = default;
  Selection<xmol::polymer::detail::add_constness_as<T,xmol::polymer::Atom>> asAtoms() const;
  Selection<xmol::polymer::detail::add_constness_as<T,xmol::polymer::Chain>>  asChains() const;
};

template<typename T>
class SelectionBaseExtension<T,xmol::polymer::detail::enabled_if_chain<T>>: public SelectionBase<T> {
public:
  SelectionBaseExtension() = default;
  template<typename U=T, typename SFINAE2=std::enable_if_t<std::is_const_v<U>>>
  SelectionBaseExtension(const SelectionBaseExtension<typename SelectionBaseExtension<U>::value_type>& rhs) : SelectionBase<T>(rhs){};
  SelectionBaseExtension(const SelectionBaseExtension& rhs) = default;
  SelectionBaseExtension(SelectionBaseExtension&& rhs) noexcept = default;

  SelectionBaseExtension& operator=(const SelectionBaseExtension& rhs) = default;
  SelectionBaseExtension& operator=(SelectionBaseExtension&& rhs) = default;
  Selection<xmol::polymer::detail::add_constness_as<T,xmol::polymer::Atom>> asAtoms() const;
  Selection<xmol::polymer::detail::add_constness_as<T,xmol::polymer::Residue>>  asResidues() const;
};

}
