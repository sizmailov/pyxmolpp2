
#include "xmol/trjtool/DatTajectoryPortion.h"
#include <fstream>

using namespace xmol::trjtool;

DatTrajectoryPortion::DatTrajectoryPortion(const std::string& filename)
    : m_filename(filename),
      m_stream(std::make_unique<std::ifstream>(filename, std::ios::binary)),
      m_reader(std::make_unique<DATReader>(*m_stream)) {
  close();
}

void DatTrajectoryPortion::close() { m_stream->close(); }

bool DatTrajectoryPortion::match(
    const xmol::polymer::AtomSelection& atoms) const {
  return m_reader->match(atoms);
}
void DatTrajectoryPortion::set_coordinates(
    xmol::polymer::frameIndex_t frameIndex,
    const std::vector<std::pair<int, xmol::polymer::Atom*>>& index_atoms) {
  //  return m_reader->set_frame(frameIndex,)
  throw std::runtime_error("Not implemented");
}
void DatTrajectoryPortion::set_coordinates(
    xmol::polymer::frameIndex_t frameIndex,
    xmol::polymer::AtomSelection& atoms) {
  if (!m_stream->is_open()) {
    m_stream->open(m_filename, std::ios::binary);
  }
  return m_reader->set_frame(frameIndex, atoms);
}
xmol::polymer::frameIndex_t DatTrajectoryPortion::n_frames() const {
  return m_reader->n_frames();
}
xmol::polymer::atomIndex_t DatTrajectoryPortion::n_atoms_per_frame() const {
  return m_reader->n_atoms_per_frame();
}
