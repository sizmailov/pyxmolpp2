#include "xmol/io/AmberNetCDF.h"
#include "xmol/Frame.h"
#include "xmol/geom/AngleValue.h"
#include <iostream>
#include <netcdf.h>
#include <utility>

using namespace xmol::io;

namespace {
inline void check_netcdf_call(int retval, int expected, const char* const nc_funciton_name) {
  if (retval != expected) {
    throw std::runtime_error(std::string(nc_funciton_name) + " failed (" + std::to_string(retval) + ")");
  }
}
} // namespace

AmberNetCDF::AmberNetCDF(std::string filename) : m_filename(std::move(filename)) {
  read_header();
  close();
}

void AmberNetCDF::open() {
  if (m_is_open) {
    return;
  }
  check_netcdf_call(nc_open(m_filename.c_str(), NC_NOWRITE, &m_ncid), NC_NOERR, "nc_open()");
  m_is_open = true;
}

AmberNetCDF::~AmberNetCDF() {
  if (m_is_open) {
    close();
  }
}
void AmberNetCDF::close() {
  if (m_is_open) {
    check_netcdf_call(nc_close(m_ncid), NC_NOERR, "nc_close()");
  }
  this->m_is_open = false;
}

void AmberNetCDF::print_info() {
  for (auto attr : {
           "Conventions",
           "ConventionVersion",
           "application",
           "program",
           "programVersion",
           "title",
       }) {
    std::cout << attr << ": `" << read_global_string_attr(attr) << "`" << std::endl;
  }
}
std::string AmberNetCDF::read_global_string_attr(const char* name) {
  open();
  nc_type attr_info;
  size_t attr_len;
  if (nc_inq_att(m_ncid, NC_GLOBAL, name, &attr_info, &attr_len) != NC_NOERR) {
    return std::string("<MISSING>");
  }
  char buffer[attr_len];
  check_netcdf_call(nc_get_att_text(m_ncid, NC_GLOBAL, name, buffer), NC_NOERR, "nc_get_att_text");
  return std::string(buffer, attr_len);
}

size_t xmol::io::AmberNetCDF::n_frames() const { return m_n_frames; }
size_t xmol::io::AmberNetCDF::n_atoms() const { return m_n_atoms; }
void xmol::io::AmberNetCDF::read_frame(size_t index, Frame& frame) {
  auto coordinates = frame.coords();
  this->open();
  {
    m_buffer.resize(n_atoms() * 3);

    size_t start[] = {static_cast<size_t>(m_current_frame), 0, 0};
    size_t count[] = {1, static_cast<size_t>(n_atoms()), 3};

    check_netcdf_call(nc_get_vara_float(m_ncid, m_coords_id, start, count, m_buffer.data()), NC_NOERR,
                      "nc_get_vara_float");
    CoordEigenMatrixMapf buffer_map(m_buffer.data(), n_atoms(), 3);
    coordinates._eigen() = buffer_map.cast<double>();
  }

  if (m_has_cell) {
    float lengths[3];
    float angles[3];
    size_t start[] = {static_cast<size_t>(m_current_frame), 0};
    size_t count[] = {1, 3};

    check_netcdf_call(nc_get_vara_float(m_ncid, m_cell_lengths_id, start, count, lengths), NC_NOERR,
                      "nc_get_vara_float");
    check_netcdf_call(nc_get_vara_float(m_ncid, m_cell_angles_id, start, count, angles), NC_NOERR, "nc_get_vara_float");

    frame.cell = geom::UnitCell(lengths[0], lengths[1], lengths[2], geom::Degrees(angles[0]), geom::Degrees(angles[1]),
                                geom::Degrees(angles[2]));
  }
}

void xmol::io::AmberNetCDF::advance(size_t shift) {
  m_current_frame += shift;

  if (m_current_frame >= n_frames()) {
    close();
    m_current_frame = 0;
    return;
  }

  if (!m_is_open) {
    open();
  }
}
void AmberNetCDF::read_header() {
  this->open();

  if (read_global_string_attr("Conventions").find("AMBER") == std::string::npos) {
    throw std::runtime_error("NetCDFTrajectoryFile::check(): `" + m_filename + "` does't follow AMBER conventions");
  };

  int atomid;
  check_netcdf_call(nc_inq_dimid(m_ncid, "atom", &atomid), NC_NOERR, "nc_inq_dimid()");
  check_netcdf_call(nc_inq_dimlen(m_ncid, atomid, &m_n_atoms), NC_NOERR, "nc_inq_dimlen()");

  int frames_id;
  check_netcdf_call(nc_inq_unlimdim(m_ncid, &frames_id), NC_NOERR, "nc_inq_unlimdim()");
  check_netcdf_call(nc_inq_dimlen(m_ncid, frames_id, &m_n_frames), NC_NOERR, "nc_inq_dimlen()");

  check_netcdf_call(nc_inq_varid(m_ncid, "coordinates", &m_coords_id), NC_NOERR, "nc_inq_varid");

  // cell
  auto cell_length_status = nc_inq_varid(m_ncid, "cell_lengths", &m_cell_lengths_id);
  auto cell_angles_status = nc_inq_varid(m_ncid, "cell_angles", &m_cell_angles_id);

  m_has_cell = cell_length_status == NC_NOERR && cell_angles_status == NC_NOERR;
}
