#include "xmol/pdb/PdbFile.h"
#include "xmol/pdb/PdbReader.h"
#include "xmol/pdb/PdbRecord.h"
#include "xmol/pdb/exceptions.h"

using namespace xmol::pdb;
PdbFile::PdbFile(const std::string& filename) : PdbFile(filename, xmol::pdb::StandardPdbRecords::instance()){
}

PdbFile::PdbFile(const std::string& filename, const basic_PdbRecords& db)
    : m_filename(filename),
      m_stream(std::unique_ptr<std::ifstream>(new std::ifstream(filename, std::ios::binary))),
      m_reader(std::unique_ptr<PdbReader>(new PdbReader(*m_stream))),
      m_db(&db)
      {
  if (m_stream->fail()){
    throw PdbException("Can't open `"+m_filename+"`");
  }
  close();
}

void PdbFile::set_coordinates(xmol::polymer::frameIndex_t frameIndex,
                              const xmol::polymer::AtomSelection& atoms, const std::vector<int>& /*update_list*/) {
  auto frame = get_frames()[frameIndex];
  auto ats = frame.asAtoms();
  Expects(ats.size()==atoms.size());
  for (int i=0;i<ats.size();i++){
    atoms[i].set_r(ats[i].r());
  }
}

xmol::polymer::frameIndex_t PdbFile::n_frames() const {
  return get_frames().size();
}

xmol::polymer::frameIndex_t PdbFile::n_atoms_per_frame() const {
  return get_frame().asAtoms().size();
}

bool PdbFile::match(const xmol::polymer::AtomSelection& atoms) const {
  auto frame = get_frame();
  auto ats = frame.asAtoms();
  if (ats.size()!=atoms.size()){
    return false;
  }
  for (int i=0;i<ats.size();i++){
    auto&A = ats[i];
    auto&B = atoms[i];

    if (A.residue().id()!=B.residue().id()
      || A.residue().name()!=B.residue().name()
      || A.name()!=B.name()
    ){
      return false;
    }
  }
  return true;
}

void PdbFile::close() {
  m_stream->close();
}

void PdbFile::close() const {
  m_stream->close();
}

xmol::polymer::Frame PdbFile::get_frame() const{
  if (!m_stream->is_open()) {
    m_stream->open(m_filename, std::ios::binary);
  }
  auto frame = m_reader->read_frame(*m_db);
  this->close();
  return frame;
}

std::vector<xmol::polymer::Frame> PdbFile::get_frames() const{
  if (!m_stream->is_open()) {
    m_stream->open(m_filename, std::ios::binary);
  }
  auto frames = m_reader->read_frames(*m_db);
  this->close();
  return frames;
}
std::unique_ptr<xmol::trajectory::TrajectoryPortion> PdbFile::get_copy() const
{
  return std::unique_ptr<PdbFile>(new PdbFile(*this));
}
