#include "xmol/io/TrjtoolDatFile.h"
#include "xmol/geom/UnitCell.h"
#include "xmol/Frame.h"

#include <utility>

using namespace xmol::io;

namespace {

template <typename T> union FromRawBytes {
  T value;
  char bytes[sizeof(T)];
};

} // namespace

TrjtoolDatFile::TrjtoolDatFile(std::string filename) : m_filename(std::move(filename)) {
  read_header();
  advance(n_frames());
}
size_t TrjtoolDatFile::n_frames() const { return m_n_frames; }
size_t TrjtoolDatFile::n_atoms() const { return m_header.nitems; }
void TrjtoolDatFile::read_frame(size_t index, Frame& frame) {
  auto coordinates = frame.coords();
  assert(m_stream);
  assert(m_current_frame == index);
  assert(m_buffer.size() == n_atoms() * 3);
  assert(coordinates.size() == n_atoms());

  /// todo: properly handle endianness
  m_stream->read(reinterpret_cast<char*>(m_buffer.data()), sizeof(float) * n_atoms() * 3);
  CoordEigenMatrixMapf buffer_map(m_buffer.data(), n_atoms(), 3);
  coordinates._eigen() = buffer_map.cast<double>();
}
void TrjtoolDatFile::read_header() {
  m_stream = std::make_unique<std::ifstream>(m_filename, std::ios::binary);
  auto& in = *m_stream;
  in.seekg(0, std::ios::beg);

  HeaderUnion hu{};

  if (!in.read(hu.bytes, sizeof(hu.bytes))) { // todo: handle endianness
    throw std::runtime_error("TrjtoolDatFile::open(): unexpected EOF");
  }

  m_header = hu.header;
  if (m_header.dtype != 5) {
    throw std::runtime_error("TrjtoolDatFile::open(): non-float data");
  }
  for (int i = 0; i < m_header.nitems; i++) {
    FromRawBytes<int> info_len{};
    if (!in.read(info_len.bytes, sizeof(info_len.bytes))) { // todo: handle endianness
      throw std::runtime_error("TrjtoolDatFile::open(): unexpected EOF");
    }
    if (info_len.value <= 5) {
      throw std::runtime_error("TrjtoolDatFile::open(): can't read info (info_len<=5)");
    }
    char buffer[info_len.value + 1];
    if (!in.read(buffer, info_len.value)) {
      throw std::runtime_error("TrjtoolDatFile::open(): unexpected EOF");
    }
  }

  m_offset = in.tellg();
  in.seekg(0, std::ios_base::end);
  auto endpos = in.tellg();

  auto n_payload_bytes = endpos - m_offset;

  m_n_frames = n_payload_bytes / m_header.nitems / m_header.ndim / 4;
  if (m_header.nitems * m_header.ndim * 4 * m_n_frames != n_payload_bytes) {
    throw std::runtime_error("File size does not match header info");
  }
}
void TrjtoolDatFile::advance(size_t shift) {
  m_current_frame += shift;

  if (m_current_frame >= n_frames()) {
    m_stream = {};
    m_buffer.clear();
    m_current_frame = 0;
    return;
  }

  if (!m_stream) {
    m_stream = std::make_unique<std::ifstream>(m_filename, std::ios::binary);
    m_buffer.resize(n_atoms() * 3);
  }

  const std::streamoff frame_begin = sizeof(float) * m_header.nitems * m_header.ndim * m_current_frame;
  m_stream->seekg(m_offset + frame_begin, std::ios::beg);
}
