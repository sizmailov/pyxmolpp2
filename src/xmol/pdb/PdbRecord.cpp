#include "xmol/pdb/PdbRecord.h"
#include "xmol/pdb/exceptions.h"

#include <fstream>

using namespace xmol::pdb;

const std::vector<int>&
PdbRecordType::getFieldColons(const FieldName& fieldName) const {
  auto col = fieldColons.find(fieldName);
  if (col != fieldColons.end()) {
    return col->second;
  }
  throw PdbUknownRecordField("Unknown field" +fieldName.str() + "`");
}

void PdbRecordType::set_field(const xmol::pdb::FieldName& fieldName,
                              const std::vector<int>& colons) {
  fieldColons[fieldName] = colons;
}

StandardPdbRecords::StandardPdbRecords() {
  this->recordTypes = detail::get_bundled_records();
}

const PdbRecordType& AlteredPdbRecords::get_record(
    const xmol::pdb::RecordName& recordTypeName) const {
  auto it = recordTypes.find(recordTypeName);
  if (it != recordTypes.end()) {
    return it->second;
  }
  return basic->get_record(recordTypeName);
}

void AlteredPdbRecords::alter_record(xmol::pdb::RecordName recordTypeName,
                                     xmol::pdb::FieldName fieldName,
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
      recordTypes.emplace(recordTypeName, PdbRecordType({{fieldName, colons}}));
    }
  }
}

const PdbRecordType&
StandardPdbRecords::get_record(const RecordName& recordTypeName) const {
  auto col = recordTypes.find(recordTypeName);
  if (col != recordTypes.end()) {
    return col->second;
  }
  throw PdbUknownRecord("Unknown record `" + recordTypeName.str() + "`");
}

const basic_PdbRecords& StandardPdbRecords::instance() {
  const static StandardPdbRecords singleton;
  return singleton;
}
