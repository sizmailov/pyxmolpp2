#pragma once

#include <string>
#include "PDBRecord.h"

namespace xmol{
namespace pdb {


class PDBLine {
  struct StrPtr{
    const char* data;
    const int n;
  };
public:
  PDBLine();
  explicit PDBLine(const std::string& pdb_line, const PDBRecordTypesBase& db);
  PDBLine(PDBLine&& other) = default;
  PDBLine(const PDBLine& other) = default;
  PDBLine& operator=(const PDBLine& other) = default;
  PDBLine& operator=(PDBLine&& other) = default;

  double getDouble(const RecordFieldName& fieldName, size_t idx = 0) const;
  int getInt(const RecordFieldName& fieldName, size_t idx = 0) const;
  std::string getString(const RecordFieldName& fieldName, size_t idx = 0) const;
  char getChar(const RecordFieldName& fieldName, size_t idx = 0) const;
  const std::string& getLine() const;
  RecordTypeName getRecordName() const;

private:
  StrPtr getStrPtr(const RecordFieldName& fieldName, size_t idx = 0) const;
  RecordTypeName recordName;
  const std::string* line;
  const PDBRecordType* pdbRecordType;
};

} //::xmol::pdb
} //::xmol
