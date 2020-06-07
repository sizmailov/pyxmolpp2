#include "xmol/v1/io/PdbFile.h"
#include "xmol/pdb/PdbFile.h"
#include "xmol/pdb/PdbReader.h"
#include "xmol/pdb/PdbRecord.h"

using namespace xmol::v1::io;

PdbFile::PdbFile(std::string filename) : m_filename(std::move(filename)) {}

PdbFile& PdbFile::read(Dialect dialect) {
  xmol::pdb::AlteredPdbRecords alteredPdbRecords(xmol::pdb::StandardPdbRecords::instance());
  switch (dialect) {
  case (Dialect::AMBER_99):
    alteredPdbRecords.alter_record(xmol::pdb::RecordName("ATOM"), xmol::pdb::FieldName("serial"), {7, 12});
    break;
  case (Dialect::STANDARD_V3):
    break;
  }

  auto v0_frames = xmol::pdb::PdbFile(m_filename, alteredPdbRecords).get_frames(); // todo: construct v1 frames inplace
  for (auto& frame0 : v0_frames) {
    Frame& frame1 = m_frames.emplace_back();
    for (auto& chain : frame0) {
      auto mol = frame1.add_molecule(MoleculeName{chain.name().str()});
      for (auto& residue0 : chain) {
        auto res = mol.add_residue(residue0.name(), residue0.id());
        for (auto& atom0 : residue0) {
          auto atom = res.add_atom(atom0.name(), atom0.id());
          atom.r(atom0.r());
        }
      }
    }
  }
  return *this;
}
