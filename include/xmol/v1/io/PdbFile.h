#pragma once
#include "xmol/v1/Frame.h"
#include <vector>

namespace xmol::v1::io {

class PdbReadError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class PdbFile {
  PdbFile(std::string filename);
  PdbFile& read();
  const std::vector<Frame>& frames() const { return m_frames; }

private:
  std::string m_filename;
  std::vector<Frame> m_frames;
};

} // namespace xmol::v1::io