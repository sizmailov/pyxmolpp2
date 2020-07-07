#pragma once
#include "xmol/trajectory/TrajectoryFile.h"

namespace xmol::io {

/**
 * AMBER NetCDF Trajectory
 *
 *
 * Format description: https://ambermd.org/netcdf/nctraj.xhtml
 * */
class AmberNetCDF : public xmol::trajectory::TrajectoryInputFile {
public:
  AmberNetCDF(const AmberNetCDF&) = delete;
  AmberNetCDF& operator=(const AmberNetCDF&) = delete;
  AmberNetCDF(AmberNetCDF&&) = default;
  AmberNetCDF& operator=(AmberNetCDF&&) = default;
  explicit AmberNetCDF(std::string filename);
  ~AmberNetCDF();

  size_t n_frames() const final;
  size_t n_atoms() const final;
  void read_frame(size_t index, Frame& frame) final;
  void advance(size_t shift) final;

  bool has_cell() const { return m_has_cell; }

private:
  std::string m_filename;

  mutable int m_ncid;
  mutable int m_coords_id;
  mutable bool m_has_cell;
  mutable int m_cell_lengths_id;
  mutable int m_cell_angles_id;
  mutable bool m_is_open = false;

  size_t m_current_frame = 0;
  size_t m_n_frames = 0;
  size_t m_n_atoms = 0;

  std::vector<float> m_buffer;

  void open();
  void close();
  void read_header();
  void print_info();
  std::string read_global_string_attr(const char* name);
};
} // namespace xmol::io
