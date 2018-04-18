#include "xmol/pdb/PDBRecord.h"

#include <fstream>

using namespace xmol::pdb;

const std::vector<int>&
PDBRecordType::getFieldColons(const RecordFieldName& fieldName) const {
  auto col = fieldColons.find(fieldName);
  if (col != fieldColons.end()) {
    return col->second;
  }
  throw std::out_of_range("Error: "
                          "xmol::pdb::entry::PDBRecordType::getFieldColons(..."
                          "): no field name `" +
                          fieldName.str() + "`");
}

void PDBRecordType::set_field(const xmol::pdb::RecordFieldName& fieldName,
                              const std::vector<int>& colons) {
  fieldColons[fieldName] = colons;
}

StandardPdbRecords::StandardPdbRecords() {
  this->recordTypes = detail::get_bundled_records();
}

const PDBRecordType& AlteredPdbRecords::get_record(
    const xmol::pdb::RecordTypeName& recordTypeName) const {
  auto it = recordTypes.find(recordTypeName);
  if (it != recordTypes.end()) {
    return it->second;
  }
  return basic->get_record(recordTypeName);
}

void AlteredPdbRecords::alter_record(xmol::pdb::RecordTypeName recordTypeName,
                                     xmol::pdb::RecordFieldName fieldName,
                                     std::vector<int> colons) {
  auto it = recordTypes.find(recordTypeName);
  if (it != recordTypes.end()) { // alter owned record
    it->second.set_field(fieldName, colons);
  } else {
    try { // copy record from backup and alter field
      auto record = recordTypes.emplace(recordTypeName,
                                        basic->get_record(recordTypeName));
      record.first->second.set_field(fieldName, colons);
    } catch (std::out_of_range&) { // create new record with single field
      recordTypes.emplace(recordTypeName, PDBRecordType({{fieldName, colons}}));
    }
  }
}

const PDBRecordType&
StandardPdbRecords::get_record(const RecordTypeName& recordTypeName) const {
  auto col = recordTypes.find(recordTypeName);
  if (col != recordTypes.end()) {
    return col->second;
  }
  throw std::out_of_range("Error: "
                          "xmol::pdb::entry::PDBDefaultRecordTypesBase::get_"
                          "record(...): no record `" +
                          recordTypeName.str() + "`");
}

const basic_PdbRecords& StandardPdbRecords::instance() {
  const static StandardPdbRecords singleton;
  return singleton;
}
