#include "xmol/pdb/PDBLine.h"
#include "xmol/pdb/PDBRecord.h"
#include "xmol/utils/string.h"
#include <algorithm>
#include <iostream>

using namespace xmol::pdb;
using xmol::utils::string::rtrim;

PDBLine::PDBLine(): line(nullptr),pdbRecordType(nullptr) { }
PDBLine::PDBLine(const std::string& pdb_line, const PDBRecordTypesBase& db) : line(&pdb_line),
                                                                         pdbRecordType(&db.get_record(RecordTypeName(
                                                                             rtrim(line->substr(0,std::min(6ul,line->size())))
                                                                         ))){
}
double PDBLine::getDouble(const RecordFieldName& fieldName, size_t idx) const {
  return std::atof(getString(fieldName, idx).c_str());
}

int PDBLine::getInt(const RecordFieldName& fieldName, size_t idx) const {
  return std::atoi(getString(fieldName, idx).c_str());
}
std::string PDBLine::getString(const RecordFieldName& fieldName, size_t idx) const {
  auto &v = pdbRecordType->getFieldColons(fieldName);
  if (line->length() < v[idx*2+1]){
    throw std::runtime_error("PDB line is too short");
  }
  return line->substr(v[idx*2]-1,v[idx*2+1]-v[idx*2]+1);
}
PDBLine::StrPtr PDBLine::getStrPtr(const RecordFieldName& fieldName, size_t idx) const {
  auto &v = pdbRecordType->getFieldColons(fieldName);
  if (line->length() < v[idx*2+1]){
    throw std::runtime_error("PDB line is too short");
  }
  return StrPtr{line->data()+v[idx*2]-1,v[idx*2+1]-v[idx*2]+1};
}
const std::string& PDBLine::getLine() const {
  return *line;
}
RecordTypeName PDBLine::getRecordName() const {
  auto first_space = std::min(6ul,line->find(" "));

  auto record_name = line->substr(0,first_space);
  return RecordTypeName(record_name);
}
char PDBLine::getChar(const RecordFieldName& fieldName, size_t idx) const {
  return getStrPtr(fieldName, idx).data[0];
}
