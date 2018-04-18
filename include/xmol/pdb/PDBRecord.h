#pragma once

#include "xmol/utils/ShortAsciiString.h"
#include <map>
#include <vector>

namespace xmol {
namespace pdb {

namespace detail {
struct RecordTypeNameTag {};
struct RecordFieldNameTag {};
}

using RecordTypeName = utils::ShortAsciiString<6, detail::RecordTypeNameTag>;
using RecordFieldName = utils::ShortAsciiString<8, detail::RecordFieldNameTag>;

class PDBRecordType {
public:
  PDBRecordType() = default;
  explicit PDBRecordType(
      std::map<RecordFieldName, std::vector<int>>&& field_colons)
      : fieldColons(std::move(field_colons)){};
  const std::vector<int>&
  getFieldColons(const RecordFieldName& fieldName) const;
  void set_field(const RecordFieldName& fieldName, const std::vector<int>& colons);
private:
  std::map<RecordFieldName, std::vector<int>> fieldColons;
};

class basic_PdbRecords {
public:
  virtual const PDBRecordType&
  get_record(const RecordTypeName& string) const = 0;
};

class StandardPdbRecords : public basic_PdbRecords {
public:
  virtual const PDBRecordType&
  get_record(const RecordTypeName& recordTypeName) const override;
  static const basic_PdbRecords& instance();

private:
  StandardPdbRecords();
  std::map<RecordTypeName, PDBRecordType> recordTypes;
};

class AlteredPdbRecords : public basic_PdbRecords {
public:
  explicit AlteredPdbRecords(const basic_PdbRecords& basic):basic(&basic){};

  virtual const PDBRecordType&
  get_record(const RecordTypeName& recordTypeName) const override;

  void alter_record(RecordTypeName, RecordFieldName, std::vector<int> colons);

private:
  const basic_PdbRecords* basic;
  std::map<RecordTypeName, PDBRecordType> recordTypes;
};


namespace detail {
std::map<RecordTypeName, PDBRecordType> get_bundled_records();
};

} //::xmol::pdb
} //::xmol
