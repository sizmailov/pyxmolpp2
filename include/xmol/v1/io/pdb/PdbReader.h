#pragma once
#include "xmol/v1/Frame.h"
#include <iostream>

namespace xmol::v1::io::pdb {

class basic_PdbRecords;

class PdbReader {
public:
  explicit PdbReader(std::istream& is) : is(&is) {}

  xmol::v1::Frame read_frame();
  xmol::v1::Frame read_frame(const basic_PdbRecords& db);
  std::vector<xmol::v1::Frame> read_frames();
  std::vector<xmol::v1::Frame> read_frames(const basic_PdbRecords& db);

private:
  std::istream* is;
};
}