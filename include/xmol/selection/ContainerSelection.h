#pragma once

#include "ContainerSelection_fwd.h"
#include "Observable.h"
#include "exceptions.h"
#include "xmol/utils/optional.h"

#include <set>

namespace xmol {
namespace selection {

enum class SelectionState { OK, HAS_DANGLING_REFERENCES };
enum class SlicingScheme{
  INTERPRET_NEGATIVE_INDICES,
  USE_INDICES_AS_IS,
};
struct NoSortTag {};

template <typename T> class ContainerRange {
public:
  ContainerRange(const ContainerRange& other) noexcept = default;
  ContainerRange(ContainerRange&& other) noexcept = default;
  ContainerRange& operator=(const ContainerRange& other) noexcept = default;
  ContainerRange& operator=(ContainerRange&& other) noexcept = default;

  T& operator*() const{
    return *it;
  }
  T* operator->() const{
    return it;
  }

  template <typename Sentinel> bool operator!=(const Sentinel&) const{
    return it!=end;
  }
  template <typename Sentinel> bool operator==(const Sentinel&) const{
    return it==end;
  }

  ContainerRange& operator++(){
    if (it!=end){
      ++it;
      while(it!=end && it->is_deleted()){
        ++it;
      }
    }
    return *this;
  }

private:
  ContainerRange(T* start, T* end) noexcept : it(start),end(end){ assert(it<=end);};
  friend class Container<typename std::remove_const<T>::type>;
  T* it;
  T* end;
};

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

  int size() const;

private:
  explicit SelectionRange(const SelectionBase<T>& selection, int pos, int end,
                          int step);
  friend class SelectionBase<T>;
  const SelectionBase<T>* selection;
  int pos;
  int m_end;
  int step;
};

template <typename T>
class SelectionBase
    : public Observable<typename SelectionTraits<T>::container_type> {
public:
  static_assert(!std::is_reference<T>::value);
  static_assert(!std::is_pointer<T>::value);
  using value_type = typename SelectionTraits<T>::value_type;
  using container_type = typename SelectionTraits<T>::container_type;
  using selection_element_type =
      typename SelectionTraits<T>::selection_element_type;

  SelectionBase();
  template <typename U = T,
            typename SFINAE = typename std::enable_if<std::is_const<U>::value>::type>
  SelectionBase(
      const SelectionBase<typename SelectionBase<U>::value_type>& rhs);
  SelectionBase(const SelectionBase& rhs);
  SelectionBase(SelectionBase&& rhs) noexcept;
  ~SelectionBase();

  SelectionBase& operator=(const SelectionBase& rhs);
  SelectionBase& operator=(SelectionBase&& rhs) noexcept;

  int count(const T&) const;

  bool empty() const ;
  int size() const ;
  void clear() ;
  bool is_valid() ;

  SelectionRange<T> begin() const;
  SelectionRange<T> end() const;
  SelectionRange<T> slice_range(xmol::utils::optional<int> first = {}, xmol::utils::optional<int> last = {},
                                xmol::utils::optional<int> stride = {}, const SlicingScheme& = SlicingScheme::INTERPRET_NEGATIVE_INDICES) &;
  SelectionRange<T> slice_range(xmol::utils::optional<int> first = {}, xmol::utils::optional<int> last = {},
                                xmol::utils::optional<int> stride = {}, const SlicingScheme& = SlicingScheme::INTERPRET_NEGATIVE_INDICES) const&;
  SelectionRange<T> slice_range(xmol::utils::optional<int> first = {}, xmol::utils::optional<int> last = {},
                                xmol::utils::optional<int> stride = {}, const SlicingScheme& = SlicingScheme::INTERPRET_NEGATIVE_INDICES) && = delete;
  SelectionRange<T> slice_range(xmol::utils::optional<int> first = {}, xmol::utils::optional<int> last = {},
                                xmol::utils::optional<int> stride = {}, const SlicingScheme& = SlicingScheme::INTERPRET_NEGATIVE_INDICES) const&& = delete;

  T& operator[](int i) const;

protected:
  //  explicit SelectionBase(container_type& container);

  void on_container_move(container_type& from, container_type& to);
  void on_container_delete(container_type& half_dead);
  void on_container_elements_move(selection_element_type old_begin, selection_element_type old_end,
                                  selection_element_type new_begin);
  void remove_redundant_observers();

  friend class SelectionRange<T>;
  friend class SelectionBase<value_type>;
  friend class SelectionBase<const value_type>;
  friend class Container<value_type>;

  std::vector<selection_element_type> elements;
  SelectionState state = SelectionState::OK;
};

template <typename T> class Selection : public SelectionBaseExtension<T> {
  static_assert(
      std::is_base_of<SelectionBase<T>, SelectionBaseExtension<T>>::value, "");
  static_assert(std::is_default_constructible<SelectionBaseExtension<T>>::value);
  static_assert(
      std::is_constructible<SelectionBaseExtension<T>,
                            const SelectionBaseExtension<
                                typename std::remove_const<T>::type>&>::value,
      "");
  static_assert(std::is_copy_constructible<SelectionBaseExtension<T>>::value);
  static_assert(std::is_move_constructible<SelectionBaseExtension<T>>::value);
  static_assert(std::is_move_assignable<SelectionBaseExtension<T>>::value);
  static_assert(std::is_copy_assignable<SelectionBaseExtension<T>>::value);

public:
  using value_type = typename SelectionTraits<T>::value_type;
  using container_type = typename SelectionTraits<T>::container_type;
  using selection_element_type =
      typename SelectionTraits<T>::selection_element_type;

  Selection() = default;
  template <typename U = T,
            typename SFINAE = typename std::enable_if<std::is_const<U>::value>::type>
  Selection(const Selection<typename Selection<U>::value_type>& rhs)
      : SelectionBaseExtension<T>(rhs){};
  Selection(const Selection& rhs) = default;
  Selection(Selection&& rhs) noexcept = default;

  Selection& operator=(const Selection& rhs) = default;
  Selection& operator=(Selection&& rhs) = default;

  template <typename Predicate> Selection& remove_if(Predicate&& p);

  template <typename Predicate> Selection filter(Predicate&& p) const;

  template <typename Predicate> std::vector<char> indicator(Predicate&& p) const;
  template <typename Predicate> std::vector<int> index(Predicate&& p) const;

  template <typename Transform> Selection& for_each(Transform&& transform);

  template <typename Transform>
  const Selection& for_each(Transform&& transform) const;

  template <typename V, typename U = T,
            typename SFINAE = typename std::enable_if<std::is_same<
                typename std::remove_const<U>::type, typename std::remove_const<V>::type>::value>::type>
  bool operator==(const Selection<V>& rhs) const;
  template <typename V, typename U = T,
            typename SFINAE = typename std::enable_if<std::is_same<
                typename std::remove_const<U>::type, typename std::remove_const<V>::type>::value>::type>
  bool operator!=(const Selection<V>& rhs) const;

  template <typename V, typename U = T,
            typename SFINAE = typename std::enable_if<std::is_same<
                typename std::remove_const<U>::type, typename std::remove_const<V>::type>::value>::type>
  Selection<U>& operator+=(const Selection<V>& rhs);
  template <typename V, typename U = T,
            typename SFINAE = typename std::enable_if<std::is_same<
                typename std::remove_const<U>::type, typename std::remove_const<V>::type>::value>::type>
  Selection<U>& operator-=(const Selection<V>& rhs);
  template <typename V, typename U = T,
            typename SFINAE = typename std::enable_if<std::is_same<
                typename std::remove_const<U>::type, typename std::remove_const<V>::type>::value>::type>
  Selection<U>& operator*=(const Selection<V>& rhs);

  Selection slice(xmol::utils::optional<int> first = {}, xmol::utils::optional<int> last = {},
                  xmol::utils::optional<int> stride = {}, const SlicingScheme& = SlicingScheme::INTERPRET_NEGATIVE_INDICES) const;

  template<typename CharIterator,
      typename SFINAE = typename std::enable_if<
          std::is_convertible<decltype(*std::declval<CharIterator>()), bool>::value
      >::type>
  Selection filter(CharIterator begin, CharIterator end) const;


  template<typename Iterator,
      typename SFINAE = typename std::enable_if<
          std::is_convertible<decltype(*std::declval<Iterator>()), int>::value
      >::type>
  Selection at_index(Iterator begin, Iterator end) const;

  template <typename Iterator>
  explicit Selection(Iterator begin_, Iterator end_);

  explicit Selection(SelectionRange<T> rng);

  template <typename Iterator> explicit Selection(Iterator begin_, Iterator end_, const NoSortTag&);

protected:
  friend class Selection<value_type>;
  friend class Selection<const value_type>;
  friend class Container<value_type>;

  explicit Selection(typename SelectionBase<T>::container_type& container);
};

template <typename T>
class Container : public Observable<SelectionBase<T>>,
                  public Observable<SelectionBase<const T>> {
public:
  using value_type = typename SelectionTraits<T>::value_type;

  explicit Container(int reserve = 0);
  ~Container();
  Container(Container&& rhs) noexcept;
  Container(const Container& rhs);

  Container& operator=(Container&& rhs) noexcept;
  Container& operator=(const Container& rhs);

  int size() const;
  bool empty() const;

  void insert(T&& value);

  template <typename... Args> T& emplace(Args&&... args);

  void clear();
  int erase(const Selection<T>& to_delete);

  Selection<T> all();
  Selection<const T> all() const;

  ContainerRange<T> begin(){
    return ContainerRange<T>(elements.data(),elements.data()+elements.size());
  }
  ContainerRange<const T> begin() const{
    return ContainerRange<const T>(elements.data(),elements.data()+elements.size());
  }
  ContainerRange<T> end(){
    return ContainerRange<T>(elements.data()+elements.size(),elements.data()+elements.size());
  }
  ContainerRange<const T> end()const{
    return ContainerRange<const T>(elements.data()+elements.size(),elements.data()+elements.size());
  }

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
SelectionRange<T>::SelectionRange(const SelectionBase<T>& selection, int pos, int end, int step)
    : selection(&selection), pos(pos), m_end(end), step(step) {}

template <typename T> T& SelectionRange<T>::operator*() const {
  if (this->selection->elements[pos]->is_deleted()) {
    throw make_deleted_element_access<T>("Invalid access to deleted element");
  }
  return *this->selection->elements[pos];
}
template <typename T> T* SelectionRange<T>::operator->() const {
  if (this->selection->elements[pos]->is_deleted()) {
    throw make_deleted_element_access<T>("Invalid access to deleted element");
  }
  return this->selection->elements[pos];
}

template <typename T>
template <typename Sentinel>
bool SelectionRange<T>::operator!=(const Sentinel&) const {
  if (step > 0) {
    return pos < m_end;
  } else {
    return pos > m_end;
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

template <typename T> int SelectionRange<T>::size() const {
  int n = 0;
  if (step > 0) {
    n = (m_end - pos + step - 1) / step;
  } else {
    n = (m_end - pos + step + 1) / step;
  }
  if (n > 0) {
    return n;
  }
  return 0;
}

template <typename T> SelectionRange<T> SelectionBase<T>::begin() const {
  if (state != SelectionState::OK) {
    throw make_dead_selection_access<T>("Dead selection access in ::begin");
  }
  return SelectionRange<T>(*this, 0, elements.size(), 1);
}

template <typename T> SelectionRange<T> SelectionBase<T>::end() const {
  if (state != SelectionState::OK) {
    throw make_dead_selection_access<T>("Dead selection access in ::end");
  }
  return SelectionRange<T>(*this, elements.size(), 0, 1);
}

template <typename T>
SelectionRange<T> SelectionBase<T>::slice_range(xmol::utils::optional<int> first, xmol::utils::optional<int> last,
                                                xmol::utils::optional<int> stride, const SlicingScheme& index_scheme)& {
  return const_cast<const SelectionBase<T>*>(this)->slice_range(first, last, stride, index_scheme);
}

template <typename T>
SelectionRange<T> SelectionBase<T>::slice_range(xmol::utils::optional<int> first, xmol::utils::optional<int> last,
                                                xmol::utils::optional<int> stride, const SlicingScheme& index_scheme) const& {
  Expects(index_scheme==SlicingScheme::INTERPRET_NEGATIVE_INDICES || (first && last && stride));
  if (state != SelectionState::OK) {
    throw make_dead_selection_access<T>("Dead selection access in ::begin");
  }

  const auto N = elements.size();
  if (index_scheme==SlicingScheme::INTERPRET_NEGATIVE_INDICES){
    if (!stride) {
      stride = 1;
    }

    if (last && last.value() < 0) {
      last = N + last.value();
    }
    if (first && first.value() < 0) {
      first = N + first.value();
    }

    if (!first) {
      if (stride > 0) {
        first = 0;
      } else {
        first = N - 1;
      }
    }
    if (!last) {
      if (stride > 0) {
        last = N;
      } else {
        last = -1;
      }
    }
  }
  return SelectionRange<T>(*this, first.value(), last.value(), stride.value());
}

template <typename T> T& SelectionBase<T>::operator[](int i) const {
  if (state != SelectionState::OK) {
    throw make_dead_selection_access<T>("Dead selection access in ::operator[]");
  }
  if (i < -size() || i >= size()) {
    throw make_selection_out_of_range<T>("");
  }
  if (i < 0) {
    i = size() + i;
  }
  if (elements[i]->is_deleted()) {
    throw make_deleted_element_access<T>("Invalid access to deleted element");
  }
  return *elements[i];
}

template <typename T> SelectionBase<T>::SelectionBase() {
  state = SelectionState::OK;
}

template <typename T> SelectionBase<T>::~SelectionBase() {
  this->clear();
}

template <typename T>
SelectionBase<T>::SelectionBase(const SelectionBase<T>& rhs)
    : Observable<container_type>(rhs),
      elements(rhs.elements),
      state(rhs.state) {
  this->notify(static_cast<typename SelectionTraits<T>::on_selection_copy_type>(&container_type::on_selection_copy),
               *this);
}

template <typename T>
template <typename U, typename SFINAE>
SelectionBase<T>::SelectionBase(
    const SelectionBase<typename SelectionBase<U>::value_type>& rhs)
    : elements(rhs.elements.begin(), rhs.elements.end()), state(rhs.state) {

  this->observers.insert(rhs.observers.begin(), rhs.observers.end());
  this->notify(static_cast<typename SelectionTraits<U>::on_selection_copy_type>(&container_type::on_selection_copy),
               *this);
}

template <typename T>
SelectionBase<T>::SelectionBase(SelectionBase<T>&& rhs) noexcept
    : Observable<container_type>(std::move(rhs)),
      elements(std::move(rhs.elements)),
      state(rhs.state) {
  this->notify(static_cast<typename SelectionTraits<T>::on_selection_move_type>(&container_type::on_selection_move),
               rhs, *this);
  rhs.state = SelectionState::OK;
}
template <typename T>
SelectionBase<T>& SelectionBase<T>::operator=(SelectionBase<T>&& rhs) noexcept {
  if (&rhs == this) {
    return *this;
  }
  this->clear();
  Observable<container_type>::operator=(std::move(rhs));
  Observable<container_type>::notify(
      static_cast<typename SelectionTraits<T>::on_selection_move_type>(&container_type::on_selection_move), rhs, *this);
  elements = std::move(rhs.elements);
  state = rhs.state;
  rhs.state = SelectionState::OK;
  return *this;
}

template <typename T>
SelectionBase<T>& SelectionBase<T>::operator=(const SelectionBase<T>& rhs) {
  if (&rhs == this) {
    return *this;
  }
  this->clear();
  Observable<container_type>::operator=(rhs);
  this->notify(&container_type::on_selection_copy, *this);
  elements = rhs.elements;
  state = rhs.state;
  return *this;
}

template <typename T>
template <typename V, typename U, typename SFINAE>
bool Selection<T>::operator==(const Selection<V>& rhs) const {
  return this->elements.size() == rhs.size() &&
         std::equal(this->elements.begin(), this->elements.end(),
                    rhs.elements.begin());
}

template <typename T>
template <typename V, typename U, typename SFINAE>
bool Selection<T>::operator!=(const Selection<V>& rhs) const {
  return this->elements.size() != rhs.size() ||
         !std::equal(this->elements.begin(), this->elements.end(),
                     rhs.elements.begin());
}

template <typename T>
template <typename V, typename U, typename SFINAE>
Selection<U>& Selection<T>::operator+=(const Selection<V>& rhs) {
  auto comparator = SelectionPointerComparator<value_type>{};
  static_cast<void>(comparator);

  for (auto cs : rhs.observers) {
    this->add_observer(*cs.first);
    cs.first->Observable<SelectionBase<T>>::add_observer(*this);
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
  auto comparator = SelectionPointerComparator<value_type>{};
  static_cast<void>(comparator);
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
  auto comparator = SelectionPointerComparator<value_type>{};
  static_cast<void>(comparator);
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
Selection<T> Selection<T>::slice(xmol::utils::optional<int> first, xmol::utils::optional<int> last,
                                 xmol::utils::optional<int> stride, const SlicingScheme& index_scheme) const {
  return Selection<T>(this->slice_range(first, last, stride, index_scheme));
}

template <typename T>
template <typename CharIterator, typename SFINAE>
Selection<T> Selection<T>::filter(CharIterator begin, CharIterator end) const {

  std::vector<T*> pointers;
  auto it = this->begin();
  auto this_end = this->end();
  while (it != this_end && begin!=end){
    if (*begin){
      pointers.push_back(&(*it));
    }
    ++it;
    ++begin;
  }
  if (begin!=end){
    throw SelectionException<T>("Selection::filter(index_begin, index_end): index array size is too small");
  }

  if (it!=this_end){
    throw SelectionException<T>("Selection::filter(index_begin, index_end): index array size is too large");
  }

  return Selection(pointers.begin(), pointers.end());
}


template <typename T>
template <typename CharIterator, typename SFINAE>
Selection<T> Selection<T>::at_index(CharIterator begin, CharIterator end) const {
  std::vector<T*> pointers;
  while (begin!=end){
    pointers.push_back(&(*this)[*begin]);
    ++begin;
  }
  return Selection(pointers.begin(), pointers.end());
}


template <typename T>
template <typename Predicate>
Selection<T>& Selection<T>::remove_if(Predicate&& p) {
  if (GSL_UNLIKELY(this->state != SelectionState::OK)) {
    throw make_dead_selection_access<T>("Dead selection access in ::remove_if");
  }
  auto end = std::remove_if(this->elements.begin(), this->elements.end(),
                            [&p](selection_element_type ptr) {
                              if (GSL_UNLIKELY(ptr->is_deleted())) {
                                throw make_deleted_element_access<T>(
                                    "Invalid access to deleted element in ::remove_if");
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
  Selection<T> result(*this);
  result.remove_if([&p](const value_type& val) { return !p(val); });
  return result;
}

template <typename T>
template <typename Predicate>
std::vector<char> Selection<T>::indicator(Predicate&& p) const {
  std::vector<char> result(this->size());
  for (int i=0;i<this->size();i++){
    result[i] = static_cast<char>(p((*this)[i]));
  }
  return result;
}


template <typename T>
template <typename Predicate>
std::vector<int> Selection<T>::index(Predicate&& p) const {
  std::vector<int> result;
  for (int i=0;i<this->size();i++){
    if (p((*this)[i])){
      result.push_back(i);
    }
  }
  return result;
}

template <typename T>
template <typename Transform>
Selection<T>& Selection<T>::for_each(Transform&& transform) {
  for (auto& value : *this) {
    transform(value);
  }
  this->remove_redundant_observers();
  return *this;
}

template <typename T>
template <typename Transform>
const Selection<T>& Selection<T>::for_each(Transform&& transform) const {
  for (auto& value : *this) {
    transform(value);
  }
  return *this;
}

template <typename T> void SelectionBase<T>::remove_redundant_observers() {
  std::set<container_type*> new_observers;
  for (auto el_ptr : this->elements) {
    new_observers.insert(el_ptr->parent());
  }

  std::set<container_type*> observers_to_del;
  for (auto cs : this->observers) {
    if (!new_observers.count(cs.first)) {observers_to_del.insert(cs.first);}
  }

  for (auto o : observers_to_del){
    this->remove_observer(*o);
    o->Observable<SelectionBase<T>>::remove_observer(*this);
  }
}

template <typename T> int SelectionBase<T>::count(const T& value) const {
  if (state != SelectionState::OK) {
    throw make_dead_selection_access<T>("Dead selection access in ::count");
  }
  return std::find_if(elements.begin(), elements.end(),
                      [&value](const selection_element_type el) {
                        return *el == value;
                      }) == elements.end()
             ? 0
             : 1;
}

template <typename T> bool SelectionBase<T>::empty() const {
  return elements.empty();
}

template <typename T> int SelectionBase<T>::size() const {
  return elements.size();
}

template <typename T> void SelectionBase<T>::clear() {
  elements.clear();
  this->template notify<ObserverState::ACTIVE>(
      static_cast<typename SelectionTraits<T>::on_selection_delete_type>(&container_type::on_selection_delete), *this);
  this->clear_observers();
  state = SelectionState::OK;
}

template <typename T> bool SelectionBase<T>::is_valid() {
  return state == SelectionState::OK;
}

template <typename T>
void SelectionBase<T>::on_container_move(SelectionBase<T>::container_type& from,
                                         SelectionBase<T>::container_type& to) {
  this->move_observer(from, to);
}
template <typename T>
void SelectionBase<T>::on_container_elements_move(selection_element_type old_begin, selection_element_type old_end,
                                                  selection_element_type new_begin) {
  for (auto& ptr : elements) {
    if (old_begin <= ptr && ptr < old_end) {
      ptr = new_begin + (ptr - old_begin);
    }
  }
}

template <typename T>
void SelectionBase<T>::on_container_delete(
    SelectionBase<T>::container_type& half_dead) {
  this->invalidate_observer(half_dead);
  state = SelectionState::HAS_DANGLING_REFERENCES;
}

template <typename T>
Selection<T>::Selection(typename SelectionBase<T>::container_type& container) {
  this->state = SelectionState::OK;
  for (T& el : container.elements) {
    if (!el.is_deleted()) {
      this->elements.push_back(&el);
    }
  }
  if (!this->empty()) {
    this->add_observer(container);
    auto comparator = SelectionPointerComparator<value_type>{};
    static_cast<void>(comparator);
    std::sort(this->elements.begin(), this->elements.end(), comparator);
  }
}

template <typename T>
template <typename Iterator>
Selection<T>::Selection(Iterator begin_, Iterator end_) {
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
  }
  auto comparator = SelectionPointerComparator<value_type>{};
  static_cast<void>(comparator);
  std::sort(this->elements.begin(), this->elements.end(), comparator);
  auto end = std::unique(this->elements.begin(), this->elements.end());
  this->elements.erase(end, this->elements.end());
}

template <typename T> Selection<T>::Selection(SelectionRange<T> rng) {
  this->state = SelectionState::OK;
  std::set<container_type*> parents;
  for (; rng != rng; ++rng) {
    if (!(*rng).is_deleted()) {
      this->elements.push_back(std::addressof(*rng));
      parents.insert((*rng).parent());
    }
  }
  for (auto container : parents) {
    this->add_observer(*container);
    container->on_selection_copy(*this);
  }
  auto comparator = SelectionPointerComparator<value_type>{};
  static_cast<void>(comparator);
  std::sort(this->elements.begin(), this->elements.end(), comparator);
}

template <typename T>
template <typename Iterator>
Selection<T>::Selection(Iterator begin_, Iterator end_, const NoSortTag&) {
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
  }
  auto comparator = SelectionPointerComparator<value_type>{};
  static_cast<void>(comparator);
  assert(std::is_sorted(this->elements.begin(), this->elements.end(), comparator));
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
  elements.reserve(reserve);
}

template <typename T> Container<T>::~Container() {
  this->clear();
}

template <typename T>
Container<T>::Container(Container<T>&& rhs) noexcept
    : Observable<SelectionBase<T>>(std::move(rhs)), Observable<SelectionBase<const T>>(std::move(rhs)),
      elements(std::move(rhs.elements)),
      n_deleted(rhs.n_deleted) {
  Observable<SelectionBase<T>>::notify(&SelectionBase<T>::on_container_move, rhs, *this);
  Observable<SelectionBase<const T>>::notify(&SelectionBase<const T>::on_container_move, rhs, *this);
}

template <typename T>
Container<T>::Container(const Container<T>& rhs)
    : elements(rhs.elements), n_deleted(rhs.n_deleted) {
  }

template <typename T>
Container<T>& Container<T>::operator=(Container<T>&& rhs) noexcept {
  elements = std::move(rhs.elements);
  n_deleted = rhs.n_deleted;
  rhs.n_deleted = 0;
  Observable<SelectionBase<T>>::operator=(std::move(rhs));
  Observable<SelectionBase<T>>::notify(&SelectionBase<T>::on_container_move, rhs, *this);
  Observable<SelectionBase<const T>>::operator=(std::move(rhs));
  Observable<SelectionBase<const T>>::notify(&SelectionBase<const T>::on_container_move, rhs, *this);
  return *this;
}

template <typename T>
Container<T>& Container<T>::operator=(const Container<T>& rhs) {
  elements = rhs.elements;
  n_deleted = rhs.n_deleted;
  return *this;
}

template <typename T> int Container<T>::size() const {
  return elements.size() - n_deleted;
}

template <typename T> bool Container<T>::empty() const {
  return elements.empty() || size() == 0;
}

template <typename T> void Container<T>::insert(T&& value) {
  auto old_begin = elements.data();
  auto old_end = elements.data() + elements.size();
  elements.emplace_back(std::move(value));
  auto new_begin = elements.data();

  if (old_begin != new_begin) {
    Observable<SelectionBase<const T>>::notify(&SelectionBase<const T>::on_container_elements_move, old_begin,
                                                 old_end, new_begin);
    Observable<SelectionBase<T>>::notify(&SelectionBase<T>::on_container_elements_move, old_begin, old_end,
                                           new_begin);
  }
}

template <typename T>
template <typename... Args>
T& Container<T>::emplace(Args&&... args) {
  auto old_begin = elements.data();
  auto old_end = elements.data() + elements.size();
  elements.emplace_back(T(std::forward<Args>(args)...));
  T& ref = elements.back();
  auto new_begin = elements.data();

  if (old_begin != new_begin) {
    Observable<SelectionBase<const T>>::notify(&SelectionBase<const T>::on_container_elements_move, old_begin,
                                                 old_end, new_begin);
    Observable<SelectionBase<T>>::notify(&SelectionBase<T>::on_container_elements_move, old_begin, old_end,
                                           new_begin);
  }
  return ref;
}

template <typename T> void Container<T>::clear() {
  elements.clear();
  n_deleted = 0;
  Observable<SelectionBase<T>>::notify(&SelectionBase<T>::on_container_delete, *this);
  Observable<SelectionBase<const T>>::notify(&SelectionBase<const T>::on_container_delete, *this);
  this->Observable<SelectionBase<T>>::clear_observers();
  this->Observable<SelectionBase<const T>>::clear_observers();
}

template <typename T> int Container<T>::erase(const Selection<T>& to_delete) {
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
  Selection<T> s(*this);
  if (!s.empty()) {
    Observable<SelectionBase<T>>::add_observer(s);
  }
  return std::move(s);
}

template <typename T> Selection<const T> Container<T>::all() const {
  Selection<const T> s(*this);
  if (!s.empty()) {
    Observable<SelectionBase<const T>>::add_observer(s);
  }
  return std::move(s);
}

template <typename T>
void Container<T>::on_selection_move(SelectionBase<T>& from,
                                     SelectionBase<T>& to) {
  Observable<SelectionBase<T>>::move_observer(from, to);
}

template <typename T>
void Container<T>::on_selection_move(SelectionBase<const T>& from,
                                     SelectionBase<const T>& to) const {
  Observable<SelectionBase<const T>>::move_observer(from, to);
}

template <typename T>
void Container<T>::on_selection_delete(SelectionBase<T>& half_dead) {
  Observable<SelectionBase<T>>::remove_observer(half_dead);
}

template <typename T>
void Container<T>::on_selection_delete(
    SelectionBase<const T>& half_dead) const {
  Observable<SelectionBase<const T>>::remove_observer(half_dead);
}

template <typename T>
void Container<T>::on_selection_copy(SelectionBase<T>& copy) {
  Observable<SelectionBase<T>>::add_observer(copy);
}

template <typename T>
void Container<T>::on_selection_copy(SelectionBase<const T>& copy) const {
  Observable<SelectionBase<const T>>::add_observer(copy);
}
}
}
