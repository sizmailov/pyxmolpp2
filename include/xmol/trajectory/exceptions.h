#pragma once

#pragma once
#include "xmol/BaseException.h"

#include <type_traits>

namespace xmol{
namespace trajectory {



class TrajectoryException: public xmol::BaseException {
public:
  using BaseException::BaseException;
};

}
}
