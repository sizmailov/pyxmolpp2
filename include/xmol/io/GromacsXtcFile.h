#pragma once

#include "xmol/Frame.h"
#include "xmol/io/xdr/XtcReader.h"
#include "xmol/trajectory/TrajectoryFile.h"

#include <vector>

namespace xmol::io {

class XtcReadError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

/// Gromacs `.xtc` input file
class GromacsXtcFile : public trajectory::TrajectoryInputFile {
public:
  explicit GromacsXtcFile(std::string filename, size_t n_frames);

  [[nodiscard]] size_t n_frames() const final;
  [[nodiscard]] size_t n_atoms() const final;
  void read_frame(size_t index, Frame& frame) final;
  void advance(size_t shift) final;

private:
  std::string m_filename;
  std::unique_ptr<xdr::XtcReader> m_reader;
  std::vector<float> m_buffer;
  int m_ahead_of_current_frame = 0;
  size_t m_current_frame = 0;
  size_t m_n_frames = 0;
  size_t m_n_atoms = 0;
};

} // namespace xmol::io