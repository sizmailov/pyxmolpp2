#pragma once

#include "xmol/polymer/Atom.h"

#include "PdbRecord_fwd.h"

#include <iostream>

namespace xmol {
namespace pdb {

class PdbWriter{
public:
  explicit PdbWriter(std::ostream& out) : m_old_locale(std::locale::global(std::locale::classic())), m_ostream(&out) {}
  ~PdbWriter() {std::locale::global(m_old_locale);}

  void write(const xmol::polymer::Frame& frame);
  void write(const xmol::polymer::Chain& chain);
  void write(const xmol::polymer::Residue& residue);
  void write(const xmol::polymer::Atom& atom);

  void write(const xmol::polymer::Frame& frame, const basic_PdbRecords& db);
  void write(const xmol::polymer::Chain& chain, const basic_PdbRecords& db);
  void write(const xmol::polymer::Residue& residue, const basic_PdbRecords& db);
  void write(const xmol::polymer::Atom& atom, const basic_PdbRecords& db);


  void write(const xmol::polymer::ConstChainSelection& chain, const basic_PdbRecords& db);
  void write(const xmol::polymer::ConstResidueSelection& residueSelection, const basic_PdbRecords& db);
  void write(const xmol::polymer::ConstAtomSelection& atomSelection, const basic_PdbRecords& db);

  void write(const xmol::polymer::ChainSelection& chain, const basic_PdbRecords& db);
  void write(const xmol::polymer::ResidueSelection& residueSelection, const basic_PdbRecords& db);
  void write(const xmol::polymer::AtomSelection& atomSelection, const basic_PdbRecords& db);


private:
  std::locale m_old_locale;
  std::ostream* m_ostream;
};

}
}