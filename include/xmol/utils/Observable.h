#pragma once

#include <cassert>
#include <functional>
#include <map>
#include <type_traits>
#include <vector>

#include <gsl/assert>

namespace xmol::utils {

class DeadObserverAccessError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

template <typename Observer>
class DeadObserverAccessErrorT : public DeadObserverAccessError { /// todo: parametrize on error message instead
public:
  using DeadObserverAccessError::DeadObserverAccessError;
};

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
          throw DeadObserverAccessErrorT<Observer>("");
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

public:

  void on_move(Observer& from, Observer& to) {
    remove_observer(from);
    add_observer(to);
  }

  void on_delete(Observer& o) { remove_observer(o); }
  void on_copy(Observer& o) { add_observer(o); }

protected:
  mutable std::map<Observer*, ObserverState> observers;
};

} // namespace xmol
