#include "xmol/pdb/PDBLine.h"
#include "xmol/pdb/PDBRecord.h"
#include "xmol/utils/parsing.h"
#include "xmol/utils/string.h"
#include <algorithm>
#include <iostream>

using namespace xmol::pdb;
using xmol::utils::string::rtrim;
using namespace xmol::utils::parsing;

PDBLine::PDBLine() : line(nullptr), pdbRecordType(nullptr) {}

PDBLine::PDBLine(const std::string& pdb_line, const basic_PdbRecords& db)
    : recordName(rtrim(pdb_line.substr(0, 6))), line(&pdb_line),
      pdbRecordType(&db.get_record(this->recordName)) {}
double PDBLine::getDouble(const RecordFieldName& fieldName, size_t idx) const {
  auto& v = pdbRecordType->getFieldColons(fieldName);
  auto first = v[idx * 2] - 1;
  auto last = v[idx * 2 + 1] - 1;
  auto n = last + 1 - first;
  auto[success, value] = parse_fixed_precision_rt(*line, first, n);
  if (!success) {
    throw std::runtime_error("PDB line: can't read double");
  }
  return value;
}

int PDBLine::getInt(const RecordFieldName& fieldName, size_t idx) const {
  auto& v = pdbRecordType->getFieldColons(fieldName);
  auto first = v[idx * 2] - 1;
  auto last = v[idx * 2 + 1] - 1;
  auto n = last + 1 - first;
  auto[success, value] = parse_int<SpaceStrip::LEFT_AND_RIGHT>(*line, first, n);
  if (!success) {
    throw std::runtime_error("PDB line: can't read int");
  }
  return value;
}

std::string PDBLine::getString(const RecordFieldName& fieldName,
                               size_t idx) const {
  auto& v = pdbRecordType->getFieldColons(fieldName);
  if (line->length() < v[idx * 2 + 1]) {
    throw std::runtime_error("PDB line is too short");
  }
  return line->substr(v[idx * 2] - 1, v[idx * 2 + 1] - v[idx * 2] + 1);
}

PDBLine::StrPtr PDBLine::getStrPtr(const RecordFieldName& fieldName,
                                   size_t idx) const {
  auto& v = pdbRecordType->getFieldColons(fieldName);
  if (line->length() < v[idx * 2 + 1]) {
    throw std::runtime_error("PDB line is too short");
  }
  return StrPtr{line->data() + v[idx * 2] - 1, v[idx * 2 + 1] - v[idx * 2] + 1};
}

const std::string& PDBLine::getLine() const { return *line; }
RecordTypeName PDBLine::getRecordName() const { return recordName; }
char PDBLine::getChar(const RecordFieldName& fieldName, size_t idx) const {
  return getStrPtr(fieldName, idx).data[0];
}
