#pragma once

#include "ContainerSelection_fwd.h"
#include "Observer.h"
#include <set>

namespace xmol::selection {

class dead_selection_access : public std::logic_error {
public:
  explicit dead_selection_access(const std::string& what_arg)
      : logic_error(what_arg) {}
  explicit dead_selection_access(const char* what_arg)
      : logic_error(what_arg) {}
};

class dead_selection_range_access : public std::logic_error {
public:
  explicit dead_selection_range_access(const std::string& what_arg)
      : logic_error(what_arg) {}
  explicit dead_selection_range_access(const char* what_arg)
      : logic_error(what_arg) {}
};

enum class SelectionState { OK, HAS_DANGLING_REFERENCES };

template <typename T> class SelectionRange {
public:
  SelectionRange(const SelectionRange& other) noexcept = default;
  SelectionRange(SelectionRange&& other) noexcept = default;
  SelectionRange& operator=(const SelectionRange& other) noexcept = default;
  SelectionRange& operator=(SelectionRange&& other) noexcept = default;

  T& operator*() const;
  T* operator->() const;

  template <typename Sentinel> bool operator!=(const Sentinel&) const;
  template <typename Sentinel> bool operator==(const Sentinel&) const;

  SelectionRange& operator++();
  SelectionRange& operator--();

  SelectionRange operator-(int n);
  SelectionRange operator+(int n);
  SelectionRange& operator+=(int n);
  SelectionRange& operator-=(int n);

private:
  explicit SelectionRange(const SelectionBase<T>& selection, int pos, int end,
                          int step);
  friend class SelectionBase<T>;
  const SelectionBase<T>* selection;
  int pos;
  int end;
  int step;
};

template <typename T>
class SelectionBase
    : public ObservableBy<typename SelectionTraits<T>::container_type> {
public:
  static_assert(!std::is_reference_v<T>);
  static_assert(!std::is_pointer_v<T>);
  using value_type = typename SelectionTraits<T>::value_type;
  using container_type = typename SelectionTraits<T>::container_type;
  using selection_element_type =
      typename SelectionTraits<T>::selection_element_type;

  SelectionBase();
  template <typename U = T,
            typename SFINAE = std::enable_if_t<std::is_const_v<U>>>
  SelectionBase(
      const SelectionBase<typename SelectionBase<U>::value_type>& rhs);
  SelectionBase(const SelectionBase& rhs);
  SelectionBase(SelectionBase&& rhs) noexcept;
  ~SelectionBase();

  SelectionBase& operator=(const SelectionBase& rhs);
  SelectionBase& operator=(SelectionBase&& rhs) noexcept;

  int count(const T&) const;

  bool empty() const noexcept;
  int size() const noexcept;
  void clear() noexcept;
  bool is_valid() noexcept;

  SelectionRange<T> begin() const;
  SelectionRange<T> end() const;

  T& operator[](int i) const;

protected:
  //  explicit SelectionBase(container_type& container);

  void on_container_move(container_type& from, container_type& to);
  void on_container_delete(container_type& half_dead);
  void on_container_elements_move(selection_element_type old_begin,
                                  selection_element_type old_end,
                                  ptrdiff_t shift);
  void remove_redundant_observers();

  friend class SelectionRange<T>;
  friend class SelectionBase<value_type>;
  friend class SelectionBase<const value_type>;
  friend class Container<value_type>;

  std::vector<selection_element_type> elements;
  SelectionState state = SelectionState::OK;
};

template <typename T> class Selection : public SelectionBaseExtension<T> {
  static_assert(std::is_base_of_v<SelectionBase<T>, SelectionBaseExtension<T>>);
  static_assert(std::is_default_constructible_v<SelectionBaseExtension<T>>);
  static_assert(std::is_constructible_v<
                SelectionBaseExtension<T>,
                const SelectionBaseExtension<std::remove_const_t<T>>&>);
  static_assert(std::is_copy_constructible_v<SelectionBaseExtension<T>>);
  static_assert(std::is_move_constructible_v<SelectionBaseExtension<T>>);
  static_assert(std::is_move_assignable_v<SelectionBaseExtension<T>>);
  static_assert(std::is_copy_assignable_v<SelectionBaseExtension<T>>);

public:
  using value_type = typename SelectionTraits<T>::value_type;
  using container_type = typename SelectionTraits<T>::container_type;
  using selection_element_type =
      typename SelectionTraits<T>::selection_element_type;

  Selection() = default;
  template <typename U = T,
            typename SFINAE = std::enable_if_t<std::is_const_v<U>>>
  Selection(const Selection<typename Selection<U>::value_type>& rhs)
      : SelectionBaseExtension<T>(rhs){};
  Selection(const Selection& rhs) = default;
  Selection(Selection&& rhs) noexcept = default;

  Selection& operator=(const Selection& rhs) = default;
  Selection& operator=(Selection&& rhs) = default;

  template <typename Predicate> Selection& remove_if(Predicate&& p);

  template <typename Predicate> Selection filter(Predicate&& p) const;

  template <typename Transform> Selection& for_each(Transform&& transform);

  template <typename Transform>
  const Selection& for_each(Transform&& transform) const;

  template <typename V, typename U = T,
            typename SFINAE = std::enable_if_t<
                std::is_same_v<std::remove_const_t<U>, std::remove_const_t<V>>>>
  bool operator==(const Selection<V>& rhs) const;
  template <typename V, typename U = T,
            typename SFINAE = std::enable_if_t<
                std::is_same_v<std::remove_const_t<U>, std::remove_const_t<V>>>>
  bool operator!=(const Selection<V>& rhs) const;

  template <typename V, typename U = T,
            typename SFINAE =
                std::enable_if_t<std::is_const_v<U> || !std::is_const_v<V>>>
  Selection<U>& operator+=(const Selection<V>& rhs);
  template <typename V, typename U = T,
            typename SFINAE = std::enable_if_t<
                std::is_same_v<std::remove_const_t<U>, std::remove_const_t<V>>>>
  Selection<U>& operator-=(const Selection<V>& rhs);
  template <typename V, typename U = T,
            typename SFINAE = std::enable_if_t<
                std::is_same_v<std::remove_const_t<U>, std::remove_const_t<V>>>>
  Selection<U>& operator*=(const Selection<V>& rhs);

  template <typename Iterator>
  explicit Selection(Iterator begin_, Iterator end_);

protected:
  friend class Selection<value_type>;
  friend class Selection<const value_type>;
  friend class Container<value_type>;

  explicit Selection(typename SelectionBase<T>::container_type& container);
};

template <typename T>
class Container : public ObservableBy<SelectionBase<T>>,
                  public ObservableBy<SelectionBase<const T>> {
public:
  using value_type = typename SelectionTraits<T>::value_type;

  explicit Container(int reserve = 0);
  ~Container();
  Container(Container&& rhs) noexcept;
  Container(const Container& rhs);

  Container& operator=(Container&& rhs) noexcept;
  Container& operator=(const Container& rhs);

  int size() const noexcept;
  bool empty() const noexcept;

  void insert(T&& value);

  template <typename... Args> T& emplace(Args&&... args);

  void clear();
  int erase(const Selection<T>& to_delete);

  Selection<T> all();
  Selection<const T> all() const;

  friend class Selection<T>;
  friend class Selection<const T>;

  friend class SelectionBase<T>;
  friend class SelectionBase<const T>;

protected:
  void on_selection_move(SelectionBase<T>& from, SelectionBase<T>& to);
  void on_selection_move(SelectionBase<const T>& from,
                         SelectionBase<const T>& to) const;

  void on_selection_copy(SelectionBase<T>& copy);
  void on_selection_copy(SelectionBase<const T>& copy) const;

  void on_selection_delete(SelectionBase<T>& half_dead);
  void on_selection_delete(SelectionBase<const T>& half_dead) const;

  std::vector<value_type> elements;
  int n_deleted = 0;
};

template <typename T>
SelectionRange<T>::SelectionRange(const SelectionBase<T>& selection, int pos,
                                  int end, int step)
    : selection(&selection), pos(pos), end(end), step(step) {}

template <typename T> T& SelectionRange<T>::operator*() const {
  if (this->selection->elements[pos]->is_deleted()) {
    throw dead_selection_range_access("SelectionRange<T>::operator*");
  }
  return *this->selection->elements[pos];
}
template <typename T> T* SelectionRange<T>::operator->() const {
  if (this->selection->elements[pos]->is_deleted()) {
    throw dead_selection_range_access("SelectionRange<T>::operator->");
  }
  return this->selection->elements[pos];
}

template <typename T>
template <typename Sentinel>
bool SelectionRange<T>::operator!=(const Sentinel&) const {
  if (step > 0) {
    return pos < end;
  } else {
    return pos > end;
  }
}

template <typename T>
template <typename Sentinel>
bool SelectionRange<T>::operator==(const Sentinel& end) const {
  return !(*this != end);
}

template <typename T> SelectionRange<T>& SelectionRange<T>::operator++() {
  pos += step;
  return *this;
}

template <typename T> SelectionRange<T>& SelectionRange<T>::operator--() {
  pos -= step;
  return *this;
}

template <typename T> SelectionRange<T>& SelectionRange<T>::operator+=(int n) {
  pos += step * n;
  return *this;
}

template <typename T> SelectionRange<T>& SelectionRange<T>::operator-=(int n) {
  pos -= step * n;
  return *this;
}

template <typename T> SelectionRange<T> SelectionRange<T>::operator-(int n) {
  SelectionRange<T> result(*this);
  result -= n;
  return result;
}

template <typename T> SelectionRange<T> SelectionRange<T>::operator+(int n) {
  SelectionRange<T> result(*this);
  result += n;
  return result;
}

template <typename T> SelectionRange<T> SelectionBase<T>::begin() const {
  LOG_VERBOSE_FUNCTION();
  if (state != SelectionState::OK) {
    throw dead_selection_access("SelectionRange<T>::begin");
  }
  return SelectionRange(*this, 0, elements.size(), 1);
}

template <typename T> SelectionRange<T> SelectionBase<T>::end() const {
  LOG_VERBOSE_FUNCTION();
  if (state != SelectionState::OK) {
    throw dead_selection_access("SelectionRange<T>::end");
  }
  return SelectionRange(*this, elements.size(), 0, 1);
}

template <typename T> T& SelectionBase<T>::operator[](int i) const {
  LOG_VERBOSE_FUNCTION();
  if (state != SelectionState::OK) {
    throw dead_selection_access("SelectionRange<T>::operator[]");
  }
  if (i < -size() || i >= size()) {
    throw std::out_of_range("SelectionBase<T>::[]");
  }
  if (i < 0) {
    i = size() + i;
  }
  if (elements[i]->is_deleted()) {
    throw dead_selection_range_access("SelectionBase<T>::[]");
  }
  return *elements[i];
}

template <typename T> SelectionBase<T>::SelectionBase() {
  LOG_DEBUG_FUNCTION();
  state = SelectionState::OK;
}

template <typename T> SelectionBase<T>::~SelectionBase() {
  LOG_DEBUG_FUNCTION();
  this->clear();
}

template <typename T>
SelectionBase<T>::SelectionBase(const SelectionBase<T>& rhs)
    : ObservableBy<container_type>(rhs), state(rhs.state),
      elements(rhs.elements) {
  LOG_DEBUG_FUNCTION();
  this->notify_all(
      static_cast<typename SelectionTraits<T>::on_selection_copy_type>(
          &container_type::on_selection_copy),
      *this);
}

template <typename T>
template <typename U, typename SFINAE>
SelectionBase<T>::SelectionBase(
    const SelectionBase<typename SelectionBase<U>::value_type>& rhs)
    : state(rhs.state), elements(rhs.elements.begin(), rhs.elements.end()) {
  LOG_DEBUG_FUNCTION();
  this->observers.insert(rhs.observers.begin(), rhs.observers.end());
  this->notify_all(
      static_cast<typename SelectionTraits<U>::on_selection_copy_type>(
          &container_type::on_selection_copy),
      *this);
}

template <typename T>
SelectionBase<T>::SelectionBase(SelectionBase<T>&& rhs) noexcept
    : ObservableBy<container_type>(std::move(rhs)),
      state(rhs.state),
      elements(std::move(rhs.elements)) {
  LOG_DEBUG_FUNCTION();
  this->notify_all(
      static_cast<typename SelectionTraits<T>::on_selection_move_type>(
          &container_type::on_selection_move),
      rhs, *this);
  rhs.state = SelectionState::OK;
}
template <typename T>
SelectionBase<T>& SelectionBase<T>::operator=(SelectionBase<T>&& rhs) noexcept {
  LOG_DEBUG_FUNCTION();
  if (&rhs == this) {
    return *this;
  }
  this->clear();
  ObservableBy<container_type>::operator=(std::move(rhs));
  ObservableBy<container_type>::notify_all(
      static_cast<typename SelectionTraits<T>::on_selection_move_type>(
          &container_type::on_selection_move),
      rhs, *this);
  elements = std::move(rhs.elements);
  state = rhs.state;
  rhs.state = SelectionState::OK;
  return *this;
}

template <typename T>
SelectionBase<T>& SelectionBase<T>::operator=(const SelectionBase<T>& rhs) {
  LOG_DEBUG_FUNCTION();
  if (&rhs == this) {
    return *this;
  }
  this->clear();
  ObservableBy<container_type>::operator=(rhs);
  this->notify_all(&container_type::on_selection_copy, *this);
  elements = rhs.elements;
  state = rhs.state;
  return *this;
}

template <typename T>
template <typename V, typename U, typename SFINAE>
bool Selection<T>::operator==(const Selection<V>& rhs) const {
  LOG_DEBUG_FUNCTION();
  return this->elements.size() == rhs.size() &&
         std::equal(this->elements.begin(), this->elements.end(),
                    rhs.elements.begin());
}

template <typename T>
template <typename V, typename U, typename SFINAE>
bool Selection<T>::operator!=(const Selection<V>& rhs) const {
  LOG_DEBUG_FUNCTION();
  return this->elements.size() != rhs.size() ||
         !std::equal(this->elements.begin(), this->elements.end(),
                     rhs.elements.begin());
}

template <typename T>
template <typename V, typename U, typename SFINAE>
Selection<U>& Selection<T>::operator+=(const Selection<V>& rhs) {
  LOG_DEBUG_FUNCTION();
  auto comparator = SelectionPointerComparator<value_type>{};

  for (auto[container, state] : rhs.observers) {
    this->add_observer(*container);
    container->ObservableBy<SelectionBase<T>>::add_observer(*this);
  }

  assert(
      std::is_sorted(this->elements.begin(), this->elements.end(), comparator));
  assert(std::is_sorted(rhs.elements.begin(), rhs.elements.end(), comparator));
  size_t selection_size = this->elements.size();
  this->elements.insert(this->elements.end(), rhs.elements.begin(),
                        rhs.elements.end());
  std::inplace_merge(this->elements.begin(),
                     this->elements.begin() + selection_size,
                     this->elements.end(), comparator);
  auto end = std::unique(this->elements.begin(), this->elements.end());
  this->elements.erase(end, this->elements.end());
  assert(
      std::is_sorted(this->elements.begin(), this->elements.end(), comparator));
  return *this;
}

template <typename T>
template <typename V, typename U, typename SFINAE>
Selection<U>& Selection<T>::operator-=(const Selection<V>& rhs) {
  LOG_DEBUG_FUNCTION();
  auto comparator = SelectionPointerComparator<value_type>{};
  assert(
      std::is_sorted(this->elements.begin(), this->elements.end(), comparator));
  assert(std::is_sorted(rhs.elements.begin(), rhs.elements.end(), comparator));
  auto it = this->elements.begin();
  auto it1 = this->elements.begin();
  auto it2 = rhs.elements.begin();
  while (it1 != this->elements.end() && it2 != rhs.elements.end()) {
    if (comparator(*it2, *it1)) { // it2 < it1
      ++it2;
    } else if (comparator(*it1, *it2)) { // it1 < it2
      *it = *it1;
      ++it;
      ++it1;
    } else { // it1 == it2;
      ++it1;
      ++it2;
    }
  }
  while (it1 != this->elements.end()) {
    *it = *it1;
    ++it;
    ++it1;
  }
  this->elements.erase(it, this->elements.end());
  assert(
      std::is_sorted(this->elements.begin(), this->elements.end(), comparator));

  this->remove_redundant_observers();

  return *this;
}

template <typename T>
template <typename V, typename U, typename SFINAE>
Selection<U>& Selection<T>::operator*=(const Selection<V>& rhs) {
  LOG_DEBUG_FUNCTION();
  auto comparator = SelectionPointerComparator<value_type>{};
  assert(
      std::is_sorted(this->elements.begin(), this->elements.end(), comparator));
  assert(std::is_sorted(rhs.elements.begin(), rhs.elements.end(), comparator));

  auto end = std::set_intersection(this->elements.begin(), this->elements.end(),
                                   rhs.elements.begin(), rhs.elements.end(),
                                   this->elements.begin(), comparator);
  this->elements.erase(end, this->elements.end());
  assert(
      std::is_sorted(this->elements.begin(), this->elements.end(), comparator));
  this->remove_redundant_observers();
  return *this;
}

template <typename T>
template <typename Predicate>
Selection<T>& Selection<T>::remove_if(Predicate&& p) {
  LOG_DEBUG_FUNCTION();
  if (this->state != SelectionState::OK) {
    throw dead_selection_access("SelectionRange<T>::remove_if");
  }
  auto end = std::remove_if(this->elements.begin(), this->elements.end(),
                            [&p](selection_element_type ptr) {
                              if (ptr->is_deleted()) {
                                throw dead_selection_range_access(
                                    "SelectionRange<T>::remove_if");
                              }
                              const value_type& value = *ptr;
                              return p(value);
                            });
  this->elements.erase(end, this->elements.end());
  this->remove_redundant_observers();
  return *this;
}
template <typename T>
template <typename Predicate>
Selection<T> Selection<T>::filter(Predicate&& p) const {
  LOG_DEBUG_FUNCTION();
  Selection<T> result(*this);
  result.remove_if([&p](const value_type& val) { return !p(val); });
  return result;
}

template <typename T>
template <typename Transform>
Selection<T>& Selection<T>::for_each(Transform&& transform) {
  LOG_DEBUG_FUNCTION();
  for (auto& value : *this) {
    transform(value);
  }
  this->remove_redundant_observers();
  return *this;
}

template <typename T>
template <typename Transform>
const Selection<T>& Selection<T>::for_each(Transform&& transform) const {
  LOG_DEBUG_FUNCTION();
  for (auto& value : *this) {
    transform(value);
  }
  return *this;
}

template <typename T> void SelectionBase<T>::remove_redundant_observers() {
  std::set<container_type*> new_observers;
  for (auto el_ptr : elements) {
    new_observers.insert(el_ptr->parent());
  }

  for (auto[container, state] : this->observers) {
    if (!new_observers.count(container)) {
      this->remove_observer(*container);
      container->ObservableBy<SelectionBase<T>>::remove_observer(*this);
    }
  }
}

template <typename T> int SelectionBase<T>::count(const T& value) const {
  LOG_DEBUG_FUNCTION();
  if (state != SelectionState::OK) {
    throw dead_selection_access("SelectionRange<T>::count");
  }
  return std::find_if(elements.begin(), elements.end(),
                      [&value](const selection_element_type el) {
                        return *el == value;
                      }) == elements.end()
             ? 0
             : 1;
}

template <typename T> bool SelectionBase<T>::empty() const noexcept {
  LOG_DEBUG_FUNCTION();
  return elements.empty();
}

template <typename T> int SelectionBase<T>::size() const noexcept {
  LOG_DEBUG_FUNCTION();
  return elements.size();
}

template <typename T> void SelectionBase<T>::clear() noexcept {
  LOG_DEBUG_FUNCTION();
  elements.clear();
  this->template notify_all<ApplyTo::ALIVE_ONLY>(
      static_cast<typename SelectionTraits<T>::on_selection_delete_type>(
          &container_type::on_selection_delete),
      *this);
  this->remove_all_observers();
  state = SelectionState::OK;
}

template <typename T> bool SelectionBase<T>::is_valid() noexcept {
  return state == SelectionState::OK;
}

template <typename T>
void SelectionBase<T>::on_container_move(SelectionBase<T>::container_type& from,
                                         SelectionBase<T>::container_type& to) {
  LOG_DEBUG_FUNCTION();
  this->move_observer(from, to);
}
template <typename T>
void SelectionBase<T>::on_container_elements_move(
    selection_element_type old_begin, selection_element_type old_end,
    ptrdiff_t shift) {
  LOG_DEBUG_FUNCTION();
  for (auto& ptr : elements) {
    if (old_begin <= ptr && ptr < old_end) {
      ptr += shift;
    }
  }
}

template <typename T>
void SelectionBase<T>::on_container_delete(
    SelectionBase<T>::container_type& half_dead) {
  LOG_DEBUG_FUNCTION();
  this->mark_as_deleted(half_dead);
  state = SelectionState::HAS_DANGLING_REFERENCES;
}

template <typename T>
Selection<T>::Selection(typename SelectionBase<T>::container_type& container) {
  LOG_DEBUG_FUNCTION();
  this->state = SelectionState::OK;
  for (T& el : container.elements) {
    if (!el.is_deleted()) {
      this->elements.push_back(&el);
    }
  }
  if (!this->empty()) {
    this->add_observer(container);
    auto comparator = SelectionPointerComparator<value_type>{};
    std::sort(this->elements.begin(), this->elements.end(), comparator);
  }
}

template <typename T>
template <typename Iterator>
Selection<T>::Selection(Iterator begin_, Iterator end_) {
  LOG_DEBUG_FUNCTION();
  this->state = SelectionState::OK;
  std::set<container_type*> parents;
  for (; begin_ != end_; ++begin_) {
    if (!(**begin_).is_deleted()) {
      this->elements.push_back(*begin_);
      parents.insert((**begin_).parent());
    }
  }
  for (auto container : parents) {
    this->add_observer(*container);
    container->on_selection_copy(*this);
    auto comparator = SelectionPointerComparator<value_type>{};
    std::sort(this->elements.begin(), this->elements.end(), comparator);
  }
}

template <typename T>
Selection<T> operator+(const Selection<T>& lhs, const Selection<T>& rhs) {
  Selection<T> result(lhs);
  result += rhs;
  return result;
}

template <typename T>
Selection<const T> operator+(const Selection<T>& lhs,
                             const Selection<const T>& rhs) {
  Selection<const T> result(rhs);
  result += lhs;
  return result;
}

template <typename T>
Selection<const T> operator+(const Selection<const T>& lhs,
                             const Selection<T>& rhs) {
  Selection<const T> result(rhs);
  result += lhs;
  return result;
}

template <typename T>
Selection<T> operator-(const Selection<T>& lhs, const Selection<T>& rhs) {
  Selection<T> result(lhs);
  result -= rhs;
  return result;
}

template <typename T>
Selection<T> operator-(const Selection<T>& lhs, const Selection<const T>& rhs) {
  Selection<T> result(lhs);
  result -= rhs;
  return result;
}

template <typename T>
Selection<const T> operator-(const Selection<const T>& lhs,
                             const Selection<T>& rhs) {
  Selection<const T> result(lhs);
  result -= rhs;
  return result;
}

template <typename T>
Selection<T> operator*(const Selection<T>& lhs, const Selection<T>& rhs) {
  Selection<T> result(lhs);
  result *= rhs;
  return result;
}

template <typename T>
Selection<T> operator*(const Selection<T>& lhs, const Selection<const T>& rhs) {
  Selection<T> result(lhs);
  result *= rhs;
  return result;
}

template <typename T>
Selection<T> operator*(const Selection<const T>& lhs, const Selection<T>& rhs) {
  Selection<T> result(rhs);
  result *= lhs;
  return result;
}

template <typename T> Container<T>::Container(int reserve) {
  LOG_DEBUG_FUNCTION();
  elements.reserve(reserve);
}

template <typename T> Container<T>::~Container() {
  LOG_DEBUG_FUNCTION();
  this->clear();
}

template <typename T>
Container<T>::Container(Container<T>&& rhs) noexcept
    : ObservableBy<SelectionBase<const T>>(std::move(rhs)),
      ObservableBy<SelectionBase<T>>(std::move(rhs)),
      elements(std::move(rhs.elements)),
      n_deleted(rhs.n_deleted) {
  LOG_DEBUG_FUNCTION();
  ObservableBy<SelectionBase<T>>::notify_all(
      &SelectionBase<T>::on_container_move, rhs, *this);
  ObservableBy<SelectionBase<const T>>::notify_all(
      &SelectionBase<const T>::on_container_move, rhs, *this);
}

template <typename T>
Container<T>::Container(const Container<T>& rhs)
    : elements(rhs.elements), n_deleted(rhs.n_deleted) {
  LOG_DEBUG_FUNCTION();
}

template <typename T>
Container<T>& Container<T>::operator=(Container<T>&& rhs) noexcept {
  LOG_DEBUG_FUNCTION();
  elements = std::move(rhs.elements);
  n_deleted = rhs.n_deleted;
  rhs.n_deleted = 0;
  ObservableBy<SelectionBase<T>>::operator=(std::move(rhs));
  ObservableBy<SelectionBase<T>>::notify_all(
      &SelectionBase<T>::on_container_move, rhs, *this);
  ObservableBy<SelectionBase<const T>>::operator=(std::move(rhs));
  ObservableBy<SelectionBase<const T>>::notify_all(
      &SelectionBase<const T>::on_container_move, rhs, *this);
  return *this;
}

template <typename T>
Container<T>& Container<T>::operator=(const Container<T>& rhs) {
  LOG_DEBUG_FUNCTION();
  elements = rhs.elements;
  n_deleted = rhs.n_deleted;
  return *this;
}

template <typename T> int Container<T>::size() const noexcept {
  return elements.size() - n_deleted;
}

template <typename T> bool Container<T>::empty() const noexcept {
  return elements.empty() || size() == 0;
}

template <typename T> void Container<T>::insert(T&& value) {
  LOG_DEBUG_FUNCTION();
  assert(ElementFlags(value).parent() == this);
  auto old_begin = elements.data();
  elements.reserve(elements.size() + 1);
  auto new_pos = elements.data();

  ptrdiff_t shift = new_pos - old_begin;
  if (shift != 0) {
    ObservableBy<SelectionBase<const T>>::notify_all(
        &SelectionBase<const T>::on_container_elements_move, old_begin,
        old_begin + elements.size(), shift);
    ObservableBy<SelectionBase<T>>::notify_all(
        &SelectionBase<T>::on_container_elements_move, old_begin,
        old_begin + elements.size(), shift);
  }
  elements.emplace_back(std::move(value));
}

template <typename T>
template <typename... Args>
T& Container<T>::emplace(Args&&... args) {
  LOG_DEBUG_FUNCTION();
  auto old_begin = elements.data();
  elements.reserve(elements.size() + 1);
  auto new_pos = elements.data();

  ptrdiff_t shift = new_pos - old_begin;
  if (shift != 0) {
    ObservableBy<SelectionBase<const T>>::notify_all(
        &SelectionBase<const T>::on_container_elements_move, old_begin,
        old_begin + elements.size(), shift);
    ObservableBy<SelectionBase<T>>::notify_all(
        &SelectionBase<T>::on_container_elements_move, old_begin,
        old_begin + elements.size(), shift);
  }
  T& ref = elements.emplace_back(T(std::forward<Args>(args)...));
  assert(ref.parent() == this);
  return ref;
}

template <typename T> void Container<T>::clear() {
  LOG_DEBUG_FUNCTION();
  int n = elements.size();
  elements.clear();
  n_deleted = 0;
  ObservableBy<SelectionBase<T>>::notify_all(
      &SelectionBase<T>::on_container_delete, *this);
  ObservableBy<SelectionBase<const T>>::notify_all(
      &SelectionBase<const T>::on_container_delete, *this);
  this->ObservableBy<SelectionBase<T>>::remove_all_observers();
  this->ObservableBy<SelectionBase<const T>>::remove_all_observers();
}

template <typename T> int Container<T>::erase(const Selection<T>& to_delete) {
  LOG_DEBUG_FUNCTION();
  int n = 0;
  for (T* el : to_delete.elements) {
    if (el->parent() == this && !el->is_deleted()) {
      el->set_deleted();
      ++n;
    }
  }
  this->n_deleted += n;
  return n;
}

template <typename T> Selection<T> Container<T>::all() {
  LOG_DEBUG_FUNCTION();
  Selection<T> s(*this);
  if (!s.empty()) {
    ObservableBy<SelectionBase<T>>::add_observer(s);
  }
  return std::move(s);
}

template <typename T> Selection<const T> Container<T>::all() const {
  LOG_DEBUG_FUNCTION();
  Selection<const T> s(*this);
  if (!s.empty()) {
    ObservableBy<SelectionBase<const T>>::add_observer(s);
  }
  return std::move(s);
}

template <typename T>
void Container<T>::on_selection_move(SelectionBase<T>& from,
                                     SelectionBase<T>& to) {
  LOG_DEBUG_FUNCTION();
  LOG_TRACE("on_selection_move");
  ObservableBy<SelectionBase<T>>::move_observer(from, to);
}

template <typename T>
void Container<T>::on_selection_move(SelectionBase<const T>& from,
                                     SelectionBase<const T>& to) const {
  LOG_DEBUG_FUNCTION();
  LOG_TRACE("const on_selection_move");
  ObservableBy<SelectionBase<const T>>::move_observer(from, to);
}

template <typename T>
void Container<T>::on_selection_delete(SelectionBase<T>& half_dead) {
  LOG_DEBUG_FUNCTION();
  LOG_TRACE("on_selection_delete");
  ObservableBy<SelectionBase<T>>::remove_observer(half_dead);
}

template <typename T>
void Container<T>::on_selection_delete(
    SelectionBase<const T>& half_dead) const {
  LOG_DEBUG_FUNCTION();
  LOG_TRACE("const on_selection_delete");
  ObservableBy<SelectionBase<const T>>::remove_observer(half_dead);
}

template <typename T>
void Container<T>::on_selection_copy(SelectionBase<T>& copy) {
  LOG_DEBUG_FUNCTION();
  LOG_TRACE("on_selection_copy");
  ObservableBy<SelectionBase<T>>::add_observer(copy);
}

template <typename T>
void Container<T>::on_selection_copy(SelectionBase<const T>& copy) const {
  LOG_DEBUG_FUNCTION();
  LOG_TRACE("const on_selection_copy");
  ObservableBy<SelectionBase<const T>>::add_observer(copy);
}
}
