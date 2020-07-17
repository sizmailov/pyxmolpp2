#pragma once

#include "xmol/fwd.h"
#include "PdbRecord_fwd.h"

#include <iostream>

namespace xmol::io::pdb {

class PdbWriter {
public:
  explicit PdbWriter(std::ostream& out) : m_old_locale(std::locale::global(std::locale::classic())), m_ostream(&out) {}
  ~PdbWriter() { std::locale::global(m_old_locale); }

  void write(xmol::Frame& frame);
  void write(const xmol::proxy::MoleculeRef& chain);
  void write(const xmol::proxy::ResidueRef& residue);
  void write(const xmol::proxy::AtomRef& atom);

  void write(xmol::Frame& frame, const basic_PdbRecords& db);
  void write(const xmol::proxy::MoleculeRef& chain, const basic_PdbRecords& db);
  void write(const xmol::proxy::ResidueRef& residue, const basic_PdbRecords& db);
  void write(const xmol::proxy::AtomRef& atom, const basic_PdbRecords& db);

  void write(const xmol::proxy::MoleculeSelection& chain, const basic_PdbRecords& db);
  void write(const xmol::proxy::ResidueSelection& residueSelection, const basic_PdbRecords& db);
  void write(const xmol::proxy::AtomSelection& atomSelection, const basic_PdbRecords& db);

  void write(const xmol::proxy::MoleculeSpan& chain, const basic_PdbRecords& db);
  void write(const xmol::proxy::ResidueSpan& residueSelection, const basic_PdbRecords& db);
  void write(const xmol::proxy::AtomSpan& atomSelection, const basic_PdbRecords& db);

private:
  std::locale m_old_locale;
  std::ostream* m_ostream;
};

}