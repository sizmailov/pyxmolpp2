#include "xmol/pdb/PdbFile.h"
#include "xmol/pdb/PdbReader.h"
#include "xmol/pdb/exceptions.h"

using namespace xmol::pdb;

PdbFile::PdbFile(const std::string& filename)
    : m_filename(filename),
      m_stream(std::unique_ptr<std::ifstream>(new std::ifstream(filename, std::ios::binary))),
      m_reader(std::unique_ptr<PdbReader>(new PdbReader(*m_stream))) {
  if (m_stream->fail()){
    throw PdbException("Can't open `"+m_filename+"`");
  }
  close();
}

void PdbFile::set_coordinates(xmol::polymer::frameIndex_t frameIndex,
                              const xmol::polymer::AtomSelection& atoms, const std::vector<int>& update_list) {
  throw xmol::BaseException("not implemented");
}

xmol::polymer::frameIndex_t PdbFile::n_frames() const {
  throw xmol::BaseException("not implemented");
}

xmol::polymer::frameIndex_t PdbFile::n_atoms_per_frame() const {
  throw xmol::BaseException("not implemented");
}

bool PdbFile::match(const xmol::polymer::AtomSelection& atoms) const {
  throw xmol::BaseException("not implemented");
}

void PdbFile::close() {
  m_stream->close();
}

xmol::polymer::Frame PdbFile::get_frame(const basic_PdbRecords& pdbRecords) {
  if (!m_stream->is_open()) {
    m_stream->open(m_filename, std::ios::binary);
  }
  return m_reader->read_frame(pdbRecords);
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
std::unique_ptr<xmol::trajectory::TrajectoryPortion> PdbFile::get_copy() const
{
  return std::unique_ptr<PdbFile>(new PdbFile(*this));
}
