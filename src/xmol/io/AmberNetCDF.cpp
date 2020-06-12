#include "xmol/io/AmberNetCDF.h"
#include <iostream>
#include <netcdf.h>

using namespace xmol::io;

namespace {
inline void check_netcdf_call(int retval, int expected, const char* const nc_funciton_name) {
  if (retval != expected) {
    throw std::runtime_error(std::string(nc_funciton_name) + " failed (" + std::to_string(retval) + ")");
  }
}
} // namespace

AmberNetCDF::AmberNetCDF(const std::string& filename) : m_filename(filename) {
  read_header();
  close();
}

void AmberNetCDF::open() {
  if (m_is_open) {
    return;
  }
  check_netcdf_call(nc_open(m_filename.c_str(), NC_NOWRITE, &ncid), NC_NOERR, "nc_open()");
  m_is_open = true;
}

AmberNetCDF::~AmberNetCDF() {
  if (m_is_open) {
    close();
  }
}
void AmberNetCDF::close() {
  if (m_is_open) {
    check_netcdf_call(nc_close(ncid), NC_NOERR, "nc_close()");
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
  if (nc_inq_att(ncid, NC_GLOBAL, name, &attr_info, &attr_len) != NC_NOERR) {
    return std::string("<MISSING>");
  }
  char buffer[attr_len];
  check_netcdf_call(nc_get_att_text(ncid, NC_GLOBAL, name, buffer), NC_NOERR, "nc_get_att_text");
  return std::string(buffer, attr_len);
}

size_t xmol::io::AmberNetCDF::n_frames() const { return m_n_frames; }
size_t xmol::io::AmberNetCDF::n_atoms() const { return m_n_atoms; }
void xmol::io::AmberNetCDF::read_coordinates(size_t index, xmol::proxy::CoordSpan& coordinates) {
  this->open();
  m_buffer.resize(n_atoms() * 3);

  size_t start[] = {static_cast<size_t>(m_current_frame), 0, 0};
  size_t count[] = {1, static_cast<size_t>(n_atoms()), 3};

  int coords_id;

  nc_inq_varid(ncid, "coordinates", &coords_id);

  check_netcdf_call(nc_get_vara_float(ncid, coords_id, start, count, m_buffer.data()), NC_NOERR, "nc_get_vara_float");
  CoordEigenMatrixMapf buffer_map(m_buffer.data(), n_atoms(), 3);
  coordinates._eigen() = buffer_map.cast<double>();
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
  check_netcdf_call(nc_inq_dimid(ncid, "atom", &atomid), NC_NOERR, "nc_inq_dimid()");
  check_netcdf_call(nc_inq_dimlen(ncid, atomid, &m_n_atoms), NC_NOERR, "nc_inq_dimlen()");

  int frames_id;
  check_netcdf_call(nc_inq_unlimdim(ncid, &frames_id), NC_NOERR, "nc_inq_unlimdim()");
  check_netcdf_call(nc_inq_dimlen(ncid, frames_id, &m_n_frames), NC_NOERR, "nc_inq_dimlen()");
}
