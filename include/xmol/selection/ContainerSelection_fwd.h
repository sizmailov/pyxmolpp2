#pragma once

#include <cassert>
#include <type_traits>
#include <utility>


namespace xmol::selection{

template<typename T>
class Container;

template<typename T>
class SelectionBase;

template<typename T>
class Selection;


template<typename T, typename SFINAE=void>
class SelectionBaseExtension : public SelectionBase<T>{
public:
  SelectionBaseExtension() = default;
  template<typename U=T, typename SFINAE2=std::enable_if_t<std::is_const_v<U>>>
  SelectionBaseExtension(const SelectionBaseExtension<typename SelectionBaseExtension<U>::value_type>& rhs) : SelectionBase<T>(rhs){};
  SelectionBaseExtension(const SelectionBaseExtension& rhs) = default;
  SelectionBaseExtension(SelectionBaseExtension&& rhs) noexcept = default;

  SelectionBaseExtension& operator=(const SelectionBaseExtension& rhs) = default;
  SelectionBaseExtension& operator=(SelectionBaseExtension&& rhs) = default;
};

template<typename value_type>
struct SelectionPointerComparator{
  static_assert(!std::is_const_v<value_type>);
  bool operator()(const value_type* lhs, const value_type* rhs) const;
};

template<typename T, typename SFINAE=void>
struct SelectionTraits{
  static_assert(!std::is_reference_v<T>);
  static_assert(!std::is_pointer_v<T>);
  static const bool is_const = std::is_const_v<T>;
  using value_type = std::remove_const_t<T>;
  using container_type = std::conditional_t<is_const,const Container<value_type>,Container<value_type>>;
  using selection_element_type = std::conditional_t<is_const,
                                          const value_type*,
                                          value_type*>;

  using on_selection_move_type =std::conditional_t<is_const,
                                                   void (container_type::*)(SelectionBase<const value_type>&,SelectionBase<const value_type>&) const,
                                                   void (container_type::*)(SelectionBase<value_type>&, SelectionBase<value_type>&)
  >;

  using on_selection_copy_type =std::conditional_t<is_const,
                                                   void (container_type::*)(SelectionBase<const value_type>&) const,
                                                   void (container_type::*)(SelectionBase<value_type>&)
  >;


  using on_selection_delete_type =std::conditional_t<is_const,
                                                     void (container_type::*)(SelectionBase<const value_type>&) const,
                                                     void (container_type::*)(SelectionBase<value_type>&)
  >;

};


}
