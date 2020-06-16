#pragma once

#include "xmol/utils/ShortAsciiString.h"

namespace xmol::io::pdb {

namespace detail {
struct RecordTypeNameTag {};
struct RecordFieldNameTag {};
} // namespace detail

using RecordName = utils::ShortAsciiString<6, false, detail::RecordTypeNameTag>;
using FieldName = utils::ShortAsciiString<8, true, detail::RecordFieldNameTag>;

class basic_PdbRecords;
} // namespace xmol::io::pdb