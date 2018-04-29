#pragma once

#pragma once
#include "xmol/BaseException.h"

#include <type_traits>

namespace xmol{
namespace selection {

template<typename T>
class SelectionException: public xmol::BaseException {
  static_assert(!std::is_const<T>::value,"");
public:
  using BaseException::BaseException;
};

template<typename T>
class dead_selection_access : public SelectionException<T> {
  static_assert(!std::is_const<T>::value,"");
public:
  using SelectionException<T>::SelectionException;
};

template<typename T>
class dead_selection_range_access : public SelectionException<T> {
  static_assert(!std::is_const<T>::value,"");
public:
  using SelectionException<T>::SelectionException;
};

template<typename T>
class selection_out_of_range: public SelectionException<T> {
  static_assert(!std::is_const<T>::value,"");
public:
  using SelectionException<T>::SelectionException;
};



template<typename T, typename ...Args>
SelectionException<typename std::remove_const<T>::type> make_SelectionException(Args&& ...args){
  return SelectionException<typename std::remove_const<T>::type>(std::forward<Args>(args)...);
}

template<typename T, typename ...Args>
dead_selection_access<typename std::remove_const<T>::type> make_dead_selection_access(Args&& ...args){
  return dead_selection_access<typename std::remove_const<T>::type>(std::forward<Args>(args)...);
}


template<typename T, typename ...Args>
dead_selection_range_access<typename std::remove_const<T>::type> make_dead_selection_range_access(Args&& ...args){
  return dead_selection_range_access<typename std::remove_const<T>::type>(std::forward<Args>(args)...);
}

template<typename T, typename ...Args>
selection_out_of_range<typename std::remove_const<T>::type> make_selection_out_of_range(Args&& ...args){
  return selection_out_of_range<typename std::remove_const<T>::type>(std::forward<Args>(args)...);
}


}
}
