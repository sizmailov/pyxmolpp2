#include "xmol/pdb/PDBRecord.h"

#include <fstream>

using namespace xmol::pdb;

const std::vector<int>&
PDBRecordType::getFieldColons(const RecordFieldName& fieldName) const {
  auto col = fieldColons.find(fieldName);
  if (col != fieldColons.end()) {
    return col->second;
  }
  throw std::runtime_error("Error: "
                           "xmol::pdb::entry::PDBRecordType::getFieldColons(..."
                           "): no field name `" +
                           fieldName.str() + "`");
}

DefaultPDBRecordTypesBase::DefaultPDBRecordTypesBase() {
  this->recordTypes = detail::get_bundled_records();
}

const PDBRecordType& DefaultPDBRecordTypesBase::get_record(
    const RecordTypeName& recordTypeName) const {
  auto col = recordTypes.find(recordTypeName);
  if (col != recordTypes.end()) {
    return col->second;
  }
  throw std::runtime_error("Error: "
                           "xmol::pdb::entry::PDBDefaultRecordTypesBase::get_"
                           "record(...): no record `" +
                           recordTypeName.str() + "`");
}

const PDBRecordTypesBase& DefaultPDBRecordTypesBase::instance() {
  const static DefaultPDBRecordTypesBase singleton;
  return singleton;
}
