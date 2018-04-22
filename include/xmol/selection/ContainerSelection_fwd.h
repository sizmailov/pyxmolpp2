#pragma once

#include <cassert>
#include <type_traits>
#include <utility>

namespace xmol {
namespace selection {

template <typename T> class Container;

template <typename T> class SelectionBase;

template <typename T> class Selection;

template <typename T, typename SFINAE = void>
class SelectionBaseExtension : public SelectionBase<T> {
public:
  SelectionBaseExtension() = default;

  template <typename U = T, typename SFINAE2 = typename std::enable_if<
                                std::is_const<U>::value>::type>
  SelectionBaseExtension(const SelectionBaseExtension<
                         typename SelectionBaseExtension<U>::value_type>& rhs)
      : SelectionBase<T>(rhs){};
  SelectionBaseExtension(const SelectionBaseExtension& rhs) = default;
  SelectionBaseExtension(SelectionBaseExtension&& rhs) noexcept = default;

  SelectionBaseExtension&
  operator=(const SelectionBaseExtension& rhs) = default;
  SelectionBaseExtension& operator=(SelectionBaseExtension&& rhs) = default;
};

template <typename value_type> struct SelectionPointerComparator {
  static_assert(!std::is_const<value_type>::value,"");
  bool operator()(const value_type* lhs, const value_type* rhs) const;
};

template <typename T, typename SFINAE = void> struct SelectionTraits {
  static_assert(!std::is_reference<T>::value,"");
  static_assert(!std::is_pointer<T>::value,"");
  static const bool is_const = std::is_const<T>::value;
  using value_type = typename std::remove_const<T>::type;
  using container_type =
      typename std::conditional<is_const, const Container<value_type>,
                                Container<value_type>>::type;
  using selection_element_type =
      typename std::conditional<is_const, const value_type*, value_type*>::type;

  using on_selection_move_type = typename std::conditional<
      is_const,
      void (container_type::*)(SelectionBase<const value_type>&,
                               SelectionBase<const value_type>&) const,
      void (container_type::*)(SelectionBase<value_type>&,
                               SelectionBase<value_type>&)>::type;

  using on_selection_copy_type = typename std::conditional<
      is_const,
      void (container_type::*)(SelectionBase<const value_type>&) const,
      void (container_type::*)(SelectionBase<value_type>&)>::type;

  using on_selection_delete_type = typename std::conditional<
      is_const,
      void (container_type::*)(SelectionBase<const value_type>&) const,
      void (container_type::*)(SelectionBase<value_type>&)>::type;
};
}
}