
#include "xmol/trjtool/DatFile.h"
#include <fstream>
#include <gsl/gsl_assert>
#include "xmol/trjtool/exceptions.h"
using namespace xmol::trjtool;

DatFile::DatFile(const std::string& filename)
    : m_filename(filename),
      m_stream(std::unique_ptr<std::ifstream>(new std::ifstream(filename, std::ios::binary))) {
  if (m_stream->fail()){
    throw TrjtoolException("Can't open `"+m_filename+"`");
  }
  m_reader = std::unique_ptr<DATReader>(new DATReader(*m_stream));
  this->close();
}

void DatFile::close() {
  if (m_stream->is_open()){
    m_stream->close();
  }
}

bool DatFile::match(
    const xmol::polymer::AtomSelection& atoms) const {
  return m_reader->match(atoms);
}

void DatFile::set_coordinates(xmol::polymer::frameIndex_t frameIndex, const xmol::polymer::AtomSelection& atoms,
                              const std::vector<int>& update_list) {
  Expects(frameIndex < n_frames());
  Expects(frameIndex >= 0);
  Expects(atoms.size() == update_list.size());

  if (!m_stream->is_open()) {
    m_stream->clear();
    assert(m_stream->good());
    m_stream->open(m_filename, std::ios::binary);
    if (m_stream->fail()){
      throw TrjtoolException("Can't open `"+m_filename+"`");
    }
  }
  assert(m_stream->is_open());
  assert(m_stream->good());
  m_reader->set_frame(frameIndex, atoms, update_list);
}
xmol::polymer::frameIndex_t DatFile::n_frames() const {
  return m_reader->n_frames();
}
xmol::polymer::atomIndex_t DatFile::n_atoms_per_frame() const {
  return m_reader->n_atoms_per_frame();
}
std::unique_ptr<xmol::trajectory::TrajectoryPortion> DatFile::get_copy() const
{
  return std::unique_ptr<DatFile>(new DatFile(*this));
}
