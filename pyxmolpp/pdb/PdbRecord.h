#pragma once

#include "init.h"

#include "xmol/pdb/PdbRecord.h"

namespace pyxmolpp {
namespace pdb {

class Pybasic_PdbRecords : public xmol::pdb::basic_PdbRecords {
public:
  using basic_PdbRecords::basic_PdbRecords;
  virtual const xmol::pdb::PdbRecordType&
  get_record(const xmol::pdb::RecordName& string) const override {
    PYBIND11_OVERLOAD_PURE(
        const xmol::pdb::PdbRecordType&, /* Return type */
        basic_PdbRecords,                /* Parent class */
        get_record,                      /* Name of function in C++ (must
                                                                      match Python name) */
        string                           /* Argument(s) */
        );
  }
};

}
}
