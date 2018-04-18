#pragma once
#include "xmol/polymer/Atom.h"
#include <iostream>

namespace xmol::pdb {

class PDBReader {
public:
  PDBReader(std::istream& is) : is(&is) {}
  xmol::polymer::Frame read_frame();
  std::vector<xmol::polymer::Frame> read_frames();

private:
  std::istream* is;
};
}
