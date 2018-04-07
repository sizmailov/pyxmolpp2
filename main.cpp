#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <cassert>

#include "Logger.h"
#include "Observer.h"
#include "ContainerSelection.h"


using namespace selection;

struct int_with_parent{
  int_with_parent(int value,Container<int_with_parent>& container): value(value), ptr(&container){}
  int value;
  Container<int_with_parent>* ptr;
};
std::ostream& operator<<(std::ostream&out, const int_with_parent& value){
  out << value.value;
  return out;
}

template<>
Container<int_with_parent>* selection::ElementWithFlags<int_with_parent>::parent() noexcept {
  return value.ptr;
}

template<>
const Container<int_with_parent>* selection::ElementWithFlags<int_with_parent>::parent() const noexcept {
  return value.ptr;
}

template<>
bool ElementWithFlags<int_with_parent>::operator<(const ElementWithFlags<int_with_parent>& rhs) const noexcept {
  return this->value.value < rhs.value.value;
}

int main() {
  ::Common::Logger::init("log.log",::Common::Logger::LOG_LEVEL::VERBOSE);
  LOG_DEBUG_SCOPE("global_scope");

  Selection<int_with_parent> s;
  Container<int_with_parent> c2;
  c2.emplace(1,c2);
  c2.emplace(2,c2);
  c2.emplace(3,c2);
  {
    Container<int_with_parent> c;
    LOG_DEBUG_SCOPE("scope");
    c.emplace(1,c);
    c.emplace(2,c);
    c.emplace(3,c);
    s = c.all();
    assert(s.size()==3);
    c.emplace(4,c);
    s += c2.all();
    assert(s.size()==6);
    for (auto&x :s ){
      std::cout << x << std::endl;
    }
    s-=c.all();
  }


  for (auto&x :s ){ // ERROR comes here: use of dangling pointers in s;
    std::cout << x << std::endl;
  }


  std::cout << "Hello, World!" << std::endl;

  return 0;
}
