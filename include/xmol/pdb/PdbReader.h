#pragma once
#include "xmol/polymer/Atom.h"
#include <iostream>

namespace xmol {
namespace pdb {

class basic_PdbRecords;

class PdbReader {
public:
  explicit PdbReader(std::istream& is) : is(&is) {}

  xmol::polymer::Frame read_frame();
  xmol::polymer::Frame read_frame(const basic_PdbRecords& db);
  std::vector<xmol::polymer::Frame> read_frames();
  std::vector<xmol::polymer::Frame> read_frames(const basic_PdbRecords& db);

private:
  std::istream* is;
};
}
}