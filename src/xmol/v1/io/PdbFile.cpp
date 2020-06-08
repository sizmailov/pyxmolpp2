#include "xmol/pdb/PdbFile.h"
#include "xmol/pdb/PdbReader.h"
#include "xmol/pdb/PdbRecord.h"
#include "xmol/v1/io/PdbInputFile.h"

using namespace xmol::v1::io;

PdbInputFile::PdbInputFile(std::string filename, Dialect dialect, bool read_now)
    : m_filename(std::move(filename)), m_dialect(dialect) {
  if (read_now) {
    read();
  }
}

PdbInputFile& PdbInputFile::read() {
  xmol::pdb::AlteredPdbRecords alteredPdbRecords(xmol::pdb::StandardPdbRecords::instance());
  switch (m_dialect) {
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
      auto mol = frame1.add_molecule().name(MoleculeName{chain.name().str()});
      for (auto& residue0 : chain) {
        auto res = mol.add_residue().id(residue0.id()).name(residue0.name());
        for (auto& atom0 : residue0) {
          auto atom = res.add_atom().name(atom0.name()).id(atom0.id());
          atom.r(atom0.r());
        }
      }
    }
  }
  m_n_frames = m_frames.size();
  if (!m_frames.empty()) {
    m_n_atoms = m_frames[0].n_atoms();
  }
  return *this;
}

size_t PdbInputFile::n_frames() const { return m_n_frames; }
size_t PdbInputFile::n_atoms() const { return m_n_atoms; }
void PdbInputFile::read_coordinates(size_t index, proxy::CoordSpan& coordinates) {
  assert(!m_frames.empty());
  assert(m_current_frame == index);

  Frame& frame = m_frames[index];
  if (coordinates.size() != frame.n_atoms()) {
    throw PdbReadError("Wrong of atoms in " + std::to_string(index) + " frame in `" + m_filename + "`. Expected " +
                       std::to_string(coordinates.size()));
  }
  for (size_t i = 0; i < coordinates.size(); ++i) {
    coordinates[i] = frame.coords()[i];
  }
}
void PdbInputFile::advance(size_t shift) {
  m_current_frame += shift;
  if (m_current_frame >= n_frames()) {
    m_frames.clear();
    m_current_frame = 0;
    return;
  }
  if (m_frames.empty()) {
    read();
  }
}
