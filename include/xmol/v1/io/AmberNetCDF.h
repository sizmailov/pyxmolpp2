#pragma once
#include "xmol/v1/trajectory/TrajectoryFile.h"

namespace xmol::v1::io {

class NetCDFTrajectoryFile : public xmol::v1::trajectory::TrajectoryInputFile {
public:
  explicit NetCDFTrajectoryFile(const std::string& filename);
  ~NetCDFTrajectoryFile();

  size_t n_frames() const override;
  size_t n_atoms() const override;
  void read_coordinates(size_t index, proxy::CoordSpan& coordinates) override;
  void advance(size_t shift) override;

private:
  std::string m_filename;

  mutable int ncid;
  mutable bool m_is_open = false;

  size_t m_current_frame=0;
  size_t m_n_frames=0;
  size_t m_n_atoms=0;

  std::vector<float> m_buffer;

  void open();
  void close();
  void read_header();
  void print_info();
  std::string read_global_string_attr(const char* name);
};
} // namespace xmol::v1::io
