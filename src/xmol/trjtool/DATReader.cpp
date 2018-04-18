#include "xmol/trjtool/DATReader.h"

using namespace xmol::trjtool;

DATReader::DATReader(std::istream& in) : m_in(&in) {
  in.seekg(0, std::ios::beg);

  if (!in.read(m_header.bytes, sizeof(m_header.bytes))) {
    throw std::runtime_error("trajtool::DATReader::EOF");
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
    throw std::runtime_error("trajtool::DATReader::Unknown datatype");
  }
  }

  for (int i = 0; i < m_header.fields.nitems; i++) {
    FromRawBytes<int> info_len;
    if (!in.read(info_len.bytes, sizeof(info_len.bytes))) {
      throw std::runtime_error("trajtool::DATReader::EOF");
    }
    char buffer[info_len.value];
    if (!in.read(buffer, info_len.value)) {
      throw std::runtime_error("trajtool::DATReader::EOF");
    }
    m_info.emplace_back(std::string(buffer, info_len.value));
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
      throw std::runtime_error("File size does not match header info");
    }
    break;
  }
  case DataType::float32: {
    m_n_frames =
        n_payload_bytes / m_header.fields.nitems / m_header.fields.ndim / 4;
    if (m_header.fields.nitems * m_header.fields.ndim * 4 * m_n_frames !=
        n_payload_bytes) {
      throw std::runtime_error("File size does not match header info");
    }
    break;
  }
  default: {
    throw std::runtime_error("trajtool::DATReader::Unknown datatype");
  }
  }
}

const std::vector<std::string>& DATReader::info() const { return m_info; }

const int DATReader::n_frames() const { return m_n_frames; }

bool DATReader::match(const xmol::polymer::ConstAtomSelection& sel) const {
  if (m_data_type != DataType::float32 ||
      sel.size() != m_header.fields.nitems || m_header.fields.ndim != 3) {
    return false;
  }
  for (int i = 0; i < info().size(); i++) {
    int resid;
    std::string rname;
    std::string aname;
    ;
    if (!(std::stringstream(info()[i]) >> resid >> rname >> aname) ||
        sel[i].residue().id() != resid ||
        sel[i].name() != xmol::polymer::AtomName(aname) ||
        sel[i].residue().name() != xmol::polymer::ResidueName(rname)) {
      return false;
    }
  }
  return true;
}

void DATReader::set_frame(size_t n, const xmol::polymer::AtomSelection& sel) {
  const size_t float_size = sizeof(float);
  const std::streamoff frame_begin =
      float_size * m_header.fields.nitems * m_header.fields.ndim * n;
  m_in->seekg(m_offset + frame_begin, std::ios::beg);
  for (int i = 0; i < info().size(); i++) {
    FromRawBytes<std::tuple<float, float, float>> xyzf{};
    m_in->read(xyzf.bytes, sizeof(xyzf));
    sel[i].set_r(xmol::polymer::XYZ{std::get<0>(xyzf.value),
                                    std::get<1>(xyzf.value),
                                    std::get<2>(xyzf.value)});
  }
}
xmol::polymer::atomIndex_t DATReader::n_atoms_per_frame() const {
  return info().size();
}
