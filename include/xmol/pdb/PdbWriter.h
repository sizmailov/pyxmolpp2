#pragma once

#include "xmol/polymer/Atom.h"

#include "PdbRecord_fwd.h"

#include <iostream>

namespace xmol {
namespace pdb {

class PdbWriter{
public:
  explicit PdbWriter(std::ostream& out) : m_ostream(&out) {}

  void write(const xmol::polymer::Frame& frame);
  void write(const xmol::polymer::Chain& chain);
  void write(const xmol::polymer::Residue& residue);
  void write(const xmol::polymer::Atom& atom);

  void write(const xmol::polymer::Frame& frame, const basic_PdbRecords& db);
  void write(const xmol::polymer::Chain& chain, const basic_PdbRecords& db);
  void write(const xmol::polymer::Residue& residue, const basic_PdbRecords& db);
  void write(const xmol::polymer::Atom& atom, const basic_PdbRecords& db);


private:
  std::ostream* m_ostream;
};

}
}