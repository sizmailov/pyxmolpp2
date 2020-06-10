#pragma once

#include "xmol/v1/fwd.h"
#include "PdbRecord_fwd.h"

#include <iostream>

namespace xmol::v1::io::pdb {

class PdbWriter {
public:
  explicit PdbWriter(std::ostream& out) : m_old_locale(std::locale::global(std::locale::classic())), m_ostream(&out) {}
  ~PdbWriter() { std::locale::global(m_old_locale); }

  void write(xmol::v1::Frame& frame);
  void write(xmol::v1::proxy::MoleculeRef& chain);
  void write(xmol::v1::proxy::ResidueRef& residue);
  void write(xmol::v1::proxy::AtomRef& atom);

  void write(xmol::v1::Frame& frame, const basic_PdbRecords& db);
  void write(xmol::v1::proxy::MoleculeRef& chain, const basic_PdbRecords& db);
  void write(xmol::v1::proxy::ResidueRef& residue, const basic_PdbRecords& db);
  void write(xmol::v1::proxy::AtomRef& atom, const basic_PdbRecords& db);

  void write(xmol::v1::proxy::MoleculeSelection& chain, const basic_PdbRecords& db);
  void write(xmol::v1::proxy::ResidueSelection& residueSelection, const basic_PdbRecords& db);
  void write(xmol::v1::proxy::AtomSelection& atomSelection, const basic_PdbRecords& db);

private:
  std::locale m_old_locale;
  std::ostream* m_ostream;
};

}