
#include "xmol/trjtool/DatFile.h"
#include <fstream>

using namespace xmol::trjtool;

DatFile::DatFile(const std::string& filename)
    : m_filename(filename),
      m_stream(std::make_unique<std::ifstream>(filename, std::ios::binary)),
      m_reader(std::make_unique<DATReader>(*m_stream)) {
  close();
}

void DatFile::close() { m_stream->close(); }

bool DatFile::match(
    const xmol::polymer::AtomSelection& atoms) const {
  return m_reader->match(atoms);
}
void DatFile::set_coordinates(
    xmol::polymer::frameIndex_t frameIndex,
    const std::vector<std::pair<int, xmol::polymer::Atom*>>& index_atoms) {
  //  return m_reader->set_frame(frameIndex,)
  throw std::runtime_error("Not implemented");
}
void DatFile::set_coordinates(
    xmol::polymer::frameIndex_t frameIndex,
    const xmol::polymer::AtomSelection& atoms) {
  if (!m_stream->is_open()) {
    m_stream->open(m_filename, std::ios::binary);
  }
  return m_reader->set_frame(frameIndex, atoms);
}
xmol::polymer::frameIndex_t DatFile::n_frames() const {
  return m_reader->n_frames();
}
xmol::polymer::atomIndex_t DatFile::n_atoms_per_frame() const {
  return m_reader->n_atoms_per_frame();
}
std::unique_ptr<xmol::trajectory::TrajectoryPortion> DatFile::get_copy() const
{
  return std::make_unique<DatFile>(*this);
}
