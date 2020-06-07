#pragma once
#include "xmol/v1/Frame.h"
#include <vector>

namespace xmol::v1::io {

class PdbReadError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

/// PDB file
class PdbFile {
public:
  /// PDB file dialect
  enum class Dialect {
    STANDARD_V3, /// PDB v3.3 standard (http://www.wwpdb.org/documentation/file-format-content/format33/v3.3.html)
    AMBER_99     /// compatibility with AMBER tools
  };

  PdbFile(std::string filename);
  PdbFile& read(Dialect dialect = Dialect::STANDARD_V3);
  const std::vector<Frame>& frames() const { return m_frames; }

private:
  std::string m_filename;
  std::vector<Frame> m_frames;
};

} // namespace xmol::v1::io