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

enum class ObserverState { ANY, ACTIVE, INVALID };

/// @brief Implements base primitives for observable entity
template <typename Observer> class Observable {
  static_assert(!std::is_reference<Observer>::value);
  static_assert(!std::is_pointer<Observer>::value);

public:
  Observable() = default;
  Observable(Observable&& rhs) noexcept = default;
  Observable(const Observable& rhs) = default;
  Observable& operator=(Observable&& rhs) noexcept = default;
  Observable& operator=(const Observable& rhs) = default;

protected:
  template <ObserverState apply_to = ObserverState::ANY, typename... Args, typename Func = void (Observer::*)(Args...)>
  void notify(Func func, Args&&... args) const {
    static_assert(apply_to != ObserverState::INVALID);
    for (auto& [observer, state] : observers) {
      if (state == ObserverState::ACTIVE) {
        std::invoke(func, observer, std::forward<Args>(args)...);
      } else {
        if (GSL_UNLIKELY(apply_to == ObserverState::ANY)) {
          throw make_SelectionException<Observer>("Observer already dead");
        }
      }
    }
  }

  void add_observer(Observer& ptr) const { observers.emplace(&ptr, ObserverState::ACTIVE); }

  void remove_observer(Observer& ptr) const {
    auto count = observers.erase(&ptr);
    static_cast<void>(count);
    assert(count == 1);
  }

  void clear_observers() const { observers.clear(); }

  /// @brief marks observer as invalid
  /// next broadcast notify would fire an exception
  void invalidate_observer(Observer& ptr) const {
    auto it = observers.find(&ptr);
    assert(it != observers.end());
    it->second = ObserverState::INVALID;
  }

  void move_observer(Observer& from, Observer& to) const {
    remove_observer(from);
    add_observer(to);
  }

protected:
  mutable std::map<Observer*, ObserverState> observers;
};
} // namespace selection
} // namespace xmol
