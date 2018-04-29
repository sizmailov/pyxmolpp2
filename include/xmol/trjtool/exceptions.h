#pragma once

#pragma once
#include "xmol/BaseException.h"

#include <type_traits>

namespace xmol{
namespace trjtool {



class TrjtoolException: public xmol::BaseException {
public:
  using BaseException::BaseException;
};


class unexpected_eof: public TrjtoolException{
public:
  using TrjtoolException::TrjtoolException;
};

class corrupted_file: public TrjtoolException{
public:
  using TrjtoolException::TrjtoolException;
};

}
}
