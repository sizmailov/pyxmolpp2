#include "xmol/pdb/PdbLine.h"
#include "xmol/pdb/PdbRecord.h"
#include "xmol/pdb/exceptions.h"
#include "xmol/utils/parsing.h"
#include "xmol/utils/string.h"
#include <algorithm>
#include <iostream>

using namespace xmol::pdb;
using xmol::utils::string::rtrim;
using namespace xmol::utils::parsing;

PdbLine::PdbLine() : line(nullptr), pdbRecordType(nullptr) {}

PdbLine::PdbLine(const std::string& pdb_line, const basic_PdbRecords& db)
    : recordName(rtrim(pdb_line.substr(0, 6))), line(&pdb_line),
      pdbRecordType(&db.get_record(this->recordName)) {}
double PdbLine::getDouble(const FieldName& fieldName, size_t idx) const {
  auto& v = pdbRecordType->getFieldColons(fieldName);
  auto first = v[idx * 2] - 1;
  auto last = v[idx * 2 + 1] - 1;
  auto n = last + 1 - first;
  bool success; double value;
  std::tie(success,value) = parse_fixed_precision_rt(*line, first, n);
  if (!success) {
    throw PdbFieldReadError("PDB line: can't read double",first,last);
  }
  return value;
}

int PdbLine::getInt(const FieldName& fieldName, size_t idx) const {
  auto& v = pdbRecordType->getFieldColons(fieldName);
  auto first = v[idx * 2] - 1;
  auto last = v[idx * 2 + 1] - 1;
  auto n = last + 1 - first;
  bool success; int value;
  std::tie(success,value) = parse_int<SpaceStrip::LEFT_AND_RIGHT>(*line, first, n);
  if (!success) {
    throw PdbFieldReadError("PDB line: can't read int",first,last);
  }
  return value;
}

std::string PdbLine::getString(const FieldName& fieldName,
                               size_t idx) const {
  auto& v = pdbRecordType->getFieldColons(fieldName);
  if (line->length() < v[idx * 2 + 1]) {
    throw PdbFieldReadError("PDB line is too short",v[idx * 2] - 1, v[idx * 2 + 1]-1);
  }
  return line->substr(v[idx * 2] - 1, v[idx * 2 + 1] - v[idx * 2] + 1);
}

PdbLine::StrPtr PdbLine::getStrPtr(const FieldName& fieldName,
                                   size_t idx) const {
  auto& v = pdbRecordType->getFieldColons(fieldName);
  if (line->length() < v[idx * 2 + 1]) {
    throw PdbFieldReadError("PDB line is too short",v[idx * 2] - 1, v[idx * 2 + 1]-1);
  }
  return StrPtr{line->data() + v[idx * 2] - 1, v[idx * 2 + 1] - v[idx * 2] + 1};
}

const std::string& PdbLine::getLine() const { return *line; }
RecordName PdbLine::getRecordName() const { return recordName; }
char PdbLine::getChar(const FieldName& fieldName, size_t idx) const {
  return getStrPtr(fieldName, idx).data[0];
}
