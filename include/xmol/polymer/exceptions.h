#pragma once

#include "xmol/BaseException.h"

namespace xmol{
namespace polymer{

class PolymerException : public xmol::BaseException{
public:
  using BaseException::BaseException;
};

class OutOfRangeResidue : public PolymerException{
public:
  using PolymerException::PolymerException;
};

class OutOfRangeChain: public PolymerException{
public:
  using PolymerException::PolymerException;
};

class OutOfRangeFrame: public PolymerException{
public:
  using PolymerException::PolymerException;
};

class IOError : public PolymerException{
public:
  using PolymerException::PolymerException;
};


}
}
