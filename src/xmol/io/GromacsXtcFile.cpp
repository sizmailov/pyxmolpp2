#include "xmol/io/GromacsXtcFile.h"

xmol::io::GromacsXtcFile::GromacsXtcFile(std::string filename, size_t n_frames)
    : m_filename(std::move(filename)), m_n_frames(n_frames) {
  xdr::XtcReader reader(m_filename);
  xdr::XtcHeader header{};
  reader.read_header(header);
  m_n_atoms = header.n_atoms;
}
size_t xmol::io::GromacsXtcFile::n_frames() const { return m_n_frames; }
size_t xmol::io::GromacsXtcFile::n_atoms() const { return m_n_atoms; }

void xmol::io::GromacsXtcFile::read_frame(size_t index, Frame& frame) {
  auto coordinates = frame.coords();
  assert(m_reader);
  assert(m_current_frame == index);
  assert(m_buffer.size() == n_atoms() * 3);
  assert(coordinates.size() == n_atoms());

  xdr::XtcHeader header{};
  std::array<float, 9> box{};

  auto status = xdr::Status::OK;
  status &= m_reader->read_header(header);
  if (!!status) {
    assert(coordinates.size() == header.n_atoms);
    status &= m_reader->read_box(box);
    if (!!status) {
      status &= m_reader->read_coords(m_buffer);
    }
  }

  if (!status) {
    throw XtcReadError("Can't read frame #" + std::to_string(index) + ": " + std::string(m_reader->last_error()));
  }
  /* I don't set frame.index = header.step as it would be any way overwritten by trajectory
   * step from input file doesn't make a lot of sense to create a separate field in Frame
   */
  frame.time = header.time;
  frame.cell = xmol::geom::UnitCell(XYZ(box[0], box[1], box[2]) * 10,
                                    XYZ(box[3], box[4], box[5]) * 10,
                                    XYZ(box[6], box[7], box[8]) * 10); // convert nanometers to angstroms

  CoordEigenMatrixMapf buffer_map(m_buffer.data(), n_atoms(), 3);
  coordinates._eigen() = buffer_map.cast<double>() * 10; // .xtc values in nanometers, convert to angstroms

  m_ahead_of_current_frame = 1;
}

void xmol::io::GromacsXtcFile::advance(size_t shift) {
  m_current_frame += shift;

  if (m_current_frame >= n_frames()) {
    m_reader.reset();
    m_buffer.clear();
    m_current_frame = 0;
    m_ahead_of_current_frame = 0;
    return;
  }

  if (!m_reader) {
    m_reader = std::make_unique<xdr::XtcReader>(m_filename);
    m_buffer.resize(n_atoms() * 3);
  }

  if (shift >= m_ahead_of_current_frame) {
    if (!m_reader->advance(shift - m_ahead_of_current_frame)) {
      throw XtcReadError("Can't advance to frame" + std::to_string(m_current_frame) + ": " + m_reader->last_error());
    }
  }
}
