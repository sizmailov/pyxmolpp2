#pragma once

#include "xmol/geometry/XYZ.h"
#include "xmol/selection/ContainerSelection_fwd.h"
#include "xmol/utils/ShortAsciiString.h"

#include <memory>
#include <vector>

namespace xmol {
namespace polymer {

using atomIndex_t = int32_t;
using residueIndex_t = int32_t;
using chainIndex_t = int32_t;

using atomId_t = int32_t;
using residueId_t = int32_t;
using frameIndex_t = int32_t;

using XYZ = xmol::geometry::XYZ;

class Atom;

class Residue;

class Chain;

class Frame;

namespace detail {

struct AtomNameTag {};
struct ResidueNameTag {};
struct ChainNameTag {};

template <typename T>
using enabled_if_atom = std::enable_if_t<
    std::is_same<typename std::remove_const<T>::type, Atom>::value>;

template <typename T>
using enabled_if_residue = std::enable_if_t<
    std::is_same<typename std::remove_const<T>::type, Residue>::value>;

template <typename T>
using enabled_if_chain = std::enable_if_t<
    std::is_same<typename std::remove_const<T>::type, Chain>::value>;

template <typename T, typename U>
using add_constness_as =
    std::conditional_t<std::is_const<T>::value, const U,
                       typename std::remove_const<U>::type>;
}

using AtomName = xmol::utils::ShortAsciiString<4, false, detail::AtomNameTag>;
using ResidueName = xmol::utils::ShortAsciiString<3, false, detail::ResidueNameTag>;
using ChainName = xmol::utils::ShortAsciiString<1, false, detail::ChainNameTag>;

template<typename T>
class ElementReference{
public:
  explicit ElementReference(T& t): ptr(&t){
    ptr->parent()->add_reference(*this);
  }
  ElementReference(const ElementReference& other) : ptr(other.ptr){
    if (is_valid()){
      ptr->parent()->add_reference(*this);
    }
  }
  ElementReference(ElementReference&& other) {
    if (other.is_valid()){
      other.ptr->parent()->remove_reference(other);
      other.ptr->parent()->add_reference(*this);
    }
    ptr=other.ptr;
    other.ptr = nullptr;
  }

  ElementReference& operator=(const ElementReference& other) {
    if (this==&other){return *this;}
    if (is_valid()){
      ptr->parent()->remove_reference(*this);
    }
    if (other.is_valid()){
      other.ptr->parent()->add_reference(*this);
    }
    ptr = other.ptr;
    return *this;
  }
  ElementReference& operator=(ElementReference&& other) {
    if (this==&other){return *this;}
    if (is_valid()){
      ptr->parent()->remove_reference(*this);
    }
    if (other.is_valid()){
      other.ptr->parent()->remove_reference(other);
      other.ptr->parent()->add_reference(*this);
    }
    ptr=other.ptr;
    other.ptr = nullptr;
    return *this;
  }
  ~ElementReference(){
    if (is_valid()){
      ptr->parent()->remove_reference(*this);
    }
  }
  bool is_valid() const{
    return ptr!=nullptr;
  }
  explicit operator T& () const{
    if (ptr==nullptr){
      throw std::runtime_error("Deleted element access through reference");
    }
    return *ptr;
  }

  void on_container_move(T* old_begin, T* new_begin){
    ptr = new_begin + (ptr-old_begin);
  }
  void on_container_delete(){
    ptr = nullptr;
  }
public:
  T* ptr;
};

}
}

namespace xmol {
namespace selection {

template <typename T>
class SelectionBaseExtension<T, xmol::polymer::detail::enabled_if_atom<T>>
    : public SelectionBase<T> {
public:
  SelectionBaseExtension() = default;

  template <typename U = T,
            typename SFINAE2 = std::enable_if_t<std::is_const<U>::value>>
  SelectionBaseExtension(const SelectionBaseExtension<
                         typename SelectionBaseExtension<U>::value_type>& rhs)
      : SelectionBase<T>(rhs){};
  SelectionBaseExtension(const SelectionBaseExtension& rhs) = default;
  SelectionBaseExtension(SelectionBaseExtension&& rhs) noexcept = default;

  SelectionBaseExtension&
  operator=(const SelectionBaseExtension& rhs) = default;
  SelectionBaseExtension& operator=(SelectionBaseExtension&& rhs) = default;

  std::vector<xmol::geometry::XYZ> toCoords() const;

  Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Residue>>
  asResidues() const;
  Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Chain>>
  asChains() const;
};

template <typename T>
class SelectionBaseExtension<T, xmol::polymer::detail::enabled_if_residue<T>>
    : public SelectionBase<T> {
public:
  SelectionBaseExtension() = default;

  template <typename U = T,
            typename SFINAE2 = std::enable_if_t<std::is_const<U>::value>>
  SelectionBaseExtension(const SelectionBaseExtension<
                         typename SelectionBaseExtension<U>::value_type>& rhs)
      : SelectionBase<T>(rhs){};
  SelectionBaseExtension(const SelectionBaseExtension& rhs) = default;
  SelectionBaseExtension(SelectionBaseExtension&& rhs) noexcept = default;

  SelectionBaseExtension&
  operator=(const SelectionBaseExtension& rhs) = default;
  SelectionBaseExtension& operator=(SelectionBaseExtension&& rhs) = default;
  Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Atom>>
  asAtoms() const;
  Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Chain>>
  asChains() const;
};

template <typename T>
class SelectionBaseExtension<T, xmol::polymer::detail::enabled_if_chain<T>>
    : public SelectionBase<T> {
public:
  SelectionBaseExtension() = default;

  template <typename U = T,
            typename SFINAE2 = std::enable_if_t<std::is_const<U>::value>>
  SelectionBaseExtension(const SelectionBaseExtension<
                         typename SelectionBaseExtension<U>::value_type>& rhs)
      : SelectionBase<T>(rhs){};
  SelectionBaseExtension(const SelectionBaseExtension& rhs) = default;
  SelectionBaseExtension(SelectionBaseExtension&& rhs) noexcept = default;

  SelectionBaseExtension&
  operator=(const SelectionBaseExtension& rhs) = default;
  SelectionBaseExtension& operator=(SelectionBaseExtension&& rhs) = default;
  Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Atom>>
  asAtoms() const;
  Selection<xmol::polymer::detail::add_constness_as<T, xmol::polymer::Residue>>
  asResidues() const;
};
}
}
