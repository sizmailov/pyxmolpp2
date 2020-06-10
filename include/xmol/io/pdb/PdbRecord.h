#pragma once

#include "PdbRecord_fwd.h"

#include <map>
#include <vector>

namespace xmol::io::pdb {

class PdbRecordType {
public:
  PdbRecordType() = default;
  explicit PdbRecordType(std::map<FieldName, std::vector<int>>&& field_colons) : fieldColons(std::move(field_colons)){};
  const std::vector<int>& getFieldColons(const FieldName& fieldName) const;
  void set_field(const FieldName& fieldName, const std::vector<int>& colons);

private:
  std::map<FieldName, std::vector<int>> fieldColons;
};

class basic_PdbRecords {
public:
  virtual const PdbRecordType& get_record(const RecordName& string) const = 0;
  virtual ~basic_PdbRecords() = default;
};

class StandardPdbRecords : public basic_PdbRecords {
public:
  virtual const PdbRecordType&
  get_record(const RecordName& recordTypeName) const override;
  static const basic_PdbRecords& instance();

private:
  StandardPdbRecords();
  std::map<RecordName, PdbRecordType> recordTypes;
};

class AlteredPdbRecords : public basic_PdbRecords {
public:
  explicit AlteredPdbRecords(const basic_PdbRecords& basic) : basic(&basic){};
  ~AlteredPdbRecords() override = default;
  virtual const PdbRecordType&
  get_record(const RecordName& recordTypeName) const override;

  void alter_record(RecordName, FieldName, std::vector<int> colons);

private:
  const basic_PdbRecords* basic;
  std::map<RecordName, PdbRecordType> recordTypes;
};

namespace detail {
std::map<RecordName, PdbRecordType> get_bundled_records();
};

} // namespace xmol::io::pdb
