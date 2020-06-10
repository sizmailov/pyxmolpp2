#pragma once
#include <stdexcept>

namespace xmol::io::pdb {

class PdbException : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class PdbUknownRecordField : public PdbException {
public:
  using PdbException::PdbException;
};

class PdbUknownRecord: public PdbException {
public:
  using PdbException::PdbException;
};

class PdbFieldReadError : public PdbException {
public:
  PdbFieldReadError(const std::string& what, int colon_l, int colon_r)
      : PdbException(what), colon_l(colon_l), colon_r(colon_r) {}
  const int colon_l, colon_r;
};

}
