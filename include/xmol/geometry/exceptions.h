#pragma once

#include "xmol/BaseException.h"

namespace xmol{

namespace geometry {

class GeometryException : public BaseException{
public:
  using BaseException::BaseException;
};

class AlignmentError : public GeometryException {
public:
  using GeometryException::GeometryException;
};

class BadRotationMatrix : public GeometryException {
public:
  using GeometryException::GeometryException;
};



}

}
