#include "xmol/pdb/PdbFile.h"
#include "xmol/pdb/PdbReader.h"

using namespace xmol::pdb;

PdbFile::PdbFile(const std::string& filename)
    : m_filename(filename),
      m_stream(std::make_unique<std::ifstream>(filename, std::ios::binary)),
      m_reader(std::make_unique<PdbReader>(*m_stream)) {
  close();
}

void PdbFile::set_coordinates(xmol::polymer::frameIndex_t frameIndex,
                              xmol::polymer::AtomSelection& atoms) {}

void PdbFile::set_coordinates(
    xmol::polymer::frameIndex_t frameIndex,
    const std::vector<std::pair<int, xmol::polymer::Atom*>>& index_atoms) {}

xmol::polymer::frameIndex_t PdbFile::n_frames() const {
  throw std::runtime_error("not implemented");
}

xmol::polymer::frameIndex_t PdbFile::n_atoms_per_frame() const {
  throw std::runtime_error("not implemented");
}

bool PdbFile::match(const xmol::polymer::AtomSelection& atoms) const {
  throw std::runtime_error("not implemented");
}

void PdbFile::close() {
  m_stream->close();
}

xmol::polymer::Frame PdbFile::get_frame() {
  if (!m_stream->is_open()) {
    m_stream->open(m_filename, std::ios::binary);
  }
  return m_reader->read_frame();
}

xmol::polymer::Frame PdbFile::get_frame(int n) {
  if (!m_stream->is_open()) {
    m_stream->open(m_filename, std::ios::binary);
  }
  return m_reader->read_frames()[n];
}