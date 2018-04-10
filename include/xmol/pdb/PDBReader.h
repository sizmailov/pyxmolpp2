#pragma once
#include <iostream>
#include "xmol/polymer/Atom.h"

namespace xmol::pdb{

class PDBReader{
public:
  PDBReader(std::istream& is): is(&is){}
  xmol::polymer::Frame read_frame();
  std::vector<xmol::polymer::Frame> read_frames();
private:
  std::istream* is;
};

}
