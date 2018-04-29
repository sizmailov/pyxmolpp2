#pragma once

#include <stdexcept>

namespace xmol {

class BaseException : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

}
