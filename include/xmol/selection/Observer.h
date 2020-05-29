#pragma once

#include "exceptions.h"

#include <cassert>
#include <functional>
#include <gsl/gsl_assert>
#include <map>
#include <type_traits>
#include <vector>

namespace xmol {
namespace selection {

enum class ObserverState { OK, DELETED };

enum class ApplyTo { ANY, ALIVE_ONLY };

template <typename T> class ObservableBy {
  static_assert(!std::is_reference<T>::value, "");
  static_assert(!std::is_pointer<T>::value, "");

public:
  ObservableBy() = default;
  ObservableBy(ObservableBy&& rhs) noexcept = default;
  ObservableBy(const ObservableBy& rhs) = default;
  ObservableBy& operator=(ObservableBy&& rhs) noexcept = default;
  ObservableBy& operator=(const ObservableBy& rhs) = default;

protected:
  template <ApplyTo apply_to = ApplyTo::ANY, typename... Args,
            typename Func = void (T::*)(Args...)>
  void notify_all(Func func, Args&&... args) const {
    for (auto& pair : observers) {
      if (pair.second == ObserverState::OK) {
        //        std::invoke(func, observable, std::forward<Args>(args)...);
        ((*std::forward<T* const>(pair.first)).*
         func)(std::forward<Args>(args)...);
      } else {
        if (GSL_UNLIKELY(apply_to == ApplyTo::ANY)) {
          throw make_SelectionException<T>("Observer already dead");
        }
      }
    }
  }

  void add_observer(T& ptr) const {
    observers.emplace(&ptr, ObserverState::OK);
  }

  void remove_observer(T& ptr) const {
    auto count = observers.erase(&ptr);
    static_cast<void>(count);
    assert(count == 1);
  }

  void remove_all_observers() const {
    observers.clear();
  }

  void mark_as_deleted(T& ptr) const {
    auto it = observers.find(&ptr);
    assert(it != observers.end());
    it->second = ObserverState::DELETED;
  }

  void move_observer(T& from, T& to) const {
    remove_observer(from);
    add_observer(to);
  }

protected:
  mutable std::map<T*, ObserverState> observers;
};
}
}
