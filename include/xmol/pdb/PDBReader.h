#pragma once
#include "xmol/polymer/Atom.h"
#include <iostream>

namespace xmol::pdb {

class basic_PdbRecords;

class PDBReader {
public:
  explicit PDBReader(std::istream& is) : is(&is) {}
  xmol::polymer::Frame read_frame();
  xmol::polymer::Frame read_frame(const basic_PdbRecords& db);
  std::vector<xmol::polymer::Frame> read_frames();

private:
  std::istream* is;
};
}
