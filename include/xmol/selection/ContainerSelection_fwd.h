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


template<typename T>
struct ElementWithFlags {
  static_assert(!std::is_const_v<T>);
  static_assert(!std::is_reference_v<T>);
  static_assert(!std::is_pointer_v<T>);

  using container_type = Container<T>;

  explicit ElementWithFlags(T&& value): value(std::move(value)),is_deleted(false){
  }
  explicit ElementWithFlags(const T& value): value(value),is_deleted(false){
  }

  bool operator<(const ElementWithFlags<T>& rhs) const noexcept ;


  struct PtrComparator{
    bool operator()(const ElementWithFlags<T>* lhs, const ElementWithFlags<T>* rhs){
      assert(lhs!=nullptr);
      assert(rhs!=nullptr);
//      if (lhs == nullptr) {
//        return false;
//      }
//
//      if (rhs == nullptr) {
//        return true;
//      }

      return *lhs < *rhs;
    }
  };

  T value;
  bool is_deleted = false;

private:
  friend class SelectionBase<T>;
  friend class SelectionBase<const T>;
  friend class Selection<T>;
  friend class Selection<const T>;
  friend class Container<T>;
  const container_type* parent() const noexcept;
  container_type* parent() noexcept;

};


}
