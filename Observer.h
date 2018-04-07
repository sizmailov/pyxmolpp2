#pragma once

#include <vector>
#include <functional>
#include <map>
#include <cassert>

#include "Logger.h"

template<typename T>
class ObserverableBy{
  static_assert(!std::is_reference_v<T>);
  static_assert(!std::is_pointer_v<T>);
public:

protected:

  enum class ObserverState{
    OK,
    DELETED
  };

  enum class ApplyTo{
    ANY,
    ALIVE_ONLY
  };

  template<ApplyTo apply_to=ApplyTo::ANY, typename ...Args, typename Func = void (T::*)(Args...)>
  void notify_all(Func func, Args&& ...args) {
    LOG_DEBUG_FUNCTION();
    for (auto [observable, state]: observers){
      if (state==ObserverState::OK){
        std::invoke(func, observable, std::forward<Args>(args)...);
      }else{
        if (apply_to==ApplyTo::ANY) {
          throw std::runtime_error("Observer already dead");
        }else{
          LOG_WARNING("Observable outlives its observer");
        }
      }

    }
  }

  template<ApplyTo apply_to=ApplyTo::ANY, typename ...Args, typename Func = void (T::*)(Args...)>
  void notify_all(Func func, Args&& ...args) const {
    LOG_DEBUG_FUNCTION();
    for (auto [observable, state]: observers){
      if (state==ObserverState::OK){
        std::invoke(func, observable, std::forward<Args>(args)...);
      }else{
        if (apply_to==ApplyTo::ANY) {
          throw std::runtime_error("Observer already dead");
        }else{
          LOG_WARNING("Observable outlives its observer");
        }
      }
    }
  }


  void add_observer(T& ptr) const {
    LOG_DEBUG_FUNCTION();
    observers.emplace(&ptr, ObserverState::OK);
  }

  void remove_observer(T& ptr) const{
    LOG_DEBUG_FUNCTION();
    auto count = observers.erase(&ptr);
    assert(count==1);
  }

  void remove_all_observers() const{
    observers.clear();
  }


  void mark_as_deleted(T& ptr) const {
    LOG_DEBUG_FUNCTION();
    auto it = observers.find(&ptr);
    assert(it!=observers.end());
    it->second = ObserverState::DELETED;
  }


  void move_observer(T& from, T& to) const{
    remove_observer(from);
    add_observer(to);
  }



private:
  mutable std::map<T*,ObserverState> observers;
};
