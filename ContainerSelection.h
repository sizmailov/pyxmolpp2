#pragma once

#include <set>
#include "Observer.h"


namespace selection {

template<typename T>
class Container;

template<typename T>
class Selection;


template<typename T>
struct ElementWithFlags{
  static_assert(!std::is_const_v<T>);
  static_assert(!std::is_reference_v<T>);
  static_assert(!std::is_pointer_v<T>);

  using container_type = Container<T>;

  explicit ElementWithFlags(T&& value): value(std::move(value)),is_deleted(false){
  }
  explicit ElementWithFlags(const T& value): value(std::move(value)),is_deleted(false){
  }
  T value;
  bool is_deleted = false;

  bool operator<(const ElementWithFlags<T>& rhs) const noexcept ;

private:
  friend class Selection<T>;
  const container_type* parent() const noexcept;
  container_type* parent() noexcept;


  struct PtrComparator{
    bool operator()(const ElementWithFlags<T>* lhs, const ElementWithFlags<T>* rhs){

      if (lhs == nullptr) {
        return false;
      }

      if (rhs == nullptr) {
        return true;
      }

      return *lhs < *rhs;
    }
  };
};


template<typename T>
struct SelectionTraits{
  static_assert(!std::is_reference_v<T>);
  static_assert(!std::is_pointer_v<T>);
  using value_type = std::remove_const_t<T>;
  using container_type = std::conditional_t<std::is_const_v<T>,const Container<value_type>,Container<value_type>>;
  using element_type = std::conditional_t<std::is_const_v<T>,
                                          const ElementWithFlags<value_type>,
                                          ElementWithFlags<value_type>>;

  using on_selection_move_type =std::conditional_t<std::is_const_v<T>,
      void (container_type::*)(Selection<const value_type>&,Selection<const value_type>&) const,
      void (container_type::*)(Selection<value_type>&, Selection<value_type>&)
  >;

  using on_selection_copy_type =std::conditional_t<std::is_const_v<T>,
                                                   void (container_type::*)(Selection<const value_type>&) const,
                                                   void (container_type::*)(Selection<value_type>&)
  >;


  using on_selection_delete_type =std::conditional_t<std::is_const_v<T>,
                                                   void (container_type::*)(Selection<const value_type>&) const,
                                                   void (container_type::*)(Selection<value_type>&)
  >;
};


enum class SelectionState{
  OK,
  HAS_DANGLING_REFERENCES
};


template<typename T>
class SelectionRange {
public:
  SelectionRange(const SelectionRange& other) noexcept = default;
  SelectionRange(SelectionRange&& other) noexcept = default;
  SelectionRange& operator=(const SelectionRange& other) noexcept  = default;
  SelectionRange& operator=(SelectionRange&& other) noexcept  = default;

  T& operator*() const;
  T* operator->() const;
  template<typename Sentinel>
  bool operator!=(const Sentinel&) const;

  SelectionRange& operator++();
  SelectionRange& operator--();

  SelectionRange operator-(int n);
  SelectionRange operator+(int n);
  SelectionRange& operator+=(int n);
  SelectionRange& operator-=(int n);

private:
  explicit SelectionRange(const Selection<T>& selection, int pos, int end, int step);
  friend class Selection<T>;
  const Selection<T>* selection;
  int pos;
  int end;
  int step;
};

template<typename T>
class Selection : public ObservableBy<typename SelectionTraits<T>::container_type> {
public:
  static_assert(!std::is_reference_v<T>);
  static_assert(!std::is_pointer_v<T>);
  using value_type = typename SelectionTraits<T>::value_type;
  using container_type = typename SelectionTraits<T>::container_type;
  using element_type = typename SelectionTraits<T>::element_type;

  Selection();
  template<typename U=T, typename SFINAE=std::enable_if_t<std::is_const_v<U>>>
  Selection(const Selection<typename Selection<U>::value_type>& rhs);
  Selection(const Selection& rhs);
  Selection(Selection&& rhs) noexcept;
  ~Selection();

  Selection& operator=(const Selection& rhs);
  Selection& operator=(Selection&& rhs) noexcept;

  bool operator==(const Selection& rhs) const;
  bool operator!=(const Selection& rhs) const;

  Selection& operator+=(const Selection& rhs);
  Selection& operator-=(const Selection& rhs);
  Selection& operator*=(const Selection& rhs);

  size_t count(T&) const;

  bool empty() const;
  size_t size() const;
  void clear() noexcept ;


  SelectionRange<T> begin();
  SelectionRange<T> end();

  T& operator[](int i) const;

private:
  explicit Selection(container_type& container);

  void on_container_move(container_type& from, container_type& to);
  void on_container_delete(container_type& half_dead);
  void on_container_elements_move(element_type* old_begin, element_type* old_end, ptrdiff_t shift);
  void remove_redundant_observers();

  friend class SelectionRange<T>;
  friend class Selection<const T>;
  friend class Container<value_type>;

  std::vector<element_type*> elements;
  SelectionState state = SelectionState::OK;

};


template<typename T>
class Container : public ObservableBy<Selection<T>>, public ObservableBy<Selection<const T>>{
public:
  using value_type = typename SelectionTraits<T>::value_type;
  using element_type = typename SelectionTraits<T>::element_type;

  Container();
  ~Container();
  Container(Container&& rhs) noexcept ;
  Container(const Container& rhs);

  Container& operator=(Container&& rhs) noexcept ;
  Container& operator=(const Container& rhs);

  size_t size() const;
  bool empty() const;

  void insert(T&& value);

  template<typename ...Args>
  void emplace(Args&& ...args);

  size_t erase();
  size_t erase(Selection<T>& selection);

  Selection<T> all();
  Selection<const T> all() const;

  friend class Selection<T>;
  friend class Selection<const T>;

protected:


  void on_selection_move(Selection<T>& from, Selection<T>& to);
  void on_selection_move(Selection<const T>& from, Selection<const T>& to) const;

  void on_selection_copy(Selection<T>& copy);
  void on_selection_copy(Selection<const T>& copy) const;

  void on_selection_delete(Selection<T>& half_dead);
  void on_selection_delete(Selection<const T>& half_dead) const;


  std::vector<element_type> elements;
  size_t deleted = 0;
};


template<typename  T>
SelectionRange<T>::SelectionRange(const Selection<T>& selection, int pos, int end, int step) :
    selection(&selection),
    pos(pos),
    end(end),
    step(step) {
}

template<typename  T>
T& SelectionRange<T>::operator*() const {
  assert(!this->selection->elements[pos]->is_deleted);
  return this->selection->elements[pos]->value;
}
template<typename  T>
T* SelectionRange<T>::operator->() const {
  assert(!this->selection->elements[pos].is_deleted);
  return this->selection->elements[pos].value;
}

template<typename T>
template<typename Sentinel>
bool SelectionRange<T>::operator!=(const Sentinel&) const {
  if (step>0){
    return pos<end;
  }else{
    return pos>end;
  }
}


template<typename T>
SelectionRange<T>& SelectionRange<T>::operator++() {
  pos+=step;
  return *this;
}

template<typename T>
SelectionRange<T>& SelectionRange<T>::operator--() {
  pos-=step;
  return *this;
}

template<typename T>
SelectionRange<T>& SelectionRange<T>::operator+=(int n) {
  pos+=step*n;
  return *this;
}

template<typename T>
SelectionRange<T>& SelectionRange<T>::operator-=(int n) {
  pos-=step*n;
  return *this;
}

template<typename T>
SelectionRange<T> SelectionRange<T>::operator-(int n) {
  SelectionRange<T> result(*this);
  result-=n;
  return result;
}

template<typename T>
SelectionRange<T> SelectionRange<T>::operator+(int n) {
  SelectionRange<T> result(*this);
  result+=n;
  return result;
}

template<typename T>
SelectionRange<T> Selection<T>::begin(){
  LOG_VERBOSE_FUNCTION();
  assert(state==SelectionState::OK);
  return SelectionRange(*this,0,elements.size(),1);
}

template<typename T>
SelectionRange<T> Selection<T>::end(){
  LOG_VERBOSE_FUNCTION();
  assert(state==SelectionState::OK);
  return SelectionRange(*this,elements.size(),0,1);
}

template<typename T>
T& Selection<T>::operator[](int i) const{
  LOG_VERBOSE_FUNCTION();
  assert(state==SelectionState::OK);
  assert(i>-elements.size() && i < elements.size());
  if (i<0){
    i = static_cast<int>(elements.size())-i;
  }
  return elements[i];
}

template<typename T>
Selection<T>::Selection() {
  LOG_DEBUG_FUNCTION();
  state=SelectionState::OK;
}

template<typename T>
Selection<T>::~Selection() {
  LOG_DEBUG_FUNCTION();
  this->clear();
}

template<typename T>
Selection<T>::Selection(const Selection<T>& rhs): ObservableBy<container_type>(rhs), state(rhs.state), elements(rhs.elements) {
  LOG_DEBUG_FUNCTION();
  this->notify_all(static_cast<typename SelectionTraits<T>::on_selection_copy_type>(&container_type::on_selection_copy),*this);
}

template<typename T>
template<typename U, typename SFINAE>
Selection<T>::Selection(const Selection<typename Selection<U>::value_type>& rhs): state(rhs.state), elements(rhs.elements.begin(),rhs.elements.end()) {
  LOG_DEBUG_FUNCTION();
  this->notify_all(static_cast<typename SelectionTraits<U>::on_selection_copy_type>(&container_type::on_selection_copy),*this);
}

template<typename T>
Selection<T>::Selection(Selection<T>&& rhs) noexcept : ObservableBy<container_type>(std::move(rhs)), state(rhs.state), elements(std::move(rhs.elements)){
  LOG_DEBUG_FUNCTION();
  this->notify_all(static_cast<typename SelectionTraits<T>::on_selection_move_type>(&container_type::on_selection_move),rhs,*this);
  rhs.state=SelectionState::OK;
}
template<typename T>
Selection<T>& Selection<T>::operator=(Selection<T>&& rhs) noexcept {
  LOG_DEBUG_FUNCTION();
  if (&rhs==this){
    return *this;
  }
  this->clear();
  ObservableBy<container_type>::operator=(std::move(rhs));
  ObservableBy<container_type>::notify_all(&container_type::on_selection_move,rhs,*this);
  elements = std::move(rhs.elements);
  state = rhs.state;
  rhs.state=SelectionState::OK;
}

template<typename T>
Selection<T>& Selection<T>::operator=(const Selection<T>& rhs){
  LOG_DEBUG_FUNCTION();
  if (&rhs==this){
    return *this;
  }
  this->clear();
  ObservableBy<container_type>::operator=(rhs);
  this->notify_all(&container_type::on_selection_copy,*this);
  elements =rhs.elements;
  state = rhs.state;
}


template<typename T>
bool Selection<T>::operator==(const Selection<T>& rhs) const{
  LOG_DEBUG_FUNCTION();
  return elements==rhs.elements;
}

template<typename T>
bool Selection<T>::operator!=(const Selection<T>& rhs) const{
  LOG_DEBUG_FUNCTION();
  return elements!=rhs.elements;
}

template<typename T>
Selection<T>& Selection<T>::operator+=(const Selection<T>& rhs) {
  LOG_DEBUG_FUNCTION();
  auto comparator = typename Selection<T>::element_type::PtrComparator{};

  for (auto [container,state]: rhs.observers){
    this->add_observer(*container);
    container->ObservableBy<Selection<T>>::add_observer(*this);
  }

  assert(std::is_sorted(elements.begin(), elements.end(),comparator));
  assert(std::is_sorted(rhs.elements.begin(), rhs.elements.end(),comparator));
  size_t selection_size = elements.size();
  elements.insert(elements.end(), rhs.elements.begin(), rhs.elements.end());
  std::inplace_merge(elements.begin(), elements.begin() + selection_size, elements.end(),comparator);
  auto end = std::unique(elements.begin(), elements.end());
  elements.erase(end, elements.end());
  assert(std::is_sorted(elements.begin(), elements.end(),comparator));
  return *this;
}
template<typename T>
Selection<T>& Selection<T>::operator-=(const Selection<T>& rhs) {
  LOG_DEBUG_FUNCTION();
  auto comparator = typename Selection<T>::element_type::PtrComparator{};
  assert(std::is_sorted(elements.begin(), elements.end(),comparator));
  assert(std::is_sorted(rhs.elements.begin(), rhs.elements.end(),comparator));
  auto it = elements.begin();
  auto it1 = elements.begin();
  auto it2 = rhs.elements.begin();
  while (it1 != elements.end() && it2 != rhs.elements.end()) {
    if (comparator(*it2,*it1)) { // it2 < it1
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
  while (it1 != elements.end()) {
    *it = *it1;
    ++it;
    ++it1;
  }
  elements.erase(it, elements.end());
  assert(std::is_sorted(elements.begin(), elements.end(), comparator));

  remove_redundant_observers();

  return *this;
}

template<typename T>
Selection<T>& Selection<T>::operator*=(const Selection<T>& rhs){
  LOG_DEBUG_FUNCTION();
  auto comparator = typename Selection<T>::element_type::PtrComparator{};
  assert(std::is_sorted(elements.begin(), elements.end(), comparator));
  assert(std::is_sorted(rhs.elements.begin(), rhs.elements.end(), comparator));

  auto end = std::set_intersection(elements.begin(),
      elements.end(),
      rhs.elements.begin(),
      rhs.elements.end(),
      elements.begin(),
      comparator);
  elements.erase(end, elements.end());
  assert(std::is_sorted(elements.begin(), elements.end(), comparator));
  remove_redundant_observers();
  return *this;
}

template<typename T>
void Selection<T>::remove_redundant_observers()
{
  std::set<container_type*> new_observers;
  for (auto el_ptr: elements){
    new_observers.insert(el_ptr->parent());
  }

  for (auto [container, state]: this->observers){
    if (!new_observers.count(container)) {
      this->remove_observer(*container);
      container->ObservableBy<Selection<T>>::remove_observer(*this);
    }
  }
}

template<typename T>
size_t Selection<T>::count(T& value) const {
  LOG_DEBUG_FUNCTION();
  assert(state==SelectionState::OK);
  return std::find_if(elements.begin(),elements.end(),
      [&value](const element_type& el){
    return el.value == value;
  }
  )==elements.end()?1:0;
}

template<typename T>
bool Selection<T>::empty() const {
  LOG_DEBUG_FUNCTION();
  return elements.empty();
}


template<typename T>
size_t Selection<T>::size() const {
  LOG_DEBUG_FUNCTION();
  return elements.size();
}

template<typename T>
void Selection<T>::clear() noexcept {
  LOG_DEBUG_FUNCTION();
  elements.clear();
  constexpr auto alive_only = ObservableBy<container_type>::ApplyTo::ALIVE_ONLY;
  this-> template notify_all<alive_only>(static_cast<typename SelectionTraits<T>::on_selection_delete_type>(&container_type::on_selection_delete),*this);
  this->remove_all_observers();
  state = SelectionState::OK;
}

template<typename T>
void Selection<T>::on_container_move(Selection<T>::container_type& from, Selection<T>::container_type& to) {
  LOG_DEBUG_FUNCTION();
  move_observer(from,to);
}
template<typename T>
void Selection<T>::on_container_elements_move(element_type* old_begin, element_type* old_end, ptrdiff_t shift) {
  LOG_DEBUG_FUNCTION();
  for (auto& ptr: elements){
    if (old_begin<=ptr && ptr<old_end){
      ptr+=shift;
    }
  }
}

template<typename T>
void Selection<T>::on_container_delete(Selection<T>::container_type& half_dead) {
  LOG_DEBUG_FUNCTION();
  this->mark_as_deleted(half_dead);
  state=SelectionState::HAS_DANGLING_REFERENCES;
}

template<typename T>
Selection<T>::Selection(Selection<T>::container_type& container) {
  LOG_DEBUG_FUNCTION();
  state = SelectionState::OK;
  this->add_observer(container);
  for (element_type& el: container.elements){
    this->elements.push_back(&el);
  }
  auto comparator = typename Selection<T>::element_type::PtrComparator{};
  std::sort(this->elements.begin(),this->elements.end(),comparator);
}

template<typename T>
Selection<T> operator+(const Selection<T>& lhs, const Selection<T>& rhs){
  Selection<T> result(lhs);
  result+=rhs;
  return result;
}

template<typename T>
Selection<T> operator-(const Selection<T>& lhs, const Selection<T>& rhs){
  Selection<T> result(lhs);
  result-=rhs;
  return result;
}

template<typename T>
Selection<T> operator*(const Selection<T>& lhs, const Selection<T>& rhs){
  Selection<T> result(lhs);
  result*=rhs;
  return result;
}


template<typename T>
Container<T>::Container(){
  LOG_DEBUG_FUNCTION();
}

template<typename T>
Container<T>::~Container(){
  LOG_DEBUG_FUNCTION();
  ObservableBy<Selection<T>>::notify_all(&Selection<T>::on_container_delete,*this);
  ObservableBy<Selection<const T>>::notify_all(&Selection<const T>::on_container_delete,*this);
}

template<typename T>
Container<T>::Container(Container<T>&& rhs) noexcept : ObservableBy<Selection<const T>>(std::move(rhs)), ObservableBy<Selection<T>>(std::move(rhs)){
  LOG_DEBUG_FUNCTION();
  notify_all(&Selection<T>::on_container_move,rhs,*this);
}

template<typename T>
Container<T>::Container(const Container<T>& rhs){
  LOG_DEBUG_FUNCTION();
}

template<typename T>
Container<T>& Container<T>::operator=(Container<T>&& rhs) noexcept {
  LOG_DEBUG_FUNCTION();
  elements = std::move(rhs.elements);
  deleted = rhs.deleted;
  rhs.deleted = 0;
  ObservableBy<Selection<T>>::operator=(std::move(rhs));
  ObservableBy<Selection<T>>::notify_all(&Selection<T>::on_container_move,rhs,*this);
  ObservableBy<Selection<const T>>::operator=(std::move(rhs));
  ObservableBy<Selection<const T>>::notify_all(&Selection<const T>::on_container_move,rhs,*this);
  return *this;
}

template<typename T>
Container<T>& Container<T>::operator=(const Container<T>& rhs) {
  LOG_DEBUG_FUNCTION();
  elements = rhs.elements;
  deleted = rhs.deleted;
  return *this;
}


template<typename T>
void Container<T>::insert(T&& value) {
  LOG_DEBUG_FUNCTION();
  auto old_begin = elements.data();
  elements.reserve(elements.size()+1);
  auto new_pos= elements.data();

  ptrdiff_t shift = new_pos-old_begin;
  if (shift!=0){
    ObservableBy<Selection<const T>>::notify_all(&Selection<const T>::on_container_elements_move,old_begin,old_begin+elements.size(),shift);
    ObservableBy<Selection<T>>::notify_all(&Selection<T>::on_container_elements_move,old_begin,old_begin+elements.size(),shift);
  }
  elements.emplace_back(std::move(value));
}

template<typename T>
template<typename ...Args>
void Container<T>::emplace(Args&& ... args) {
  LOG_DEBUG_FUNCTION();
  auto old_begin = elements.data();
  elements.reserve(elements.size()+1);
  auto new_pos= elements.data();

  ptrdiff_t shift = new_pos-old_begin;
  if (shift!=0){
    ObservableBy<Selection<const T>>::notify_all(&Selection<const T>::on_container_elements_move,old_begin,old_begin+elements.size(),shift);
    ObservableBy<Selection<T>>::notify_all(&Selection<T>::on_container_elements_move,old_begin,old_begin+elements.size(),shift);
  }
  elements.emplace_back(T(std::forward<Args>(args)...));
}



template<typename T>
Selection<T> Container<T>::all(){
  LOG_DEBUG_FUNCTION();
  Selection<T> s(*this);
  ObservableBy<Selection<T>>::add_observer(s);
  return std::move(s);
}

template<typename T>
Selection<const T> Container<T>::all() const{
  LOG_DEBUG_FUNCTION();
  Selection<const T> s(*this);
  ObservableBy<Selection<const T>>::add_observer(s);
  return std::move(s);
}

template<typename T>
void Container<T>::on_selection_move(Selection<T>& from, Selection<T>& to){
  LOG_DEBUG_FUNCTION();
  ObservableBy<Selection<T>>::move_observer(from,to);
}

template<typename T>
void Container<T>::on_selection_move(Selection<const T>& from, Selection<const T>& to) const{
  LOG_DEBUG_FUNCTION();
  ObservableBy<Selection<const T>>::move_observer(from,to);
}


template<typename T>
void Container<T>::on_selection_delete(Selection<T>& half_dead){
  LOG_DEBUG_FUNCTION();
  ObservableBy<Selection<T>>::remove_observer(half_dead);
}

template<typename T>
void Container<T>::on_selection_delete(Selection<const T>& half_dead) const{
  LOG_DEBUG_FUNCTION();
  ObservableBy<Selection<const T>>::remove_observer(half_dead);
}


template<typename T>
void Container<T>::on_selection_copy(Selection<T>& copy){
  LOG_DEBUG_FUNCTION();
  ObservableBy<Selection<T>>::add_observer(copy);
}

template<typename T>
void Container<T>::on_selection_copy(Selection<const T>& copy) const{
  LOG_DEBUG_FUNCTION();
  ObservableBy<Selection<const T>>::add_observer(copy);
}




}
