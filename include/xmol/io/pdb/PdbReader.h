#pragma once
#include "xmol/Frame.h"
#include <iostream>

namespace xmol::io::pdb {

class basic_PdbRecords;

class PdbReader {
public:
  explicit PdbReader(std::istream& is) : is(&is) {}

  xmol::Frame read_frame();
  xmol::Frame read_frame(const basic_PdbRecords& db);
  std::vector<xmol::Frame> read_frames();
  std::vector<xmol::Frame> read_frames(const basic_PdbRecords& db);

private:
  std::istream* is;
};
}