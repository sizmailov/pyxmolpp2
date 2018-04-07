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
  int_with_parent(int value): value(value){}
  int value;
  Container<int_with_parent>* ptr;
};
std::ostream& operator<<(std::ostream&out, const int_with_parent& value){
  out << value.value;
  return out;
}

template<>
Container<int_with_parent>* selection::ElementWithFlags<int_with_parent>::parent() noexcept{
  return value.ptr;
}

template<>
const Container<int_with_parent>* selection::ElementWithFlags<int_with_parent>::parent() const noexcept {
  return value.ptr;
}

int main() {
  ::Common::Logger::init("log.log",::Common::Logger::LOG_LEVEL::VERBOSE);
  LOG_DEBUG_SCOPE("global_scope");

  Selection<int_with_parent> s;
  {
    Container<int_with_parent> c;
    LOG_DEBUG_SCOPE("scope");
    c.insert(1);
    c.insert(2);
    c.insert(3);
    s = c.all();
    assert(s.size()==3);
    c.insert(4);
    s += c.all();
    assert(s.size()==4);
    for (auto&x :s ){
      std::cout << x << std::endl;
    }
    s.clear();
  }


  for (auto&x :s ){ // ERROR comes here: use of dangling pointers in s;
    std::cout << x << std::endl;
  }


  std::cout << "Hello, World!" << std::endl;

  return 0;
}
