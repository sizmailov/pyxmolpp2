#include "xmol/trjtool/DATReader.h"
#include "xmol/trjtool/exceptions.h"

using namespace xmol::trjtool;


namespace {
struct XYZf{
  float x,y,z;
};

template <class T> inline void hash_combine(std::size_t& seed, const T& v) {
  seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
}

DATReader::DATReader(std::istream& in) : m_in(&in) {
  in.seekg(0, std::ios::beg);

  if (!in.read(m_header.bytes, sizeof(m_header.bytes))) {
    throw trjtool::unexpected_eof("trjtool::DATReader::EOF");
  }
  switch (m_header.fields.dtype) {
  case 2: {
    m_data_type = DataType::uint8;
    break;
  }
  case 5: {
    m_data_type = DataType::float32;
    break;
  }
  default: {
    throw trjtool::corrupted_file("trjtool::DATReader::Unknown datatype");
  }
  }
  m_atoms_hash = 0;
  for (int i = 0; i < m_header.fields.nitems; i++) {
    FromRawBytes<int> info_len;
    if (!in.read(info_len.bytes, sizeof(info_len.bytes))) {
      throw trjtool::unexpected_eof("trjtool::DATReader::EOF");
    }
    if (info_len.value<=5){
      throw trjtool::corrupted_file("Can't read info (info_len<=5)");
    }
    char buffer[info_len.value+1];
    if (!in.read(buffer, info_len.value)) {
      throw trjtool::unexpected_eof("trjtool::DATReader::EOF");
    }
    std::stringstream ss(std::string(buffer+5, info_len.value-5));
    int resid;
    std::string rname;
    std::string aname;
    if (ss >> resid >> rname >> aname) {
      hash_combine(m_atoms_hash,xmol::polymer::residueId_t(resid));
      hash_combine(m_atoms_hash,xmol::polymer::ResidueName(rname));
      hash_combine(m_atoms_hash,xmol::polymer::AtomName(aname));
    } else {
      throw trjtool::corrupted_file("Can't read info");
    }
  }

  m_offset = in.tellg();
  in.seekg(0, std::ios_base::end);
  auto endpos = in.tellg();

  auto n_payload_bytes = endpos - m_offset;

  switch (m_data_type) {
  case DataType::uint8: {
    m_n_frames =
        n_payload_bytes / m_header.fields.nitems / m_header.fields.ndim / 1;
    if (m_header.fields.nitems * m_header.fields.ndim * 1 * m_n_frames !=
        n_payload_bytes) {
      throw trjtool::corrupted_file("File size does not match header info");
    }
    break;
  }
  case DataType::float32: {
    m_n_frames =
        n_payload_bytes / m_header.fields.nitems / m_header.fields.ndim / 4;
    if (m_header.fields.nitems * m_header.fields.ndim * 4 * m_n_frames !=
        n_payload_bytes) {
      throw trjtool::corrupted_file("File size does not match header info");
    }
    break;
  }
  default: {
    throw trjtool::corrupted_file("trjtool::DATReader::Unknown datatype");
  }
  }
}

int DATReader::n_frames() const { return m_n_frames; }

bool DATReader::match(const xmol::polymer::ConstAtomSelection& sel) const {
  if (m_data_type != DataType::float32 ||
      sel.size() != m_header.fields.nitems || m_header.fields.ndim != 3) {
    return false;
  }
  size_t sel_hash=0;
  for (auto& a: sel) {
    hash_combine(sel_hash,a.residue().id());
    hash_combine(sel_hash,a.residue().name());
    hash_combine(sel_hash,a.name());
  }
  return sel_hash==m_atoms_hash;
}

void DATReader::set_frame(size_t n, const xmol::polymer::AtomSelection& sel, const std::vector<int>& indices) {
  const size_t float_size = sizeof(float);
  const std::streamoff frame_begin =
      float_size * m_header.fields.nitems * m_header.fields.ndim * n;
  m_in->seekg(m_offset + frame_begin, std::ios::beg);
  int curpos = 0;
  for (auto i: indices) {
    if (curpos!=i){
      m_in->seekg(sizeof(XYZf)*(i-curpos),std::ios::cur);
      curpos=i;
    }
    FromRawBytes<XYZf> xyzf{};
    m_in->read(xyzf.bytes, sizeof(xyzf));
    ++curpos;
    sel[i].set_r(xmol::polymer::XYZ{xyzf.value.x,
                                    xyzf.value.y,
                                    xyzf.value.z});
  }
}
xmol::polymer::atomIndex_t DATReader::n_atoms_per_frame() const { return m_header.fields.nitems; }
