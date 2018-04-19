#pragma once

#include "PdbRecord.h"
#include <string>

namespace xmol {
namespace pdb {

class PdbLine {
  struct StrPtr {
    const char* data;
    const int n;
  };

public:
  PdbLine();
  explicit PdbLine(const std::string& pdb_line, const basic_PdbRecords& db);
  PdbLine(PdbLine&& other) = default;
  PdbLine(const PdbLine& other) = default;
  PdbLine& operator=(const PdbLine& other) = default;
  PdbLine& operator=(PdbLine&& other) = default;

  double getDouble(const FieldName& fieldName, size_t idx = 0) const;
  int getInt(const FieldName& fieldName, size_t idx = 0) const;
  std::string getString(const FieldName& fieldName, size_t idx = 0) const;
  char getChar(const FieldName& fieldName, size_t idx = 0) const;
  const std::string& getLine() const;
  RecordName getRecordName() const;

private:
  StrPtr getStrPtr(const FieldName& fieldName, size_t idx = 0) const;
  RecordName recordName;
  const std::string* line;
  const PdbRecordType* pdbRecordType;
};

} //::xmol::pdb
} //::xmol
